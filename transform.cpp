#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


//top-left
//top-right
//bottom-right
//bottom-left

cv::Point2f p[4];
cv::Point2f d[4];
int i=0;

void calculate_size()
{
	cv::Point tl = p[0], tr = p[1], br = p[2], bl = p[3];
	float width = (std::max(sqrt((tr.x-tl.x)*(tr.x-tl.x)+(tr.y-tl.y)*(tr.y-tl.y)),sqrt((br.x-bl.x)*(br.x-bl.x)+(br.y-bl.y)*(br.y-bl.y))));
	float height = (std::max(sqrt((tr.x-br.x)*(tr.x-br.x)+(tr.y-br.y)*(tr.y-br.y)),sqrt((tl.x-bl.x)*(tl.x-bl.x)+(tl.y-bl.y)*(tl.y-bl.y))));
	d[0] = cv::Point2f(0,0);
	d[1] = cv::Point2f((width-1),0);
	d[2] = cv::Point2f((width-1),(height-1));
	d[3] = cv::Point2f(0,(height-1));
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if(event == cv::EVENT_LBUTTONDBLCLK)
	{
		p[i].x = x;
		p[i].y = y;
		std::cout<<p[i]<<std::endl;
		i++;
	}

}

int main(int argc, char **argv)
{
	cv::Mat img; 
	img = cv::imread(argv[1],cv::IMREAD_COLOR);
	cv::namedWindow("Original", cv::WINDOW_AUTOSIZE );
	cv::setMouseCallback("Original",CallBackFunc, NULL);
	cv::imshow("Original",img);
	cv::waitKey(0);
	calculate_size();
	cv::Mat M;
	M = cv::getPerspectiveTransform(p,d);
	cv::Mat dst;
	cv::warpPerspective(img, dst, M, dst.size());
	cv::namedWindow("Transform", cv::WINDOW_AUTOSIZE );
	cv::imshow("Transform",dst);
	cv::waitKey(0);
	return 0;
}