#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>

#include "fc.h"
#include "colors.h"


int main(int argc, char **argv)
{  

    int totalColumns = 10800 * 4;
    int totalRows = 4800 + 6000 + 6000 + 4800;

    int columns = 10800;
    int rows[4] = {4800, 6000, 6000, 4800};
    const char* fileNames[4][4] = { "all10//a10g", "all10//b10g","all10//c10g","all10//d10g",
                            "all10//e10g", "all10//f10g","all10//g10g","all10//h10g",
                            "all10//i10g", "all10//j10g","all10//k10g","all10//l10g",
                            "all10//m10g", "all10//n10g","all10//o10g","all10//p10g"};

    std::ofstream frame;
    frame.open("c10_43200_21600.ppm", std::ios::trunc);

    frame << "P3\n" << totalColumns<< " " << 4800 + 6000 + 6000 + 4800<< "\n255\n";

    short *memblock = new short[totalColumns];

    CTable hue;
    hue.Init(5,
            -407.0, 0.1,//8752
            3345.0, 70.0,//5000
            7344.0, 135.0,//1000
            8345.0, 150.0,//0
            8752.0, 250.0);//-407


    for (int ii = 0; ii < 4; ii++)
    {
        std::ifstream file1(fileNames[ii][0], std::ios::in | std::ios::binary); 
        std::ifstream file2(fileNames[ii][1], std::ios::in | std::ios::binary);  
        std::ifstream file3(fileNames[ii][2], std::ios::in | std::ios::binary);  
        std::ifstream file4(fileNames[ii][3], std::ios::in | std::ios::binary); 

        file1.seekg(0, std::ios::beg); 
        file2.seekg(0, std::ios::beg); 
        file3.seekg(0, std::ios::beg); 
        file4.seekg(0, std::ios::beg);  

        if (file1.is_open() && file2.is_open() && file3.is_open() && file4.is_open() && frame.is_open())
        {

            for (int jj = 0; jj < rows[ii]; jj++)
            {
                file1.read((char*)&memblock[0], columns * sizeof(short));
                file2.read((char*)&memblock[columns], columns * sizeof(short));
                file3.read((char*)&memblock[columns * 2], columns * sizeof(short));
                file4.read((char*)&memblock[columns * 3], columns * sizeof(short));

                for (int cc = 0; cc < totalColumns; cc++)
                {
                      double mb =   (double)memblock[cc];
                      mb =   std::max(std::min(mb, 8752.0), -407.0);

                      mb =  8752 - 407 - mb;

                      double H = hue << mb;

                      RGB rgb = ConvertHSVtoRGB(H,1,1);

                      frame << (int)(rgb.R * 255) << " " << (int)(rgb.G * 255) << " " << (int)(rgb.B * 255) << "\n";   
                }

                std::cout << "major: " << ii << ", row: " << jj << std::endl;
            }

            file1.close();
            file2.close();
            file3.close();
            file4.close(); 
        } 
        else
        {
            std::cout << "batch: " << ii << " one of the files unable to open" << std::endl;
            std::cin.get();
            return 0;
        } 
    }
 

    delete [] memblock;
}




        
