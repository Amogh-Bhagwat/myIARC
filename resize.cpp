#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <string>
using namespace std;
using namespace cv;

int main()
{
	for (int i=1;i<5;i++)
	{
		Mat dst; 
		Mat src = imread("ch"+to_string(i)+".jpg");
		resize(src,dst,Size(300,300));
		imwrite("char/ch_000_"+to_string(i)+".jpg",dst);
	}
}