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

	//打开摄像头的参数
    VideoCapture cap(1);
	if(!cap.isOpened())
	{
	        cout<<"can't open camera";
		return -1;
	}

       cap.set(WIDTH_ID,WIDTH);
       cap.set(HEIGHT_ID,HEIGHT);
       cap.set(FPS_ID,10);

	//Mat left, right;
   // Mat img;
	int goodFrameCount = 0;
	namedWindow("ImageL");
	//namedWindow("ImageR");
	//namedWindow("hebing");
	cout << "按Q退出 ..." << endl;
	while (goodFrameCount < frameNumber)
	{
		cap >>frame;
		i++;
		//ROS_WARN("%d---%d",frame.cols, frame.rows);
		cout<<"curent frame times:"<<i<<endl;

		if(!frame.empty())
		{
			getImages(rgbImageL,rgbImageR);
			imageleft=rgbImageL.clone();
			imageright=rgbImageR.clone();
			cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);
			cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);
			//imshow("image_lfet",rgbImageL);
			//imshow("image_right",rgbImageR);
		
			bool isFindL, isFindR;
       		isFindL = findChessboardCorners(rgbImageL, boardSize, cornerL);
			isFindR = findChessboardCorners(rgbImageR, boardSize, cornerR);
        	if (isFindL == true && isFindR == true)	 //如果两幅图像都找到了所有的角点 则说明这两幅图像是可行的
				{
			/*
			Size(5,5) 搜索窗口的一半大小
			Size(-1,-1) 死区的一半尺寸
			TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1)迭代终止条件
			*/
				/*
				cornerSubPix(grayImageL, cornerL, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
				drawChessboardCorners(rgbImageL, boardSize, cornerL, isFindL);
				imshow("ImageL", rgbImageL);
				//imagePointL.push_back(cornerL);


				cornerSubPix(grayImageR, cornerR, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
				drawChessboardCorners(rgbImageR, boardSize, cornerR, isFindR);
				imshow("ImageR", rgbImageR);
				*/
				//Mat result(rgbImageL.rows,rgbImageL.cols+rgbImageR+1,rgbImageL.type());
				//rgbImageL.colRange(0,rgbImageL.cols).copyTo(result.colRange(0,rgbImageL.cols));
				//rgbImageR.colRange(0,rgbImageR.cols).copyTo(result.colRange(rgbImageL.cols+1,result.cols));
				//imshow("hebing",result);
				//imagePointR.push_back(cornerR);
				/*Mat result=mergeCols(rgbImageL,rgbImageR);
				imshow("hebing",result);
				*/

			/*
			本来应该判断这两幅图像是不是好的，如果可以匹配的话才可以用来标定
			但是在这个例程当中，用的图像是系统自带的图像，都是可以匹配成功的。
			所以这里就没有判断
			*/
			//string filename = "res\\image\\calibration";
			//filename += goodFrameCount + ".jpg";
			//cvSaveImage(filename.c_str(), &IplImage(rgbImage));		//把合格的图片保存起来

				goodFrameCount++;
				cout << "The image is good =" << goodFrameCount<< endl;

		/*读取左边的图像*/
				sprintf(filename, "data/left-%04d.png", goodFrameCount + 40);
				imshow("ImageL",imageleft);
				imwrite(filename,imageleft);

		/*读取右边的图像*/
				sprintf(filename, "data/right-%04d.png", goodFrameCount + 40);
				imwrite(filename,imageright);
				//sprintf(filename, "data/hebing/hebing-%04d.png", goodFrameCount + 1);
				//imwrite(filename,result);
				waitKey(1500);
				cap>>frame;
				cap>>frame;
				cap>>frame;
				cap>>frame;
				cap>>frame;
				cap>>frame;
				cap>>frame;
				cap>>frame;
				}
			//else
				//continue;

		else
		{
			cout << "The image is bad please try again" << endl;
		}
		}

		if (waitKey(10) == 'q')
		{
			break;
		}

		
	}

	return 0;
}
