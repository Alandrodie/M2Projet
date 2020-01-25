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

	for (int i = 0; i < En.size(); i++)
	{
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	}
	
   

}

vector<Edgecoord> MatToVector(Mat mat){
	vector<Edgecoord> vec;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 q
	for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
		{
			if(mat.at<uchar>(i,j)==0)
				vec.push_back({i,j})
		}
	} 
}
