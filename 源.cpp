#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include<fstream>
#include<opencv2/core.hpp>
#include<opencv2/imgproc/types_c.h>
#include "Դ.h"


using namespace std;
using namespace cv;//�����ռ�cv

//����Ϊ�Ҷ�ͼ��
void grayShow(Mat& image) {

	Mat gray;
	cvtColor(image, gray, COLOR_RGB2GRAY);
	namedWindow("gary");
	imshow("gary", gray);
	waitKey(0);

}

//�ȱ�����С
void zoomShow(Mat& image) {

	int image_size = 200;
	Mat image_mini;
	resize(image, image_mini, Size(image_size/3, image_size/3));
	imshow("mini", image_mini);
	waitKey(0);
	return ;

}

//ͼ��ü�
void selectShow(Mat& image) {

	//	cv::Rect m_select;
	Rect m_select;
	image = imread("G:/2.jpg");
	imshow("ԭͼ", image);
	m_select = Rect(0, 0, 111, 217);
	Mat select = image(m_select);
	imshow("�ü�", select);
	waitKey(0);
	return;

}

//��ֵģ������
void blurShow(Mat& image) {

	Mat dstblur;
	namedWindow("imageblur");
	blur(image, dstblur, Size(15, 15), Point(-1, -1));
	imshow("imageblur", dstblur);
	waitKey(0);
	return ;

}

//��Ե��ȡ
void cannyShow(Mat& image) {

	Mat cannyImage;
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_RGB2GRAY);//���лҶȴ���
	Canny(grayImage, cannyImage, 128, 255, 3);//canny��ȡ��Ե����
	vector<vector<Point>> contours;  //contoursΪ������������ݼ���
	vector<Vec4i> hierarchy;

	findContours(cannyImage, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));

	/*findContours(1.��Ե���Ӵ������ͼ��,
				   2.����Ϊ��vector<vector> contours����
						��һ��������������һ��˫��������������ÿ��Ԫ�ر�����һ����������Point�㹹�ɵĵ�ļ��ϵ�������
						ÿһ��Point�㼯����һ�������� �ж�������������contours���ж���Ԫ��.
				   3.hierarchy��Ԫ�غ���������contours�ڵ�Ԫ����һһ��Ӧ��,�ֱ��ʾ�� i�������ĺ�һ��������ǰһ������������������Ƕ�������������
				   4.���������ļ���ģʽ,CV_RETR_LIST ������е�����,
				   5.���������Ľ��Ʒ���CV_CHAIN_APPROX_NONE��������߽������������������㵽contours������,
				   6.ƫ���������е�������Ϣ�����ԭʼͼ���Ӧ���ƫ����Point(0,0));
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
	Mat Contours = Mat::zeros(image.size(), CV_8UC1);  //����

	for (int i = 0; i < contours.size(); i++) {
		//contours[i]������ǵ�i��������contours[i].size()������ǵ�i�����������е����ص���
		for (int j = 0; j < contours[i].size(); j++) {

			Point P = Point(contours[i][j].x, contours[i][j].y);//���Ƴ�contours�������������ص�
			Contours.at<uchar>(P) = 255;
		}

		drawContours(imageContours, contours, i, Scalar(255), 1, 8, hierarchy);//��������

	}

	imshow("anny", Contours);//����contours�ڱ�������������㼯
	waitKey(0);
	return;
}

//�򵥵���ֵ�ָ�
void divShow(Mat& image) {

	Mat divImage;
	threshold(image, divImage, 127, 255, THRESH_BINARY_INV);
	imshow("div", divImage);
	waitKey(0);
	return ;
}

//�Ҷ�ֱ��ͼ
int histShow(Mat& gray) {
	
	MatND hist;
	int dims = 1;
	float hranges[] = { 0,255 };
	const float* ranges[] = { hranges };
	int size = 256;
	int channels = 0;
	calcHist(&gray, 1, &channels, Mat(), hist, dims, &size, ranges);//����ͼ��ֱ��ͼ
	int scale = 1;
	Mat imageShow(size * scale, size, CV_8U, Scalar(0));
	double minVal = 0;//��ȡ��Сֵ
	double maxVal = 0;//��ȡ���ֵ
	minMaxLoc(hist, &minVal, &maxVal, 0, 0);
	//��ʾֱ��ͼ��ͼ��
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

	Mat image = imread("G:/2.jpg",1);//��ȡ�ռ�
	
	grayShow(image);//��ʾ�Ҷ�ͼƬ
	selectShow(image);//ͼ��ü�
	blurShow(image);//��ֵģ������
	zoomShow(image);//�ȱ�����С	
	divShow(image);//�򵥵���ֵ�ָ�
	histShow(image);//�Ҷ�ֱ��ͼ
	cannyShow(image);//��Ե��ȡ
	
	namedWindow("img");//�򿪴��壬����img
	imshow("img", image);//��ʾͼƬ
	waitKey(0);//ʵ�ֽ���ˢ��
	return 0;

}