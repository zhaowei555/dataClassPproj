// datapro.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#define k 32//簇的数目  
using namespace std;  
//存放元组的属性信息  
typedef vector<double> Tuple;//存储每条数据记录  
  
int dataNum = 62988;//数据集中数据记录数目  
static int dimNum=5;//每条记录的维数   
const int N = 62988;
using namespace std;
double tmp[62988];
double my_atoi(string &c);
void convert_to_month();
void ymd_txt_to_memory();
double variance(double x[ ],int n,double mean);
struct YMD{
      string year;
      string month;
      string day;
      string analysis_line(const string &c){
         int i = 0, j = 0;
	     char s[50];  
         while(c[i]!='/')s[j++]=c[i++];
         s[j]='\0';year=s;
        // printf("%s\n",s);   
         j=0;
         i++;
         while(c[i]!='/')s[j++]=c[i++];
         s[j]='\0';month=s;
      //  printf("%s\n",s);   
         i++;
         j=0;
         while(c[i]!=' '&&c[i]!='\t'&&c[i]!='\0'&&c[i]!='\n')s[j++]=c[i++];
         s[j]='\0';
		 day=s; 
       //   printf("%s\n",s);   
         return day;
         }
};
static struct YMD ymd[62988];

double getDistXY(const Tuple& t1, const Tuple& t2)   
{  
    double sum = 0;  
    for(int i=1; i<=dimNum; ++i)  
    {  
        sum += (t1[i]-t2[i]) * (t1[i]-t2[i]);  
    }  
    return sqrt(sum);  
}  
  
//根据质心，决定当前元组属于哪个簇  
int clusterOfTuple(Tuple means[],const Tuple& tuple){  
    double dist=getDistXY(means[0],tuple);  
    double tmp;  
    int label=0;//标示属于哪一个簇  
    for(int i=1;i<k;i++){  
        tmp=getDistXY(means[i],tuple);  
        if(tmp<dist) {dist=tmp;label=i;}  
		    }  
    return label;     
}  
//获得给定簇集的平方误差  
double getVar(vector<Tuple> clusters[],Tuple means[]){  
    double var = 0;  
    for (int i = 0; i < k; i++)  
    {  
        vector<Tuple> t = clusters[i];  
        for (unsigned int j = 0; j<t.size(); j++)  
        {  
            var += getDistXY(t[j],means[i]);  
        }  
    }  
    //cout<<"sum:"<<sum<<endl;  
    return var;  
  
}  
//获得当前簇的均值（质心）  
Tuple getMeans(const vector<Tuple>& cluster){  
      
    int num = cluster.size();  
    Tuple t(dimNum+1, 0);  
    for (int i = 1; i <=dimNum; i++)  
    {  
        for(int j=0; j<num; ++j)  
        {  
			t[i] += cluster[j][i];  
        }  
    }  
    for(int i=1; i<=dimNum; ++i)  
        t[i] /= num;  
    return t;  
    //cout<<"sum:"<<sum<<endl;  
}  
   
void sort(double a[][7])
{

    for(int i=0;i<32;i++) 

    { 

        int n=i;//选择排序法，k放最小值所在的 行标号 

        for(int m=i+1;m<32;m++)
		{
			if(a[m][6]>a[n][6])
				n=m;
		}

        if(n!=i)//交换行 

        { 

            for(int j=0;j<7;j++) 

            { 

                double h=a[i][j]; 

                a[i][j]=a[n][j]; 

                a[n][j]=h; 

            } 

        } 

    } 
}
  
void KMeans(vector<Tuple>& tuples){  
    vector<Tuple> clusters[k];//k个簇  
    Tuple means[k];//k个中心点  
    int i=0;  
	for(i=0;i<k;i++)
	{
		for(int j=0; j<=dimNum; ++j)  
            {  
                means[i].push_back(tuples[i][j]);  
            }  
	}

    int lable=0;  
    //根据默认的质心给簇赋值  
    for(i=0;i!=tuples.size();++i){  
        lable=clusterOfTuple(means,tuples[i]);  
        clusters[lable].push_back(tuples[i]);  
    }  
    double oldVar=-1;  
    double newVar=getVar(clusters,means);  
    cout<<"初始的的整体误差平方和为："<<newVar<<endl;   
    int t = 0;  
    while(abs(newVar - oldVar) >= 20) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止  
    {  
        cout<<"第 "<<++t<<" 次迭代开始："<<endl;  
        for (i = 0; i < k; i++) //更新每个簇的中心点  
       {  
            means[i] = getMeans(clusters[i]);  
        }  
        oldVar = newVar;  
        newVar = getVar(clusters,means); //计算新的准则函数值  
        for (i = 0; i < k; i++) //清空每个簇  
        {  
            clusters[i].clear();  
        }  
        //根据新的质心获得新的簇  
        for(i=0; i!=tuples.size(); ++i){  
            lable=clusterOfTuple(means,tuples[i]);  
            clusters[lable].push_back(tuples[i]);  
        }  
        cout<<"此次迭代之后的整体误差平方和为："<<newVar<<endl; 


    } 
	int num[32];
	for(i=0;i<32;i++)
	{
		num[i]=clusters[i].size();
	}
	double s[32][7];
	for(int i = 0;i<32;i++)
	{
		s[i][0]=i+1;
		for(int j=1;j<6;j++)
			{
				s[i][j] = means[i][j];
		     }
		s[i][6] = 0.0395*s[i][0]+0.0771*s[i][1]+0.2756*s[i][2]+0.2166*s[i][3]+0.3912*s[i][4];
	}
    sort(s);
	double allmean[5];
	double su;
	for(i=1;i<6;i++)
	{
		su=0;
		for(int j=0;j<32;j++)
		{
			su+=s[j][i];
		}
		allmean[i-1]=su/32;
	}
	char strm[32][5];
	for(i=0;i<32;i++)
	{
		for(int j=0;j<5;j++)
		{
			if(s[i][j+1]<allmean[j])
				strm[i][j]='L';
			else
				strm[i][j]='H';
		}
	}


    cout<<"The result is:\n";  
	string str[32];
	for(i = 0;i<32;i++)
	{
		if(s[i][6]>5)
			str[i]="high value";
		else if(s[i][6]<1)
			str[i]="low value";
		else
			str[i]="medium";
	}
	ofstream outFile;
	outFile.open("data.txt");
	outFile.width(15);
	outFile<<left<<"Label";
	outFile.width(15);
	outFile<<left<<"L";
	outFile.width(15);
	outFile<<left<<"R";
	outFile.width(15);
	outFile<<left<<"F";
	outFile.width(15);
	outFile<<left<<"M";
	outFile.width(15);
	outFile<<left<<"C";
	outFile.width(15);
	outFile<<left<<"value";
	outFile.width(15);
	outFile<<left<<"价值";
	outFile.width(15);
	outFile<<left<<"sum";
	outFile.width(15);
	outFile<<left<<"LRFMC";
	outFile<<"\n";

	for(int i = 0;i<32;i++)
	{
		for(int j=0;j<7;j++)
		{
			outFile.width(15);
			outFile<<left<<s[i][j];
		}
		outFile.width(15);
		outFile<<left<<str[i];
		outFile.width(15);
		outFile<<left<<num[i];
		for(int t=0;t<5;t++)
			outFile<<strm[i][t];
		outFile<<"\n";
	}
} 
class CSVRow
{
public:
    std::string const & operator[](std::size_t index) const
    {
        return m_data[index];
    }
    std::size_t size() const
    {
        return m_data.size();
    }
    void readNextRow(std::istream & str)
    {
        std::string         line;
        std::getline(str, line);
        std::stringstream   lineStream(line);
        std::string         cell;
        m_data.clear();

        while(std::getline(lineStream, cell, ','))
        {
            m_data.push_back(cell);
        }
    }
private:
    std::vector<std::string>    m_data;
};

std::istream & operator>>(std::istream & str, CSVRow & data)
{
    data.readNextRow(str);
    return str;
}
int main()
{
    std::ifstream  file("hw1air_data.csv");
    CSVRow  row;
    char filename[16] = "seldat.txt";
    std::ofstream ofs(filename, std::ios::app);
    double sum1 = 0;
    double sum2 = 0;
    double sum3 = 0;
    double sum4 = 0;
    double sum5 = 0;

    double sqrsum1 = 0;
    double sqrsum2 = 0;
    double sqrsum3 = 0;
    double sqrsum4 = 0;
    double sqrsum5 = 0;

    double stdeviation1 = 0;
    double stdeviation2 = 0;
    double stdeviation3 = 0;
    double stdeviation4 = 0;
    double stdeviation5 = 0;

	ymd_txt_to_memory();
	convert_to_month();
    while(file >> row)
    {
        //sum1 += atof(row[44].c_str());
        sum2 += atof(row[22].c_str());
        sum3 += atof(row[10].c_str());
        sum4 += atof(row[16].c_str());
        sum5 += atof(row[28].c_str());
        //ofs << row[1] << "\t" << row[9] << "\t" << row[22] << "\t" << row[10] << "\t" << row[16] << "\t" << row[28] << std::endl;
    }


    std::ifstream  file1("hw1air_data.csv");
	file1>>row;
    while(file1 >> row)
    {
        //sqrsum1 += (atof(row[44].c_str()) - sum2 / N) * (atof(row[44].c_str()) - sum1 / N);
        sqrsum2 += (atof(row[22].c_str()) - sum2 / N) * (atof(row[22].c_str()) - sum2 / N);
        sqrsum3 += (atof(row[10].c_str()) - sum2 / N) * (atof(row[10].c_str()) - sum3 / N);
        sqrsum4 += (atof(row[16].c_str()) - sum2 / N) * (atof(row[16].c_str()) - sum4 / N);
        sqrsum5 += (atof(row[28].c_str()) - sum2 / N) * (atof(row[28].c_str()) - sum5 / N);
    }
    stdeviation2=sqrt(sqrsum2/N);
    stdeviation3=sqrt(sqrsum3/N);
    stdeviation4=sqrt(sqrsum4/N);
    stdeviation5=sqrt(sqrsum5/N);
	

    std::ifstream  file2("hw1air_data.csv");

    int i=0;
	file2>>row;
    while(file2>>row)
    {
        ofs <<tmp[i]<<"\t"<< (atof(row[22].c_str())-sum2/N )/stdeviation2
        <<"\t"<<(atof(row[10].c_str())-sum3/N )/stdeviation3<<"\t"<<(atof(row[16].c_str())-sum4/N )/stdeviation4
        <<"\t"<<(atof(row[28].c_str())-sum5/N )/stdeviation5 <<std::endl;
        i++;
    }

    ofs.close(); /*关闭文件*/
    //ofs.clear(); /*清理*/
	cout<<"data ok"<<endl;
	ifstream infile("seldat.txt");  
    if(!infile){  
        cout<<"不能打开输入的文件"<<endl;  
        return 0;  
    }  
    vector<Tuple> tuples;  
    //从文件流中读入数据  
    for(int i=0; i<dataNum && !infile.eof(); ++i)  
    {  
        string str;  
        getline(infile, str);  
        istringstream istr(str);  
        Tuple tuple(dimNum+1, 0);//第一个位置存放记录编号，第2到dimNum+1个位置存放实际元素  
        tuple[0] = i+1;  
        for(int j=1; j<=dimNum; ++j)  
        {  
            istr>>tuple[j];  
        }  
        tuples.push_back(tuple);  
    }  

  
    cout<<endl<<"开始聚类"<<endl;  
    KMeans(tuples);  
    std::cin.get();
    std::cin.get();
	
    return 0;
}
void convert_to_month(){
    double sum=0;
    double mean=0;
    for (int i=0;i<62988;i++){
        
        tmp[i]=2014*12+4-my_atoi(ymd[i].year)*12-my_atoi(ymd[i].month);
        sum+=tmp[i];
       
    }
    mean=sum/62988;   
    double var=variance(tmp,62988,mean);
    for(int i=0;i<62988;i++)
	{
        tmp[i]=(tmp[i]-mean)/sqrt(var);
    }
    
}

void ymd_txt_to_memory()
{
	int i=0;
	std::ifstream  file("hw1air_data.csv");
	CSVRow  row;
	file>>row;
	while(file>>row)
	{
	ymd[i].analysis_line(row[2]);
	i++;
	}
}
double my_atoi(string &c){
            
    int ret=0,sign=1,i;  
    for( i=0; c[i]==' '||c[i]=='\t'; i++);  
    if(c[i]== '-')sign = -1;  
    if(c[i]== '-' || c[i] == '+')i++;  
    while(isdigit(c[i])){  
        ret = ret*10 + c[i] - '0';  
        i++;  
        } 
      return (double)sign*ret; 
}

double variance(double x[],int n,double mean)
{
   double divisor,sum;
   int k1;
   for(sum=k1=0;k1<n;k1++)
      sum+=pow(x[k1]-mean,2);
   if(n<20)divisor=n-1;
    else divisor=n;
   return sum/divisor;
}