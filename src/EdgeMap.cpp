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
	for (int i= 0; i < 2; i++)
	{
		video >> LastImage;
		video >> curImage;
	}
	
	//video >> LastImage;
	//video >> curImage;
}
void EdgeMap::boucle(string filePath){
	video = cv::VideoCapture(filePath);
	video >> LastImage;
	int indiceb=0;
	while(1){
		video >> curImage;
    	if (curImage.empty() ||indiceb==30){
			break;
		}	
		cout << indiceb << endl;
		process(LastImage,curImage,indiceb);
	curImage.copyTo(LastImage);
	indiceb++;
  }
}
//pour avoir les background edges
Mat EdgeMap::BackgroundEdge(){
	int numberofframes=300;
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
void EdgeMap::process(Mat LastI,Mat curI,int indiceb)
{	
	//imshow("oskour",LastI);
	//waitKey(0);
	//imshow("oskour",curI);
	//waitKey(0);

	cv::Mat CannyCurImage,CannyLastImage,res,Edges,Edgesbis,Bedges,placeholder;

	//canny pour avoir les differents edge map que l'on a besoin
	Canny(curI,CannyCurImage,100,200);
    Canny(LastI,CannyLastImage,100,200);
	absdiff(CannyLastImage,CannyCurImage,Edges);
	absdiff(LastI,curI,res);
	//threshold
	double thres_val = getThreshVal_Otsu_8u(res);
	Canny(res,Edgesbis,thres_val*0.5,thres_val);
	//Bedges = BackgroundEdge();
	vector<Edgecoord> En = MatToVector(CannyCurImage);
	vector<Edgecoord> DEn = MatToVector(Edgesbis);
	vector<Edgecoord> Denprevious =MatToVector(CannyLastImage);
	vector<Edgecoord> MEchange,MEStill;
	
	//imshow("oskour",curI);
	//waitKey(0);
	//calcul des edge maps mouvante et non mouvante
	double Tdist=5;
	for (int i = 0; i < En.size(); i++)
	{
		//TODO DEBOGUER LA , CORE DUMP Parceque je cheque pas les I des autres vector mais 
		// si je fais double for ça va prendre 600 minutes pour UNE frame
		for (int j = 0; j < DEn.size(); j++){
			Edgecoord temp ={En[i].x,En[i].y};
	    	if(distanceBetweenTwoPoints(En[i].x,En[i].y,DEn[j].x,DEn[j].y)<=Tdist){
				MEchange.push_back({En[i].x,En[i].y});
			}
		}     
		for (int j = 0; j < Denprevious.size(); j++){        
		//if(Bedges.at<uchar>(En[i].x,En[i].y)=0){
			if(distanceBetweenTwoPoints(En[i].x,En[i].y,Denprevious[j].x,Denprevious[j].y)<=Tdist){
				MEStill.push_back({En[i].x,En[i].y});
			}  
		}             
	}   
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           


	
	//reunion des deux Moving edges pour faire la final edge map
	Mat finaledgemap(cv::Size(curI.cols, curI.rows),CV_8U,Scalar(255));

	
	for (int i = 0; i < MEchange.size(); i++)
	{
		finaledgemap.at<uchar>(MEchange[i].x,MEchange[i].y)=0;
	}
	for (int i = 0; i < MEStill.size(); i++)
	{
		finaledgemap.at<uchar>(MEStill[i].x,MEStill[i].y)=0;
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
				if(finaledgemap.at<uchar>(i,j)==0){
					first =j;
				}
			}
			else{
				if(finaledgemap.at<uchar>(i,j)==0){
					indexlast =j;
				}
			}
		}
		if(first!=-1 && indexlast!=-1){
			for (int j = first; j <= indexlast; j++)
			{
				horizontalcand.push_back({i,j});
			}
		}
		else if (first!=-1 && indexlast==-1){
			for (int j = first; j <= finaledgemap.rows; j++){
				horizontalcand.push_back({i,j});
			}
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
				if(finaledgemap.at<uchar>(i,j)==0){
					first =j;
				}
			}
			else{
				if(finaledgemap.at<uchar>(i,j)==0){
					indexlast =j;
				}
			}
		}
		if(first!=-1 && indexlast!=-1){
			for (int j = first; j <= indexlast; j++){
				verticalcand.push_back({j,i});
			}
		}
		else if (first!=-1 && indexlast==-1){
			for (int j = first; j <= finaledgemap.rows; j++){
				verticalcand.push_back({j,i});
			}
		}
	}
	
	//Trouver les VOP avec les candidats avec un AND
	Mat VOPS(cv::Size(curI.cols, curI.rows),CV_8U,Scalar(255));
	for(int j=0 ; j < horizontalcand.size();j++){
		VOPS.at<uchar>(horizontalcand[j].x,horizontalcand[j].y)=0;
	}
	//imshow("oskour",VOPS);
	//waitKey(0);
	/*
	for(int i ; i < verticalcand.size();i++){
			//cout << endl;
			//cout << endl;
			//cout <<VOPS.rows << " " << VOPS.cols<< endl;
			//cout <<verticalcand[i].x << " " << verticalcand[i].y<< endl;
			VOPS.at<uchar>(verticalcand[i].x,verticalcand[i].y)=0;
	}
	*/



	//label extracted vops
	imwrite("res.png"+to_string(indiceb)+".png",VOPS);
	//imshow("show",VOPS);
	//waitKey(0);
	Mat labels(VOPS.size(),CV_32S);

	//Mat 
} 

vector<Edgecoord> EdgeMap::MatToVector(Mat toconvertmat){
	vector<Edgecoord> vec;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	for (int i = 0; i < toconvertmat.rows; i++)
    {
        for (int j = 0; j < toconvertmat.cols; j++)
		{
			if(toconvertmat.at<uchar>(i,j)==255){
				vec.push_back({i,j});
			}
		}
	} 
	return vec;
}
double EdgeMap::distanceBetweenTwoPoints(double x, double y, double a, double b){
	return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

double EdgeMap::getThreshVal_Otsu_8u( const cv::Mat& _src )
{
    cv::Size size = _src.size();
    if ( _src.isContinuous() )
    {
        size.width *= size.height;
        size.height = 1;
    }
    const int N = 256;
    int i, j, h[N] = {0};
    for ( i = 0; i < size.height; i++ )
    {
        const uchar* src = _src.data + _src.step*i;
        for ( j = 0; j <= size.width - 4; j += 4 )
        {
            int v0 = src[j], v1 = src[j+1];
            h[v0]++; h[v1]++;
            v0 = src[j+2]; v1 = src[j+3];
            h[v0]++; h[v1]++;
        }
        for ( ; j < size.width; j++ )
            h[src[j]]++;
    }

    double mu = 0, scale = 1./(size.width*size.height);
    for ( i = 0; i < N; i++ )
        mu += i*h[i];

    mu *= scale;
    double mu1 = 0, q1 = 0;
    double max_sigma = 0, max_val = 0;

    for ( i = 0; i < N; i++ )
    {
        double p_i, q2, mu2, sigma;

        p_i = h[i]*scale;
        mu1 *= q1;
        q1 += p_i;
        q2 = 1. - q1;

        if ( std::min(q1,q2) < FLT_EPSILON || std::max(q1,q2) > 1. - FLT_EPSILON )
            continue;

        mu1 = (mu1 + i*p_i)/q1;
        mu2 = (mu - q1*mu1)/q2;
        sigma = q1*q2*(mu1 - mu2)*(mu1 - mu2);
        if ( sigma > max_sigma )
        {
            max_sigma = sigma;
            max_val = i;
        }
    }

    return max_val;
}
