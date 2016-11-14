#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>

const int N = 62988;

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
    char filename[16] = "seldata.txt";
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


    while(file >> row)
    {
        sum1 += atof(row[44].c_str());
        sum2 += atof(row[22].c_str());
        sum3 += atof(row[10].c_str());
        sum4 += atof(row[16].c_str());
        sum5 += atof(row[28].c_str());
        //ofs << row[1] << "\t" << row[9] << "\t" << row[22] << "\t" << row[10] << "\t" << row[16] << "\t" << row[28] << std::endl;
    }


    std::ifstream  file1("hw1air_data.csv");

    while(file1 >> row)
    {
        sqrsum1 += (atof(row[44].c_str()) - sum2 / N) * (atof(row[44].c_str()) - sum1 / N);
        sqrsum2 += (atof(row[22].c_str()) - sum2 / N) * (atof(row[22].c_str()) - sum2 / N);
        sqrsum3 += (atof(row[10].c_str()) - sum2 / N) * (atof(row[10].c_str()) - sum3 / N);
        sqrsum4 += (atof(row[16].c_str()) - sum2 / N) * (atof(row[16].c_str()) - sum4 / N);
        sqrsum5 += (atof(row[28].c_str()) - sum2 / N) * (atof(row[28].c_str()) - sum5 / N);
    }

    stdeviation1=sqrt(sqrsum1/N);
    stdeviation2=sqrt(sqrsum2/N);
    stdeviation3=sqrt(sqrsum3/N);
    stdeviation4=sqrt(sqrsum4/N);
    stdeviation5=sqrt(sqrsum5/N);


    std::ifstream  file2("hw1air_data.csv");

    int i=1;
    while(file2>>row)
    {
        ofs <<row[0]<<"\t"<<(atof(row[44].c_str())-sum1/N )/stdeviation1 <<"\t"<< (atof(row[22].c_str())-sum2/N )/stdeviation2
        <<"\t"<<(atof(row[10].c_str())-sum3/N )/stdeviation3<<"\t"<<(atof(row[16].c_str())-sum4/N )/stdeviation4
        <<"\t"<<(atof(row[28].c_str())-sum5/N )/stdeviation5 <<std::endl;
        i++;
    }

    ofs.close(); /*关闭文件*/
    //ofs.clear(); /*清理*/
    //std::cin.get();
    //std::cin.get();
    return 0;
}
