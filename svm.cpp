#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
using namespace cv;
using namespace cv::ml;
int main(int argc, char **argv)
{
    //Input
    Mat img;
    img = imread(argv[1], IMREAD_COLOR);
    namedWindow("Original", WINDOW_AUTOSIZE );
    imshow("Original", img);
    waitKey(0);
    // Data for visual representation
    int width = 512, height = 512;
    Size size(width,height);
    Mat image;
    resize(img,image,size);
    namedWindow("New", WINDOW_AUTOSIZE );
    imshow("New", image);
    waitKey(0);
    //Mat image = Mat::zeros(height, width, CV_8UC3);
    // Set up training data
    int labels[7] = {1, 1, 1, 1, -1, -1, -1};
    float trainingData[7][2] = { {100, 146}, {210, 145}, {370, 146}, {467, 143}, {282,19}, {322,124}, {321,332} };
    Mat trainingDataMat(7, 2, CV_32FC1, trainingData);
    Mat labelsMat(7, 1, CV_32SC1, labels);
    // Train the SVM
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
    // Show the decision regions given by the SVM
    Vec3b green(0,255,0), blue (255,0,0);
    for (int i = 0; i < image.rows; ++i)
        for (int j = 0; j < image.cols; ++j)
        {
            Mat sampleMat = (Mat_<float>(1,2) << j,i);
            float response = svm->predict(sampleMat);
            if (response == 1)
                image.at<Vec3b>(i,j)  = green;
            else if (response == -1) 
                image.at<Vec3b>(i,j)  = blue;
        }
    // Show the training data
    int thickness = -1;
    int lineType = 8;/*
    circle( image, Point(501,  10), 5, Scalar(  0,   0,   0), thickness, lineType );
    circle( image, Point(255,  10), 5, Scalar(255, 255, 255), thickness, lineType );
    circle( image, Point(501, 255), 5, Scalar(255, 255, 255), thickness, lineType );
    circle( image, Point( 10, 501), 5, Scalar(255, 255, 255), thickness, lineType );*/
    // Show support vectors
    thickness = 2;
    lineType  = 8;
    Mat sv = svm->getUncompressedSupportVectors();
    for (int i = 0; i < sv.rows; ++i)
    {
        const float* v = sv.ptr<float>(i);
        circle( image,  Point( (int) v[0], (int) v[1]),   6,  Scalar(128, 128, 128), thickness, lineType);
    }
    //imwrite("result.png", image);        // save the image
    imshow("SVM Simple Example", image); // show it to the user
    waitKey(0);
}