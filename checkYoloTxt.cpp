#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char const *argv[])
{
	ifstream in;
	//ofstream out;
	for (int i = 1; i <= 1202; ++i)
	{
		if(i==8)continue;
		string fin = "yolotxt/dt-"+to_string(i)+".txt";
		//string fout = "modified/dt-"+to_string(i)+".txt";
		in.open(fin);
		//out.open(fout);

		// char str[100];
		// while(in)
		// {
		// 	in.getline(str,100);
		// 	if(in)
		// 		cout<<str<<" "<<sizeof(str)<<endl;
		// }

		string str;
		double cx,cy,wx,wy;
		while(getline(in,str))
		{
			//cout<<str<<" "<<str.size()<<" "<<str[str.size()-1]<<endl;
			if(str.size()>1)
			{
			string word[5];
			double sx,sy,ex,ey;
			int j=0,cl;
			stringstream iss(str);
			while(iss>>word[j])
				++j;
			//cout<<word[4]<<endl;
			cl = atoi(word[0].c_str());
			cx = stod(word[1]);
			cy = stod(word[2]);
			wx = stod(word[3]);
			wy = stod(word[4]);

			// //double cx,cy,wx,wy;
			// cx = ((sx+ex+0.0)/2.0)/900.0;
			// cy = ((sy+ey+0.0)/2.0)/560.0;
			// wx = ((ex-sx+0.0)/900.0);
			// wy = ((ey-sy+0.0)/560.0);
			
			// if(word[4]=="A")
			// 	cl = 0;
			// if(word[4]=="C")
			// 	cl = 1;
			// if(word[4]=="I")
			// 	cl = 2;
			// if(word[4]=="R")
			// 	cl = 3;
			//out.setf( ios::fixed );
			//out<<cl<<" "<<cx<<" "<<cy<<" "<<wx-0.0001<<" "<<wy-0.0001<<setprecision(4)<<endl;
			}		

		}
	if(((cx+wx/2.0)>=1)||((cx-wx/2.0)<=0)||((cy+wy/2.0)>=1)||((cy-wy/2.0)<=0)||(wx>=1)||(wx<=0)||(wy<=0)||(wy>=1))
		cout<<fin<<endl;
	in.close();
	//out.close();
	}
	
	return 0;
}
