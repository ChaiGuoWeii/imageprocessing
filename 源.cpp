#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include<fstream>
#include<opencv2/core.hpp>
#include<opencv2/imgproc/types_c.h>
#include "源.h"


using namespace std;
using namespace cv;//命名空间cv

//处理为灰度图像
void grayShow(Mat& image) {

	Mat gray;
	cvtColor(image, gray, COLOR_RGB2GRAY);
	namedWindow("gary");
	imshow("gary", gray);
	waitKey(0);

}

//等比例缩小
void zoomShow(Mat& image) {

	int image_size = 200;
	Mat image_mini;
	resize(image, image_mini, Size(image_size/3, image_size/3));
	imshow("mini", image_mini);
	waitKey(0);
	return ;

}

//图像裁剪
void selectShow(Mat& image) {

	//	cv::Rect m_select;
	Rect m_select;
	image = imread("G:/2.jpg");
	imshow("原图", image);
	m_select = Rect(0, 0, 111, 217);
	Mat select = image(m_select);
	imshow("裁剪", select);
	waitKey(0);
	return;

}

//均值模糊处理
void blurShow(Mat& image) {

	Mat dstblur;
	namedWindow("imageblur");
	blur(image, dstblur, Size(15, 15), Point(-1, -1));
	imshow("imageblur", dstblur);
	waitKey(0);
	return ;

}

//边缘提取
void cannyShow(Mat& image) {

	Mat cannyImage;
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_RGB2GRAY);//进行灰度处理
	Canny(grayImage, cannyImage, 128, 255, 3);//canny提取边缘算子
	vector<vector<Point>> contours;  //contours为输出的轮廓数据集合
	vector<Vec4i> hierarchy;

	findContours(cannyImage, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));

	/*findContours(1.边缘算子处理过的图像,
				   2.定义为“vector<vector> contours”，
						是一个向量，并且是一个双重向量，向量内每个元素保存了一组由连续的Point点构成的点的集合的向量，
						每一组Point点集就是一个轮廓。 有多少轮廓，向量contours就有多少元素.
				   3.hierarchy的元素和轮廓向量contours内的元素是一一对应的,分别表示第 i个轮廓的后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号
				   4.定义轮廓的检索模式,CV_RETR_LIST 检测所有的轮廓,
				   5.定义轮廓的近似方法CV_CHAIN_APPROX_NONE保存物体边界上所有连续的轮廓点到contours向量内,
				   6.偏移量，所有的轮廓信息相对于原始图像对应点的偏移量Point(0,0));
	*/

	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {

			cout << contours[i][j].x << "," << contours[i][j].y << ",";
			ofstream f;
			f.open("640.txt", ios::out | ios::app);
			f << contours[i][j].x << ", " << contours[i][j].y << ", ";

		}
	}

	Mat imageContours = Mat::zeros(image.size(), CV_8UC1);
	Mat Contours = Mat::zeros(image.size(), CV_8UC1);  //绘制

	for (int i = 0; i < contours.size(); i++) {
		//contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
		for (int j = 0; j < contours[i].size(); j++) {

			Point P = Point(contours[i][j].x, contours[i][j].y);//绘制出contours向量内所有像素点
			Contours.at<uchar>(P) = 255;
		}

		drawContours(imageContours, contours, i, Scalar(255), 1, 8, hierarchy);//绘制轮廓

	}

	imshow("anny", Contours);//向量contours内保存的所有轮廓点集
	waitKey(0);
	return;
}

//简单的阈值分割
void divShow(Mat& image) {

	Mat divImage;
	threshold(image, divImage, 127, 255, THRESH_BINARY_INV);
	imshow("div", divImage);
	waitKey(0);
	return ;
}

//灰度直方图
int histShow(Mat& gray) {
	
	MatND hist;
	int dims = 1;
	float hranges[] = { 0,255 };
	const float* ranges[] = { hranges };
	int size = 256;
	int channels = 0;
	calcHist(&gray, 1, &channels, Mat(), hist, dims, &size, ranges);//计算图像直方图
	int scale = 1;
	Mat imageShow(size * scale, size, CV_8U, Scalar(0));
	double minVal = 0;//获取最小值
	double maxVal = 0;//获取最大值
	minMaxLoc(hist, &minVal, &maxVal, 0, 0);
	//显示直方图的图像
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++){

		float value = hist.at<float>(i);
		int realValue = saturate_cast<int>(value * hpt / maxVal);
		rectangle(imageShow, Point(i * scale, size - 1), Point((i + 1) * scale - 1, size - realValue), Scalar(255));

	}
	namedWindow("hist");
	imshow("hist", imageShow);
	waitKey(0);
	return 0;



}

int main(int argc, char** argv) {

	Mat image = imread("G:/2.jpg",1);//读取空间
	
	grayShow(image);//显示灰度图片
	selectShow(image);//图像裁剪
	blurShow(image);//均值模糊处理
	zoomShow(image);//等比例缩小	
	divShow(image);//简单的阈值分割
	histShow(image);//灰度直方图
	cannyShow(image);//边缘提取
	
	namedWindow("img");//打开窗体，名字img
	imshow("img", image);//显示图片
	waitKey(0);//实现界面刷新
	return 0;

}