#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int MAX_KERNEL_LENGTH = 20;
int scale = 1;
int delta = 0;
int ddepth = CV_16S;
int const max_elem = 2;
int const max_kernel_size = 21;
cv::Mat grad_x, grad_y;
cv::Mat abs_grad_x, abs_grad_y;




int main(int argc, char **argv)
{
  cv::Mat img;
	img = cv::imread(argv[1],cv::IMREAD_COLOR);
	cv::namedWindow("Original", cv::WINDOW_AUTOSIZE );
	cv::imshow("Original", img);
	cv::waitKey(0);
	cv::Mat im_smooth;
	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
  { 
    cv::GaussianBlur(img, im_smooth, cv::Size( i, i ), 0, 0 );
  }
  cv::Mat im_gray;
  cv::cvtColor( im_smooth, im_gray, CV_BGR2GRAY);

  cv::Sobel( im_gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
  cv::convertScaleAbs( grad_y, abs_grad_y );
  cv::Mat final1,final2;
  final2 = abs_grad_y;
  
  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
  { 
    cv::bilateralFilter ( final2, final1, i, i*2, i/2 );
  }
  cv::Mat bw;
  cv::adaptiveThreshold(final1, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, -2);
  cv::Mat horizontal = bw.clone();
  // Specify size on horizontal axis
  int horizontalsize = horizontal.cols / 30;
  // Create structure element for extracting horizontal lines through morphology operations
  cv::Mat horizontalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontalsize,1));
  // Apply morphology operations
  cv::erode(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
  cv::dilate(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
  // Show extracted horizontal lines
  cv::namedWindow("horizontal", cv::WINDOW_AUTOSIZE);
  cv::imshow("horizontal", horizontal);
  cv::waitKey(0);

  
  //Hough Lines
  std::vector<cv::Vec2f> lines;
  cv::HoughLines(horizontal, lines, 1, CV_PI/180, 300, 0, 0 );
  for( size_t i = 0; i < lines.size(); i++ )
    {
      float rho = lines[i][0], theta = lines[i][1];
      if( theta>CV_PI/180*88 && theta<CV_PI/180*92)
      {
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cv::line( img, pt1, pt2, cv::Scalar(0,0,255), 4, CV_AA);
      }
    }
  cv::namedWindow("Final", cv::WINDOW_AUTOSIZE);
  cv::imshow("Final",img);
  cv::waitKey(0);
	return 0; 
}