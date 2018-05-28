#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat drawing;
	for(int i=0;i<9;i++)
	{

	drawing =Mat::ones(Size(128,128),CV_8UC1)*(25*i); //8UC3 : allows us to have three layers per pixel

	
	//imshow("image",drawing);
	imwrite("blank"+to_string(i+1)+".png",drawing);
	}
	waitKey(0);
}