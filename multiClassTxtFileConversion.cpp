#include <fstream>
#include <string>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char const *argv[])
{
	ifstream in;
	ofstream out;
	for (int i = 1; i <= 1; ++i)
	{
		string fin = "dt-"+to_string(i)+".txt";
		string fout = "dtt-"+to_string(i)+".txt";
		in.open(fin);
		out.open(fout);

		// char str[100];
		// while(in)
		// {
		// 	in.getline(str,100);
		// 	if(in)
		// 		cout<<str<<" "<<sizeof(str)<<endl;
		// }

		string str;
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
			sx = stod(word[0]);
			sy = stod(word[1]);
			ex = stod(word[2]);
			ey = stod(word[3]);

			double cx,cy,wx,wy;
			cx = ((sx+ex+0.0)/2.0)/900.0;
			cy = ((sy+ey+0.0)/2.0)/560.0;
			wx = ((ex-sx+0.0)/900.0);
			wy = ((ey-sy+0.0)/560.0);


			if(word[4]=="A")
				cl = 0;
			if(word[4]=="C")
				cl = 1;
			if(word[4]=="I")
				cl = 2;
			if(word[4]=="R")
				cl = 3;

			out<<cl<<" "<<cx<<" "<<cy<<" "<<wx<<" "<<wy<<endl;
			}

		}

	}
	in.close();
	out.close();
	return 0;
}