#include<iostream>
#include<stdio.h>
#include<math.h>
#include<limits.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<queue>

#define PATH "2.png"

using namespace std;
using namespace cv;

class node
{
public:
  int cost,x,y,px,py;
};
int rx,ry,gx,gy;
bool operator<(node a,node b) {return a.cost > b.cost ? true : false;}

int isvalid(Mat img,int i,int j)
{
  if(i<0 || j<0 || i>=img.rows || j>=img.cols)
    return 0;
  else return 1;
}
Mat raw_image()
{
  Mat img=imread(PATH,CV_LOAD_IMAGE_COLOR);
  Mat img2(img.rows,img.cols,CV_8UC3,Scalar(0,0,0));
  for(int i=0;i<img.rows;i++)
    {
      for(int j=0;j<img.cols;j++)
	{
	  if(img.at<Vec3b>(i,j)[0]<50 && img.at<Vec3b>(i,j)[1]<50 && img.at<Vec3b>(i,j)[2]>200)
	    img2.at<Vec3b>(i,j)[2]=255;
	  if(img.at<Vec3b>(i,j)[0]<50 && img.at<Vec3b>(i,j)[1]>200 && img.at<Vec3b>(i,j)[2]<50)
	    img2.at<Vec3b>(i,j)[1]=255;
	  if(img.at<Vec3b>(i,j)[0]>200 && img.at<Vec3b>(i,j)[1]>200 && img.at<Vec3b>(i,j)[2]>200)
	    {
                img2.at<Vec3b>(i,j)[2]=255;
	  	img2.at<Vec3b>(i,j)[1]=255;
	  	img2.at<Vec3b>(i,j)[0]=255;
	    }
	}
    }
  return img2;
}
int* Centre(Mat img,int *c)
{
  int c1,c2;
  c[0]=c[1]=c[2]=c[3]=c1=c2=0;
  for(int i=0;i<img.rows;i++)
    {
      for(int j=0;j<img.cols;j++)
	{
	  if(img.at<Vec3b>(i,j)[0]==0 && img.at<Vec3b>(i,j)[1]==0 && img.at<Vec3b>(i,j)[2]==255)
	    {
	      c1++;
	      c[0]+=i;
	      c[1]+=j;
	    }
	  if(img.at<Vec3b>(i,j)[0]==0 && img.at<Vec3b>(i,j)[1]==255 && img.at<Vec3b>(i,j)[2]==0)
	    {
	      c2++;
	      c[2]+=i;
	      c[3]+=j;
	    }
	}
    }
  c[0]/=c1;
  c[1]/=c1;
  c[2]/=c2;
  c[3]/=c2;
  return c;
}
int isobstacle(Mat img,int i,int j)
{
  if(img.at<Vec3b>(i,j)[0]==255 && img.at<Vec3b>(i,j)[1]==255 && img.at<Vec3b>(i,j)[2]==255)
    return 1;
  else return 0;
}
void path_planning(Mat);
int main()
{
  Mat img=raw_image();
  int c[4];
  Centre(img,c);  
  rx=c[0],ry=c[1],gx=c[2],gy=c[3];
  path_planning(img);
  waitKey(0);
  return 0;
}

void path_planning(Mat img)
{
  int c=0;
  node **ar;
  ar=new node*[img.rows];
  for(int i=0;i<img.rows;i++)
      ar[i]=new node[img.cols];
  for(int i=0;i<img.rows;i++)
    for(int j=0;j<img.cols;j++)
      {
	ar[i][j].cost=0;
	ar[i][j].x=i;
	ar[i][j].y=j;
      }

  priority_queue<node> s;
  Mat mark(img.rows,img.cols,CV_8UC1,Scalar(0));
  node n,tmp;
  n.cost=0;
  n.x=rx;
  n.y=ry;
  n.px=rx;
  n.py=ry;
  s.push(n);
  mark.at<uchar>(rx,ry)=1;
  namedWindow("img",WINDOW_NORMAL);
  while(!s.empty())
    {
      n=s.top();
      s.pop();
      for(int i=n.x-1;i<=n.x+1;i++)
	{
	  for(int j=n.y-1;j<=n.y+1;j++)
	    {
	      if(!isvalid(img,i,j))
		continue;
	      if(isobstacle(img,i,j))
		continue;
	      if(i==n.x && j==n.y)
		continue;
	      if(mark.at<uchar>(i,j)==0)
	     {
	       if(i==gx && j==gy)
		 {
		   ar[i][j].cost=ar[n.x][n.y].cost+10;
		   ar[i][j].px=n.x;
		   ar[i][j].py=n.y;
		   goto jmp;
		 }
	       ar[i][j].cost=(i==n.x || j==n.y)?ar[n.x][n.y].cost+10:ar[n.x][n.y].cost+14;
	      ar[i][j].px=n.x;
	      ar[i][j].py=n.y;
	      mark.at<uchar>(i,j)=1;
	      s.push(ar[i][j]);
	      // cout<<ar[i][j].px<<' '<<ar[i][j].py<<endl;
	      // waitKey(1);
	     }
	     else if(mark.at<uchar>(i,j)==1)
		{
		    if(i==n.x || j==n.y)
		    {
		       if(ar[i][j].cost>n.cost+10)
		       {
		          ar[i][j].cost=n.cost+10;
		          ar[i][j].px=n.x;
		          ar[i][j].py=n.y;
		       }
		    }
		    else if(ar[i][j].cost>n.cost+14)
		      {
		      ar[i][j].cost=n.cost+14;
		      ar[i][j].px=n.x;
		      ar[i][j].py=n.y;
		    }

		}
	     //  img.at<Vec3b>(i,j)[0]=150;
	    //   imshow("img",img);
	    //   waitKey(1);
	    }
	}
    }
 jmp:
  int i=gx,j=gy,k,l;
  while(i!=rx || j!=ry)
    {c++;
      k=i;
      l=j;
      // cout<<i<<' '<<j<<endl;
      // imshow("img",img);
      // waitKey(0);
      img.at<Vec3b>(i,j)[2]=255;
      img.at<Vec3b>(i,j)[1]=255;
      img.at<Vec3b>(i,j)[0]=0;
      i=ar[k][l].px;
      j=ar[k][l].py;
    }
   cout<<c;
  imshow("img",img);
  waitKey(0);
  imwrite("ps1-output.png",img);
  return;
}
