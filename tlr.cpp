#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<math.h>

using namespace std;
using namespace cv;

void func(int pos,void* data);
int isvalid(Mat img,int i,int j)
{
	if(i<0 || j<0 || i>=img.rows || j>=img.cols)
		return 0;
	return 1;
}
Mat cand_region(Mat img);
Mat Edge(Mat);
Mat hough(Mat);
int ma=0;
int p=0;

int main()
{
	namedWindow("trackbar",WINDOW_NORMAL);
	int f=0;
	createTrackbar("factor","trackbar",&f,1000,func,(void*)&f);
	waitKey(0);
}

void func(int pos,void* data)
{
	//int max=0;
	p=pos;
	Mat img=imread("traffic_2.jpg",CV_LOAD_IMAGE_COLOR);
	Mat img2(img.rows,img.cols,CV_8UC3,Scalar(0,0,0));
	for(int i=0;i<img.rows;i++)
		for(int j=0;j<img.cols;j++)
		{
			float sum=0;
			for(int k=0;k<3;k++)
				sum+=img.at<Vec3b>(i,j)[k];
			if(ma<sum)
				ma=sum;
			for(int k=0;k<3;k++)
			{
				if(!sum)
					img2.at<Vec3b>(i,j)[k]=0;
				else
				{
					img2.at<Vec3b>(i,j)[k]=(img.at<Vec3b>(i,j)[k]*pos/sum);
				}
			}
		}
	// cout<<ma<<endl;
	imshow("original",img);
	imshow("nbgr",img2);
	Mat cand_rgn=cand_region(img);
	imshow("cand_region",cand_rgn);
	Mat edge=Edge(cand_rgn);
	// Mat edge;
	// Canny(cand_rgn,edge,80,200,3);
	imshow("edge",edge);
	//Mat disc=imread("test4.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	
	// imwrite("edge.jpg",edge);
	
	//Canny(edge,edge,80,200,3);
	//imshow("edge",edge);
	Mat hough_cir=hough(edge);
	imshow("circles",hough_cir);
	//waitKey(0);
}

Mat cand_region(Mat img)
{
	for(int i=0;i<img.rows;i++)
		for(int j=0;j<img.cols;j++)
		{
			int b=img.at<Vec3b>(i,j)[0];
			int g=img.at<Vec3b>(i,j)[1];
			int r=img.at<Vec3b>(i,j)[2];
			if(!((r>150 && g<100 && b<100) || (r>150 && g>100 && b<100) || (r<100 && g>190 && b>170)))
				img.at<Vec3b>(i,j)={0,0,0};
		}
	return img;
}

Mat Edge(Mat im)
{
	Mat img(im.rows,im.cols,CV_8UC1,Scalar(0));
	int scale=1,delta=0,ddepth=CV_16S;
	Mat edge;
	cvtColor(im,img,CV_BGR2GRAY);
	Mat grad_x,grad_y;
	Mat abs_grad_x,abs_grad_y;
	Sobel(img,grad_x,ddepth,1,0,3,scale,delta,BORDER_DEFAULT);
	convertScaleAbs(grad_x,abs_grad_x);

	Sobel(img,grad_y,ddepth,0,1,3,scale,delta,BORDER_DEFAULT);
	convertScaleAbs(grad_y,abs_grad_y);

	addWeighted(abs_grad_x,0.5,abs_grad_y,0.5,0,edge);
	// for(int i=0;i<img.rows;i++)
	// 	for(int j=0;j<img.cols;j++)
	// 		if(edge.at<uchar>(i,j)!=0)
	// 			edge.at<uchar>(i,j)=255;
	return edge;
}

Mat hough(Mat img)
{
	// Mat img2(img.rows,img.cols,CV_8UC3,Scalar(0,0,0));
	// vector<Vec3f> circles;
	// HoughCircles(img,circles,CV_HOUGH_GRADIENT,3,img.rows/8,150,100,0,0);
	// cout<<circles.size	// for(int i=0;i<circles.size();i++)
	// {
	// 	Point centre(cvRound(circles[i][0]),cvRound(circles[i][1]));
	// 	int radius=cvRound(circles[i][2]);
	// 	//circle(img2,centre,3,Scalar(0,255,0),-1,8,0);
	// 	circle(img2,centre,radius,Scalar(0,0,255),3,8,0);
	// }
	// return img2;

	int ***vote;
	vote=new int**[img.rows];
	for(int i=0;i<img.rows;i++)
		vote[i]=new int*[img.cols];
	for(int i=0;i<img.rows;i++)
		for(int j=0;j<img.cols;j++)
			vote[i][j]=new int[i>j?j:i];
	
	for(int i=0;i<img.rows;i++)
		for(int j=0;j<img.cols;j++)
			for(int k=0;k<(i>j?j:i);k++)
				vote[i][j][k]=0;

	for(int i=0;i<img.rows;i++)
	{
		for(int j=0;j<img.cols;j++)
		{
			int c=0;
			if(img.at<uchar>(i,j)==0)
				continue;
			for(int p=i-1;p<i+2;p++)
				for(int q=j-1;q<j+2;q++)
				{
					if(isvalid(img,p,q)==0)
						continue;
					if(img.at<uchar>(p,q)==0)
						c++;
				}
			if(c<4)
				continue;
			for(int a=0;a<img.rows;a++)
			{
				for(int b=0;b<img.cols;b++)
				{
					int r=sqrt( pow(i-a,2) + pow(j-b,2) );
					if(r>a || r>b | r>(img.rows-a) || r>(img.cols-b))
						continue;
					vote[a][b][r]++;
				}
			}
		}
	}

	int vote_max=0,a=0,b=0,r=0;
	Mat img2(img.rows,img.cols,CV_8UC3,Scalar(0,0,0));
	for(int i=0;i<img.rows;i++)
		for(int j=0;j<img.cols;j++)
			for(int k=0;k<(i>j?j:i);k++)
				if(vote_max<vote[i][j][k])
				{
					if(k>4)
						continue;
					//cout<<vote_max<<endl;
					vote_max=vote[i][j][k];
					a=i;
					b=j;
					r=k;
				}
				// if(vote[i][j][k]>25)
				// {
				// 	if(k>20)
				//		continue;
					Point centre(b,a);
					circle(img2,centre,r,Scalar(0,0,255),1,8,0);
					cout<<a<<' '<<b<<' '<<r<<endl;
				// }
	
			
	return img2;
}