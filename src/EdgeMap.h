#ifndef SOURCE_DIRECTORY__PreProcess_H_
#define SOURCE_DIRECTORY__PreProcess_H_

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

/**
 * 
 */
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
	 * Open and load an image located in the system at filePath
	 */
	bool open(string filePath);

	Mat BackgroundEdge();
	/**
	 * Shows the interface with the input image
	 */
	void process();
	vector<Edgecoord> MatToVector(Mat mat);
};

#endif
