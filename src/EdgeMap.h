
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

/**
 * 
 */
struct Edgecoord{
	int x,y;
};
class EdgeMap
{
  private:
	string inputFilePath;
	cv::Mat curImage;
	cv::Mat LastImage;
	VideoCapture video;

  public:
	/**
	 * Default constructor
	 */
	EdgeMap();
	/**
	 * Destructor
	 */
	virtual ~EdgeMap();

	/**
	 * Open and load a video  located in the system at filePath
	 */
	bool open(string filePath);
	/**
	 * Get distance between two points
	 */
	double distanceBetweenTwoPoints(double x, double y, double a, double b);
	/**
	 * return the background edges map of the current frame
	 */
	Mat BackgroundEdge();
	/**
	 * launch process
	 */
	void process();
	vector<Edgecoord> MatToVector(Mat toconvertmat);
	double getThreshVal_Otsu_8u( const cv::Mat& _src );
};

