#include "EdgeMap.h"

struct Edgecoord{
	int x,y;
};
EdgeMap::EdgeMap()
{
}

EdgeMap::~EdgeMap()
{
}

bool EdgeMap::open(string filePath)
{
	video = cv::VideoCapture(filePath);

	video >> LastImage;
	video >> curImage;
}

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
void EdgeMap::process()
{	
	cv::Mat CannyCurImage,CannyLastImage,res,Edges,Edgesbis,Bedges;
	Canny(curImage,CannyCurImage,100,200);
    Canny(LastImage,CannyLastImage,100,200);
	absdiff(CannyLastImage,CannyCurImage,Edges);
	absdiff(curImage,LastImage,res);
	Canny(res,Edgesbis,100,200);
	Bedges = BackgroundEdge();
	vector<Edgecoord> En = MatToVector(CannyCurImage);
	vector<Edgecoord> DEn = MatToVector(Edgesbis);
	vector<Edgecoord> DEn = MatToVector(Edgesbis);
	vector<Edgecoord> Denprevious ; //TODO PREVIOUS EDGEMAP
	double Tdist=5;
	vector<Edgecoord> MEchange,MEStill;
	
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
	
	//reunion des deux Moving edges set 
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