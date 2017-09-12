#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include "stdio.h"
#include "cv.h"
#include <cv.hpp>
using namespace std;
using namespace cv;

#define WIDTH_ID 3
#define HEIGHT_ID 4
#define FPS_ID 5

//#define WIDTH 2560
#define WIDTH 1344
//#define HEIGHT 720
#define HEIGHT 376
const int imageWidth = 672;								//摄像头的分辨率
const int imageHeight = 376;
const int boardWidth = 11;								//横向的角点数目
const int boardHeight = 8;								//纵向的角点数据
const int boardCorner = boardWidth * boardHeight;		//总的角点数据
const int frameNumber = 40;								//相机标定时需要采用的图像帧数
const int squareSize = 20;								//标定板黑白格子的大小 单位mm
const Size boardSize = Size(boardWidth, boardHeight);	//
Size imageSize = Size(imageWidth, imageHeight);
Mat R, T, E, F;											//R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵
vector<Mat> rvecs;									    //旋转向量
vector<Mat> tvecs;										//平移向量
vector<vector<Point2f>> imagePointL;				    //左边摄像机所有照片角点的坐标集合
vector<vector<Point2f>> imagePointR;					//右边摄像机所有照片角点的坐标集合
vector<vector<Point3f>> objRealPoint;					//各副图像的角点的实际物理坐标集合


vector<Point2f> cornerL;								//左边摄像机某一照片角点坐标集合
vector<Point2f> cornerR;								//右边摄像机某一照片角点坐标集合

Mat rgbImageL,grayImageL;
Mat rgbImageR,grayImageR;

Mat Rl, Rr, Pl, Pr, Q;									//校正旋转矩阵R，投影矩阵P 重投影矩阵Q (下面有具体的含义解释）	
Mat mapLx, mapLy, mapRx, mapRy;							//映射表
Rect validROIL, validROIR;	
Mat imageleft;
Mat imageright;	

Mat frame;
char filename[100];
int i=0;
Mat mergeCols(Mat A, Mat B)
 {
     // cv::CV_ASSERT(A.cols == B.cols&&A.type() == B.type());
      int totalCols = A.cols + B.cols;
      Mat mergedDescriptors(A.rows,totalCols, A.type());
      Mat submat = mergedDescriptors.colRange(0, A.cols);
      A.copyTo(submat);
      submat = mergedDescriptors.colRange(A.cols, totalCols);
      B.copyTo(submat);
     return mergedDescriptors;
 }

bool getImages(Mat& left_image,Mat& right_image) 
{
    Rect left_rect(0, 0, WIDTH/2, HEIGHT);
    Rect right_rect(WIDTH/2, 0, WIDTH/2, HEIGHT);
	left_image = frame(left_rect);
	right_image = frame(right_rect);	
}

int main(int argc, char **argv)
{
      char filename[100];
      int goodFrameCount=0;
      for(goodFrameCount=0;goodFrameCount<=79;goodFrameCount++)
{
	sprintf(filename, "data/left-%04d.png", goodFrameCount + 1);
        rgbImageL = imread(filename, CV_LOAD_IMAGE_COLOR);
	sprintf(filename, "data/right-%04d.png", goodFrameCount + 1);
        rgbImageR = imread(filename, CV_LOAD_IMAGE_COLOR);
        Mat result=mergeCols(rgbImageL,rgbImageR);
        sprintf(filename, "data/%d.png", goodFrameCount + 1);
	imwrite(filename,result);

}			

	return 0;
}

