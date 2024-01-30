#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iomanip>
using namespace std;

int main() 
{

int a=11,b=11,c=27;

double a1,a2,b1,b2,c1,c2;

double da=379.99,db=379.99,u1,u2;
int n;

int hr,min,hhr,mmin;
for(hr=0; hr<24; hr++)
{//%%% hr hr hr
for(min=0;min<6;min++)
{//%%% min min min
fstream file1;
char writefile1[50];
sprintf(writefile1, "3D_ch_0101_%dhr_%d-%d_PBL_udf.c", hr,min*10,min*10+10);
file1.open(writefile1, ios::out | ios::trunc);

char readfile1[50],readfile2[50];
sprintf (readfile1, "wrf_3D_cbl_1102_%dhr%dmin-2.txt", hr,min*10); 
if(min==5){ hhr=hhr+1; mmin=0;}
else{hhr=hr;mmin=min*10+10;}
sprintf (readfile2, "wrf_3D_cbl_1102_%dhr%dmin-2.txt", hhr,mmin); 

ifstream inFile1(readfile1),inFile2(readfile2);

const int xsize = 6;//
const int ysize = 9702;//
double (*arr)[xsize] = new  double[ysize][xsize];

string line;
int y = 0;
while(getline(inFile1,line)) 
{
istringstream ss(line);
 double word;
int x = 0;
while(ss >> word) 
{
   arr[y][x] = word;
   ++x;
   }

++y;
}

double (*arr2)[xsize] = new  double[ysize][xsize];

//string line;
int j = 0;
while(getline(inFile2,line)) 
{
istringstream ss(line);
 double word;
int i = 0;
while(ss >> word) 
{
   arr2[j][i] = word;
   ++i;
   }

++j;
}



file1<<"#include \"udf.h\""<<"\n"<<"\n";
file1
<<"DEFINE_TURBULENT_VISCOSITY(mut,c,t){"<<"\n"
<<"real mu_t,ls,kappa,cs,kd,csd;"<<"\n"
<<"real y[ND_ND]; "<<"\n"
<<"kappa=0.4;/*von karman constant*/"<<"\n"
<<"cs=0.18;/*smagorinsky constant*/"<<"\n"
<<"C_CENTROID(y,c,t);"<<"\n"
<<"csd=cs*pow(C_VOLUME(c,t),1.0/3.0);"<<"\n"
<<"kd=kappa*y[2];"<<"\n"
<<"if(kd<csd){ls=kd;}"<<"\n"
<<"else {ls=csd;}"<<"\n"
<<"mu_t =C_R(c,t)*ls*ls*C_STRAIN_RATE_MAG(c,t); "<<"\n"
<<"return mu_t;}"<<"\n";


/**************************************************UTOP***************************************/
file1<<"DEFINE_PROFILE(u_top1,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";

for(n=0;n<(a-1)*b;n=n+b)
{
for(y=a*b*(c-1)+n; y < (a*b*(c-1)+n+(b-1)); ++y) 
{

    a1= arr[y][0];
    a2= arr[y+b][0];
    b1= arr[y][1];
    b2= arr[y+1][1];
          
  	file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2
      <<"){s1=(("
<<setprecision(6)<<arr[y][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+b][3]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+1][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr[y+b+1][3]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
<<"s2=(("
<<setprecision(6)<<arr2[y][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+b][3]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+1][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr2[y+b+1][3]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"

<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
}  	
      	
}

file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n";
/*********************************************************UTOP*******************************************/


/************************UDOWN*********************************************************************/
file1<<"DEFINE_PROFILE(u_down2,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s2,s1;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";

for(n=0;n<(a-1)*b;n=n+b)
{
for(y=0+n; y < (b-1)+n; ++y) 
{
 
    a1= arr[y][0];
    a2= arr[y+b][0];
    b1= arr[y][1];
    b2= arr[y+1][1];
    
      
  	file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+b][3]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+1][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr[y+b+1][3]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
<<"s2 =(("
<<setprecision(6)<<arr2[y][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+b][3]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+1][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr2[y+b+1][3]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
}      	
}
file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n";
/************************UDOWN*********************************************************************/


/************************UNORTH*********************************************************************/
file1<<"DEFINE_PROFILE(u_north3,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";

for(n=0;n<a*b*(c-1);n=n+a*b)  
{
for(y=b-1+n; y <a*b-1+n ; y=y+b)
{
    a1= arr[y][0];
    a2= arr[y+b][0];
    c1= arr[y][2];
    c2= arr[y+a*b][2];


    file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
      <<setprecision(6)<<arr[y][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+b][3]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+a*b][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr[y+a*b+b][3]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"s2 =(("
      <<setprecision(6)<<arr2[y][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+b][3]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+a*b][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr2[y+a*b+b][3]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";

}      	
}
file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";
/************************UNORTH*********************************************************************/

/************************USOUTH*********************************************************************/
file1<<"DEFINE_PROFILE(u_south4,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
    
for(n=0;n<a*b*(c-1);n=n+a*b)  
{ 
for(y=0+n; y <a*b-b+n; y=y+b) 
{
 
    a1= arr[y][0];
    a2= arr[y+b][0];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
      file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
      <<setprecision(6)<<arr[y][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+b][3]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+a*b][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr[y+a*b+b][3]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"s2 =(("
      <<setprecision(6)<<arr2[y][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+b][3]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+a*b][3]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr2[y+a*b+b][3]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
      
      	
}
}
file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";
/************************USOUTH*********************************************************************/

/************************UEAST*********************************************************************/
file1<<"DEFINE_PROFILE(u_east5,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
  

for(n=1;n<=(c-1);n++)
{  
for(y=a*b*n-b; y <n*a*b-1 ; y=y+1) 
{
 
    b1= arr[y][1];
    b2= arr[y+1][1];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
        file1 << "if(y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][3]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+1][3]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+a*b][3]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr[y+a*b+1][3]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
<<"s2 =(("
<<setprecision(6)<<arr2[y][3]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+1][3]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+a*b][3]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr2[y+a*b+1][3]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
 
  
      	
}
}
file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";

/************************UEAST*********************************************************************/

/************************UWEST*********************************************************************/
file1<<"DEFINE_PROFILE(u_west6,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
    

for(n=1;n<=(c-1);n++)
{  
for(y=a*b*(n-1); y <(b-1)+a*b*(n-1) ; y=y+1) 
{
 
    b1= arr[y][1];
    b2= arr[y+1][1];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
        file1 << "if(y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][3]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+1][3]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+a*b][3]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr[y+a*b+1][3]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"   
      <<"s2 =(("
<<setprecision(6)<<arr2[y][3]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+1][3]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+a*b][3]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr2[y+a*b+1][3]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"  
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
    

      	
}
}
file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";

/************************UWEST*********************************************************************/

/**************************************************VTOP***************************************/
file1<<"DEFINE_PROFILE(v_top1,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
   
for(n=0;n<(a-1)*b;n=n+b)
{
for(y=a*b*(c-1)+n; y < (a*b*(c-1)+n+(b-1)); ++y) 
{

    a1= arr[y][0];
    a2= arr[y+b][0];
    b1= arr[y][1];
    b2= arr[y+1][1];
          
  	file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+b][4]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+1][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr[y+b+1][4]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
      <<"s2 =(("
<<setprecision(6)<<arr2[y][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+b][4]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+1][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr2[y+b+1][4]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
    
}  	
      	
}  


file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n";
/*********************************************************VTOP*******************************************/


/************************VDOWN*********************************************************************/
file1<<"DEFINE_PROFILE(v_down2,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
    
  
for(n=0;n<(a-1)*b;n=n+b)
{
for(y=0+n; y < (b-1)+n; ++y) 
{
 
    a1= arr[y][0];
    a2= arr[y+b][0];
    b1= arr[y][1];
    b2= arr[y+1][1];
    
      
  	file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+b][4]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+1][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr[y+b+1][4]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
      <<"s2 =(("
<<setprecision(6)<<arr2[y][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+b][4]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+1][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr2[y+b+1][4]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
}      	
}

file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n";
/************************VDOWN*********************************************************************/


/************************VNORTH*********************************************************************/
file1<<"DEFINE_PROFILE(v_north3,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
  
for(n=0;n<a*b*(c-1);n=n+a*b)  
{
for(y=b-1+n; y <a*b-1+n ; y=y+b)
{
    a1= arr[y][0];
    a2= arr[y+b][0];
    c1= arr[y][2];
    c2= arr[y+a*b][2];


    file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
      <<setprecision(6)<<arr[y][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+b][4]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+a*b][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr[y+a*b+b][4]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"s2 =(("
      <<setprecision(6)<<arr2[y][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+b][4]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+a*b][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr2[y+a*b+b][4]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";

}      	
}  

file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";
/************************VNORTH*********************************************************************/

/************************VSOUTH*********************************************************************/
file1<<"DEFINE_PROFILE(v_south4,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
   
  
for(n=0;n<a*b*(c-1);n=n+a*b)  
{ 
for(y=0+n; y <a*b-b+n; y=y+b) 
{
 
    a1= arr[y][0];
    a2= arr[y+b][0];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
      file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
      <<setprecision(6)<<arr[y][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+b][4]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+a*b][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr[y+a*b+b][4]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"s2 =(("
      <<setprecision(6)<<arr2[y][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+b][4]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+a*b][4]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr2[y+a*b+b][4]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
      	
}
}

file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";
/************************VSOUTH*********************************************************************/

/************************VEAST*********************************************************************/
file1<<"DEFINE_PROFILE(v_east5,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
    
for(n=1;n<=(c-1);n++)
{  
for(y=a*b*n-b; y <n*a*b-1 ; y=y+1) 
{
 
    b1= arr[y][1];
    b2= arr[y+1][1];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
        file1 << "if(y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][4]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+1][4]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+a*b][4]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr[y+a*b+1][4]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"s2 =(("
<<setprecision(6)<<arr2[y][4]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+1][4]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+a*b][4]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr2[y+a*b+1][4]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
 
  
      	
}
}

file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";

/************************VEAST*********************************************************************/

/************************VWEST*********************************************************************/
file1<<"DEFINE_PROFILE(v_west6,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
    
for(n=1;n<=(c-1);n++)
{  
for(y=a*b*(n-1); y <(b-1)+a*b*(n-1) ; y=y+1) 
{
 
    b1= arr[y][1];
    b2= arr[y+1][1];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
        file1 << "if(y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][4]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+1][4]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+a*b][4]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr[y+a*b+1][4]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"   
<<"s2 =(("
<<setprecision(6)<<arr2[y][4]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+1][4]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+a*b][4]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr2[y+a*b+1][4]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
    

      	
}
}


file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";

/************************VWEST*********************************************************************/


/**************************************************WTOP***************************************/
file1<<"DEFINE_PROFILE(w_top1,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
    
for(n=0;n<(a-1)*b;n=n+b)
{
for(y=a*b*(c-1)+n; y < (a*b*(c-1)+n+(b-1)); ++y) 
{

    a1= arr[y][0];
    a2= arr[y+b][0];
    b1= arr[y][1];
    b2= arr[y+1][1];
          
  	file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+b][5]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+1][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr[y+b+1][5]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
      <<"s2 =(("
<<setprecision(6)<<arr2[y][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+b][5]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+1][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr2[y+b+1][5]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
    
}  	
      	
}  


file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n";
/*********************************************************WTOP*******************************************/


/************************WDOWN*********************************************************************/
file1<<"DEFINE_PROFILE(w_down2,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
  
for(n=0;n<(a-1)*b;n=n+b)
{
for(y=0+n; y < (b-1)+n; ++y) 
{
 
    a1= arr[y][0];
    a2= arr[y+b][0];
    b1= arr[y][1];
    b2= arr[y+1][1];
    
      
  	file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+b][5]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr[y+1][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr[y+b+1][5]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
<<"s2 =(("
<<setprecision(6)<<arr2[y][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+b][5]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<b2<<"-y[1])+("
<<setprecision(6)<<arr2[y+1][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[1]-"<<setprecision(5)<<b1<<")+("
<<setprecision(6)<<arr2[y+b+1][5]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[1]-"<<setprecision(5)<<b1<<"))/"
<<setprecision(9)<<da*db<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
}      	
}  


file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n";
/************************WDOWN*********************************************************************/


/************************WNORTH*********************************************************************/
file1<<"DEFINE_PROFILE(w_north3,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
   
for(n=0;n<a*b*(c-1);n=n+a*b)  
{
for(y=b-1+n; y <a*b-1+n ; y=y+b)
{
    a1= arr[y][0];
    a2= arr[y+b][0];
    c1= arr[y][2];
    c2= arr[y+a*b][2];


    file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
      <<setprecision(6)<<arr[y][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+b][5]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+a*b][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr[y+a*b+b][5]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"s2 =(("
      <<setprecision(6)<<arr2[y][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+b][5]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+a*b][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr2[y+a*b+b][5]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";

}      	
}  

file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";
/************************WNORTH*********************************************************************/


/************************WSOUTH*********************************************************************/
file1<<"DEFINE_PROFILE(w_south4,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
    
for(n=0;n<a*b*(c-1);n=n+a*b)  
{ 
for(y=0+n; y <a*b-b+n; y=y+b) 
{
 
    a1= arr[y][0];
    a2= arr[y+b][0];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
      file1 << "if(y[0]>="<<setprecision(5)<< a1 <<" && y[0]<="<< setprecision(5)<< a2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
      <<setprecision(6)<<arr[y][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+b][5]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr[y+a*b][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr[y+a*b+b][5]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"s2 =(("
      <<setprecision(6)<<arr2[y][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+b][5]<<")*("<<"y[0]-"<<setprecision(5)<<a1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
      <<setprecision(6)<<arr2[y+a*b][5]<<")*("<<setprecision(5)<<a2<<"-y[0])*(y[2]-"<<setprecision(5)<<c1<<")+("
      <<setprecision(6)<<arr2[y+a*b+b][5]<<")*(y[0]-"<<setprecision(5)<<a1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
      <<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
      <<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
      	
}
}  

file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";
/************************WSOUTH*********************************************************************/

/************************WEAST*********************************************************************/
file1<<"DEFINE_PROFILE(w_east5,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
   
for(n=1;n<=(c-1);n++)
{  
for(y=a*b*n-b; y <n*a*b-1 ; y=y+1) 
{
 
    b1= arr[y][1];
    b2= arr[y+1][1];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
        file1 << "if(y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][5]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+1][5]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+a*b][5]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr[y+a*b+1][5]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
<<"s2 =(("
<<setprecision(6)<<arr2[y][5]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+1][5]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+a*b][5]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr2[y+a*b+1][5]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";
 
  
      	
}
}


file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";

/************************WEAST*********************************************************************/

/************************WWEST*********************************************************************/
file1<<"DEFINE_PROFILE(w_west6,t,p)"<<"\n"
    <<"{"<<"\n"<<"real time,s1,s2;"<<"\n"<<"real y[ND_ND]; "<<"\n"<<"face_t f;"<<"\n"
    <<"time=CURRENT_TIME;"<<"\n"
    <<"begin_f_loop(f,t)"<<"\n"<<"{"<<"\n"<<"F_CENTROID(y,f,t);"<<"\n";
   
for(n=1;n<=(c-1);n++)
{  
for(y=a*b*(n-1); y <(b-1)+a*b*(n-1) ; y=y+1) 
{
 
    b1= arr[y][1];
    b2= arr[y+1][1];
    c1= arr[y][2];
    c2= arr[y+a*b][2];
    
        file1 << "if(y[1]>="<<setprecision(5)<< b1 <<" && y[1]<="<< setprecision(5)<< b2 <<
            " && y[2]>="<<setprecision(5)<< c1 <<" && y[2]<="<< setprecision(5)<< c2
      <<"){s1 =(("
<<setprecision(6)<<arr[y][5]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+1][5]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr[y+a*b][5]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr[y+a*b+1][5]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
<<"s2 =(("
<<setprecision(6)<<arr2[y][5]<<")*("<<setprecision(5)<<b2<<"-y[1])*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+1][5]<<")*("<<"y[1]-"<<setprecision(5)<<b1<<")*("<<setprecision(5)<<c2<<"-y[2])+("
<<setprecision(6)<<arr2[y+a*b][5]<<")*("<<setprecision(5)<<b2<<"-y[1])*(y[2]-"<<setprecision(5)<<c1<<")+("
<<setprecision(6)<<arr2[y+a*b+1][5]<<")*(y[1]-"<<setprecision(5)<<b1<<")*(y[2]-"<<setprecision(5)<<c1<<"))/"
<<setprecision(9)<<da*(c2-c1)<<";"<<"\n"
<<"F_PROFILE(f,t,p)=s1+(time-"<<hr*3600+min*600<<")*(s2-s1)/600;}"<<"\n";     
    

      	
}
}

file1<<"}"<<"\n"<<"end_f_loop(f,t)"<<"\n"<<"}"<<"\n"<<"\n"<<"\n";

/************************WWEST*********************************************************************/


  delete arr,arr2;
}//%%% min min min
}//%%%hrhrhr
  return 0;
}//----------