#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	cv::Mat IplImage; 
	IplImage = cv::imread(argv[1],cv::IMREAD_COLOR);
	cv::namedWindow("Original", cv::WINDOW_AUTOSIZE );
	cv::imshow("Original",IplImage);
	cv::waitKey(0);
	cv::Mat imgGrayScale;
	cv::cvtColor( IplImage, imgGrayScale, CV_BGR2GRAY );
	cv::threshold(imgGrayScale,imgGrayScale,128,255,CV_THRESH_BINARY);  
	std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(imgGrayScale, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
  std::vector<std::vector<cv::Point> > result(contours.size());
  int thickness = 2;
  int lineType = 8;
  for(int i = 0;i < contours.size();i++)
  {
  	int len = cv::arcLength(contours[i], true);
  	cv::approxPolyDP(contours[i],result[i],0.02*len, true);
  	std::cout<<i<<std::endl;
  	std::cout <<result[i].size()<<std::endl;
  	if(result[i].size() == 3)
  	{
  	 	std::cout<<"TRIANGLE"<<std::endl;
  	 	cv::Point p[3];
  	 	for(int j = 0;j<3;j++)
  	 	{
  	 		p[j] = result[i][j];
  	 	}
  	 		
  		cv::line(IplImage,p[0],p[1],cv::Scalar(255, 0, 0 ),thickness,lineType);
  		cv::line(IplImage,p[1],p[2],cv::Scalar(255, 0, 0 ),thickness,lineType);
  		cv::line(IplImage,p[2],p[0],cv::Scalar(255, 0, 0 ),thickness,lineType);
  	 		
  	}
  	if(result[i].size() == 4)
  	{
  	  std::cout<<"RECTANGLE"<<std::endl;
  	 	cv::Point p[4];
  	 	for(int j = 0;j<4;j++)
  	 	{
  	 		p[j] = result[i][j];
  	 	}
  	 		
  		cv::line(IplImage,p[0],p[1],cv::Scalar(0, 255, 0),thickness,lineType);
  		cv::line(IplImage,p[1],p[2],cv::Scalar(0, 255, 0),thickness,lineType);
  		cv::line(IplImage,p[2],p[3],cv::Scalar(0, 255, 0),thickness,lineType);
  		cv::line(IplImage,p[3],p[0],cv::Scalar(0, 255, 0),thickness,lineType);
  	 }

  }
  cv::namedWindow("Final", cv::WINDOW_AUTOSIZE );
	cv::imshow("Final",IplImage);
	cv::waitKey(0);
	return 0;
}