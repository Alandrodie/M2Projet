#include "EdgeMap.h"

EdgeMap::EdgeMap()
{
}

EdgeMap::~EdgeMap()
{
}
//ouvrir la video et prendre les deux premieres frames
bool EdgeMap::open(string filePath)
{
	video = cv::VideoCapture(filePath);

	video >> LastImage;
	video >> curImage;
}
//pour avoir les background edges
Mat EdgeMap::BackgroundEdge(){
	int numberofframes=20;
	Mat curframe;
	Mat Bedges;
	cv::Mat CannyCurImage,BackgroundEdge;
	cv::Ptr<BackgroundSubtractorMOG2> bsubstracor  = createBackgroundSubtractorMOG2(100,50,true);
	for(int k =0; k <numberofframes;k++){
		video >> curframe;
		bsubstracor->apply(curframe,Bedges);
	
	}
	return Bedges;
}
//process principal
void EdgeMap::process()
{	
	cv::Mat CannyCurImage,CannyLastImage,res,Edges,Edgesbis,Bedges;

	//canny pour avoir les differents edge map que l'on a besoin
	Canny(curImage,CannyCurImage,100,200);
    Canny(LastImage,CannyLastImage,100,200);
	absdiff(CannyLastImage,CannyCurImage,Edges);
	absdiff(curImage,LastImage,res);
	Canny(res,Edgesbis,100,200);
	Bedges = BackgroundEdge();
	vector<Edgecoord> En = MatToVector(CannyCurImage);
	vector<Edgecoord> DEn = MatToVector(Edgesbis);
	vector<Edgecoord> Denprevious =MatToVector(CannyLastImage);
	double Tdist=5;
	vector<Edgecoord> MEchange,MEStill;
	//calcul des edge maps mouvante et non mouvante
	for (int i = 0; i < En.size(); i++)
	{
	    if(distanceBetweenTwoPoints(En[i].x,En[i].y,DEn[i].x,DEn[i].y)<=Tdist){
			MEchange.push_back({En[i].y,En[i].x});
		}                
		if(Bedges.at<uchar>(En[i].x,En[i].y)=0){
			if(distanceBetweenTwoPoints(En[i].x,En[i].y,Denprevious[i].x,Denprevious[i].y)<=Tdist){
				MEStill.push_back({En[i].y,En[i].x});
			}                
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	}
	
	//reunion des deux Moving edges pour faire la final edge map
	Mat finaledgemap(3,3,CV_32F, Scalar(255));
	for (int i = 0; i < MEchange.size(); i++)
	{
		finaledgemap.at<uchar>(MEchange[i].y,MEchange[i].x)=0;
	}
	for (int i = 0; i < MEStill.size(); i++)
	{
		finaledgemap.at<uchar>(MEStill[i].y,MEStill[i].x)=0;
	}
   
	//parcours de la final edge map pour trouver les candidats
	//horizontal
	vector<Edgecoord> horizontalcand;
		for (int i = 0; i < finaledgemap.rows; i++)
    {
		int first = -1;
		int indexlast=-1;
        for (int j = 0; j < finaledgemap.cols; j++)
		{
			if(first==-1){
				if(finaledgemap.at<uchar>(j,i)==0){
					first =j;
				}
			}
			else{
				if(finaledgemap.at<uchar>(j,i)==0){
					indexlast =j;
				}
			}
		}
		if(first!=-1 && indexlast!=-1){
			horizontalcand.push_back({i,first});
			horizontalcand.push_back({i,indexlast});
		}
		else if (first!=-1 && indexlast==-1){
			horizontalcand.push_back({i,first});
			horizontalcand.push_back({i,finaledgemap.cols});
		}
		
	} 
		//vertical
		vector<Edgecoord> verticalcand;
		for (int i = 0; i < finaledgemap.cols; i++)
    {
		int first = -1;
		int indexlast=-1;
        for (int j = 0; j < finaledgemap.rows; j++)
		{
			if(first==-1){
				if(finaledgemap.at<uchar>(j,i)==0){
					first =j;
				}
			}
			else{
				if(finaledgemap.at<uchar>(j,i)==0){
					indexlast =j;
				}
			}
		}
		if(first!=-1 && indexlast!=-1){
			verticalcand.push_back({first,i});
			verticalcand.push_back({indexlast,i});
		}
		else if (first!=-1 && indexlast==-1){
			verticalcand.push_back({first,i});
			verticalcand.push_back({finaledgemap.rows,i});
		}
	}
	//Trouver les VOP avec les candidats avec un AND
	Mat VOPS(3,3,CV_32F, Scalar(255));
	for(int i ; i < verticalcand.size();i++){
		for(int j ; j < horizontalcand.size();j++){
		if(verticalcand[i].x == horizontalcand[i].x && verticalcand[i].y== horizontalcand[i].y)
			VOPS.at<uchar>(j,i)=0;
		}
	}
	imshow("test",VOPS);
	waitKey(0);
	//label extracted vops
	Mat labels(VOPS.size(),CV_32S);
	int nLabels = connectedComponents(VOPS,labels,8);
	vector<Vec3b> colors(nLabels);
	colors[0]= Vec3b(0,0,0);
	for(int i=1; i< nLabels ; ++i){
		colors[i]=Vec3b ( (rand()&255),(rand()&255),(rand()&255));
	}
	Mat labeled(VOPS.size(),CV_8UC3);
	for(int i ; i < labeled.rows;i++){
		for(int j ; j < labeled.cols;j++){
			int label = labels.at<int>(i,j);
			Vec3b &pixel = labeled.at<Vec3b>(i,j);
			pixel = colors[label];
		}
	}
	imshow("test",labeled);
	waitKey(0);
} 

vector<Edgecoord> EdgeMap::MatToVector(Mat toconvertmat){
	vector<Edgecoord> vec;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	for (int i = 0; i < toconvertmat.rows; i++)
    {
        for (int j = 0; j < toconvertmat.cols; j++)
		{
			if(toconvertmat.at<uchar>(i,j)==0){
				vec.push_back({i,j});
			}
		}
	} 
	return vec;
}
double EdgeMap::distanceBetweenTwoPoints(double x, double y, double a, double b){
	return sqrt(pow(x - a, 2) + pow(y - b, 2));
}