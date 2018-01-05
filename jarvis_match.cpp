#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int orientation(cv::Point p,cv::Point q,cv::Point r)
{
	int val = (q.y - p.y)*(r.x-q.x) - (q.x - p.x)*(r.y - q.y);
	if (val == 0)
		return 0;
	return (val>0)? 1:2; //clockwise or counter clockwise
}

std::vector<cv::Point> extractpoints(cv::Mat img)
{
	std::vector<cv::Point> p;
	cv::Scalar temp;
	for (int y = 0; y < img.rows; ++y)
	{
		for (int x = 0; x < img.cols; ++x)
		{
			temp = img.at<uchar>(cv::Point(x, y));
			if(temp.val[0] >= 128)
				p.push_back(cv::Point(x, y));
		}
	}
	return p;
}


void convexHull(cv::Mat Image)
{	
	cv::Mat img;
	cv::cvtColor( Image, img, CV_BGR2GRAY );
	std::vector<cv::Point> points;
	std::vector<cv::Point> hull;
	points = extractpoints(img);
	int n = points.size();
	if(n < 3)
	{
		std::cout<<"CONVEX_HULL_NOT_POSSIBLE"<<std::endl;
		return ;
	}
	int min = 0;
	for (int i = 1; i < n; ++i)
	{
		if(points[i].x < points[min].x)
			min = i;
	}
	int p = min,q;
	do
	{
		hull.push_back(points[p]);
		q = (p+1)%n;
		for (int i = 0; i < n; ++i)
		{
			if(orientation(points[p], points[i], points[q]) == 2)
				q = i;
		}
		p = q;
	}while(p != min);
	for (int i = 0; i < hull.size(); ++i)
	{
		std::cout<<hull[i]<<std::endl;
	}
	int thickness = 2;
  	int lineType = 7;
	for (int i = 0; i < hull.size(); ++i)
	{
		cv::line(Image,hull[i%hull.size()],hull[(i+1)%hull.size()],cv::Scalar(255, 0, 0 ),thickness,lineType);
	}
	cv::namedWindow("Convex_Hull", cv::WINDOW_AUTOSIZE );
	cv::imshow("Convex_Hull",Image);
	cv::waitKey(0);
}



int main(int argc, char **argv)
{
	cv::Mat Image; 
	Image = cv::imread(argv[1],cv::IMREAD_COLOR);
	cv::namedWindow("Original", cv::WINDOW_AUTOSIZE );
	cv::imshow("Original",Image);
	cv::waitKey(0);
	std::vector<cv::Point> q;
	convexHull(Image);
	return 0;
}