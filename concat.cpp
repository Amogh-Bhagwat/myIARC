#include <iostream>
#include <fstream>
#include <ostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <cmath>
#include <string>
using namespace std;
using namespace cv;


int main(int argc, char const *argv[])
{
	Mat t[4],temp[9],temp1[9],temp3[9],bl[5];
	Mat final,ftemp,final1,final2,final3;
	for(int i=951;i<=1000;i++)
	{		
		t[1] = imread("letterA/fA-"+to_string(i)+".png");
		t[3] = imread("letterC/fC-"+to_string(i)+".png");
		t[0] = imread("letterI/fI-"+to_string(i)+".png");
		t[2] = imread("letterR/fR-"+to_string(i)+".png");
		for(int j=0;j<4;j++)
			resize(t[j],t[j],Size(120,120));
		for(int ii=0;ii<9;ii++)
		{
			bl[ii]=imread("blank"+to_string(ii+1)+".png");
			resize(bl[ii],bl[ii],Size(120,120));
		}
		temp1[0] = bl[0].clone();
		for(int ii=0;ii<8;ii++)
		{
			hconcat(temp1[ii],bl[ii+1],temp1[ii+1]);
		}
		final1=temp1[8];

		temp3[8] = bl[8].clone();
		for(int ii=8;ii>0;ii--)
		{
			hconcat(temp3[ii],bl[ii-1],temp3[ii-1]);
		}
		final3=temp3[0];
				
		temp[0] = t[0].clone();
		hconcat (temp[0] , bl[0] ,temp[1]);
		hconcat (temp[1] , t[1] ,temp[2]);
		hconcat (temp[2] , bl[1] ,temp[3]);
		hconcat (temp[3] , bl[2] ,temp[4]);
		hconcat (temp[4] , t[2] ,temp[5]);
		hconcat (temp[5] , bl[3] ,temp[6]);
		hconcat (temp[6] , bl[1] ,temp[7]);
		hconcat (temp[7] , bl[4] ,temp[8]);
		
		final2 = temp[8];

		vconcat(final3,final2,ftemp);
		vconcat(ftemp,final1,final);

		imwrite("fin1F/gr0-"+to_string(i)+".png",final);

		// ofstream myfile;
		// string fs = "fin1F/gr0-"+to_string(i)+".txt";
		// myfile.open (fs);
		// myfile << "0 0.16666 0.50 0.110 0.330"<<endl;
		// myfile << "1 0.38888 0.50 0.110 0.330"<<endl;
		// myfile << "2 0.61111 0.50 0.110 0.330"<<endl;
		// myfile << "3 0.83333 0.50 0.110 0.330"<<endl;
		// myfile.close();

		
		//imshow("final",final);
	}	
	return 0;
}
