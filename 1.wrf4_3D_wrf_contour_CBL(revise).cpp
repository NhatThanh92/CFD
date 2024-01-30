#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

int main() 
{
int hr,min;
for(hr=0; hr<24; hr++){//%%% hr hr hr
for(min=0;min<6;min++){//%%% min min min
fstream file1,file2;
char writefile1[50],writefile2[50];
sprintf(writefile1, "wrf_3D_cbl_1102_%dhr%dmin-2.txt",hr,min*10);
sprintf(writefile2, "wrf_3D_cbl_1102_%dhr%dmin-3.txt", hr,min*10);
file1.open(writefile1, ios::out | ios::trunc);

file1<<"VARIABLES= " << "x," << "y," << "z,"<< "U," << "V," <<"W"<<"\n"<<
       "ZONE I="<< 11 << ",J="<< 11 << ",K="<< 22 << ",DATAPACKING=point"<<"\n";  

int num,n,var,end;

for(n=1;n<5;n++)
{
if(n==1){num=40,end=70,var=10;}
if(n==2){num=80,end=260,var=20;}
if(n==3){num=300,end=550,var=50;}
if(n==4){num=600,end=1100,var=100;}
if(n==5){num=1100,end=1600,var=100;}
for(num; num<end; num=num+var) 
{
//-UUU
//ifstream readfile1;
char infile1[20];
sprintf (infile1, "U_%dm_2017-06-01_00_00_00.txt", num,hr*6+min+1);
ifstream readfile1(infile1);
const int xsize = 120; //  line  file U,V,W from WRF
const int ysize = 120; // lay 120 colum trong file U,VW tu WRF 
string (*arr)[xsize] = new string[ysize][xsize];

string line;
int y = 0;
while(getline(readfile1,line)) 
{
istringstream ss(line);
if(y>=1)
{ 
string word;
int x = 0;
while(ss >> word) 
{
   arr[y-1][x] = word;
   ++x;
   }}
++y;
}
//--------------------------------------------------------------------UUU
//------------------------------------------VVV
char infile2[20];
sprintf (infile2, "V_%dm_2017-06-01_00_00_00.txt", num,hr*6+min+1);//######################################################################
ifstream readfile2(infile2);
string (*arr2)[xsize] = new string[ysize][xsize];
int j = 0;
while(getline(readfile2,line)) 
{
istringstream ss(line);
if(j>=1)
{
string word;
int i = 0;
while(ss >> word) 
{
   arr2[j-1][i] = word;
   ++i;
   }}
++j;
}
//--------------------------------------------------------------------VVV
//------------------------------------------WWW
char infile3[20];
sprintf (infile3, "W_%dm_2017-06-01_00_00_00.txt", num,hr*6+min+1);
ifstream readfile3(infile3);
string (*arr3)[xsize] = new string[ysize][xsize];
int n = 0;
while(getline(readfile3,line)) 
{
istringstream ss(line);
if(n>=1)
{
string word;
int m = 0;
while(ss >> word) 
{
   arr3[n-1][m] = word;
   ++m;
   }}
++n;
}

//--------------------------------------------------------------------WWW

// cai dat do dai cua x,y (example (x,y)=(0,0)->(9999.99,9999.99)

 for(int x = 10; x <21 ; ++x) 
 {
for(int y = 21; y < 32; ++y) 
{
      double x1=(x-10)* 359.99; 
      double y1=(y-21)* 359.99;
      
        
  	file1<<setprecision(8)<< x1 << "\t" 
      <<setprecision(8)<< y1 << "\t"
      <<num<<"\t" 
      <<setprecision(8)<< arr[y][x] <<  "\t" 
      <<setprecision(8)<<arr2[y][x]<<"\t" 
      <<setprecision(8)<<arr3[y][x]<<"\n" ;
      
      	file2<<setprecision(8)<< x1 << "\t" 
      <<setprecision(8)<< y1 << "\t"
      <<num<<"\t" 
      <<setprecision(8)<< arr[y][x] <<  "\t" 
      <<setprecision(8)<<arr2[y][x]<<"\t" 
      <<setprecision(8)<<arr3[y][x]<<"\n" ;    
  }
}

delete arr;
delete arr2;
delete arr3;
readfile1.close();
readfile2.close();
readfile3.close();
}
}

}//%%% min min min
}//%%%hrhrhr
  return 0;
}
