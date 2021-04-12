#include <ctime>
#include <fstream>
#include <string>
#include <stdio.h>


#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



using namespace cv;

const int startX = 25490;
const int endX = startX + 500;
const int startY = 5200;
const int endY = startY + 500;

const std::string outputFileName = "crimea.stl";

const float heightMultiplier = 50.0f;

void computeNormal(float n[3], 
                    float x1, float y1, float z1,
                    float x2, float y2, float z2,
                    float x3, float y3, float z3)
{
    n[0] = (y2 - y1) * (z3 - z1) - (y3 - y1) * (z2 - z1);
    n[1] = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
    n[2] = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
}

float convertToKm(int value)
{
    float ret = -(float)value / 255.0f * 9.1590f * heightMultiplier;
    
    return ret;
}

int main()
{
 
    std::string path = "bw.png";                                                                                                   
    
    std::ifstream check(path.c_str());
    
    if (check.good())
    {
        std::cout << "file exists" << std::endl;
    }
    else
    {
        
        std::cout << "cannot find file" << std::endl;
    }
    
    cv::Mat mapFile = cv::imread(path.c_str(), cv::IMREAD_GRAYSCALE);
    
    if (mapFile.empty())
    {
        std::cout << "image empty" << std::endl;
        return -1;
        
    }
    else
    {
        std::cout << "cols: " << mapFile.cols << std::endl;
        std::cout << "rows: " << mapFile.rows << std::endl;
    }
    
 
    std::cout << "Output file name: " << outputFileName << std::endl;
    std::ofstream wf(outputFileName.c_str(), std::ios::out | std::ios::binary);
    
    unsigned long numberOfTriangles = (endX - startX) * (endY - startY) * 2;
    
    std::cout << "Max triangles supported: " << INT_MAX << std::endl;
    std::cout << "number of triangles: "  << numberOfTriangles << std::endl;
    
    char head[80];
    strncpy(head,outputFileName.c_str(), strlen(outputFileName.c_str()));
    
    wf.write(head, sizeof(head));
    wf.write((char*)&numberOfTriangles, 4);
    
    char attribute[2] = "0";
    
    unsigned int tCount = 0;
    
    clock_t startTimeloop = clock();
    
    for (int c = startX; c <  endX; c++)
    {
        for (int r = startY; r <  endY; r++)
        {
            
            float v1x = c;
            float v1y = r;
            float v1z = convertToKm((int)mapFile.at<unsigned char>(r,c));
            
            float v2x = c;
            float v2y = (r + 1);
            float v2z = convertToKm((int)mapFile.at<unsigned char>(r + 1,c));
            
            float v3x = (c + 1);
            float v3y = r;
            float v3z = convertToKm((int)mapFile.at<unsigned char>(r,c + 1));
       
            float normal[3];
            
            computeNormal(normal, v1x,v1y,v1z,
                                    v2x,v2y,v2z,
                                    v3x,v3y,v3z);
       
            
            
            wf.write((char*)&normal[0], 4);
            wf.write((char*)&normal[1], 4);
            wf.write((char*)&normal[2], 4);
            
            wf.write((char*)&v1x, 4);
            wf.write((char*)&v1y, 4);
            wf.write((char*)&v1z, 4);
            
            wf.write((char*)&v2x, 4);
            wf.write((char*)&v2y, 4);
            wf.write((char*)&v2z, 4);
            
            wf.write((char*)&v3x, 4);
            wf.write((char*)&v3y, 4);
            wf.write((char*)&v3z, 4);
            
            

            wf.write(attribute, 2);
            
            tCount++;
            
            v1x = (c + 1);
            v1y = r;
            v1z = convertToKm((int)mapFile.at<unsigned char>(r,c + 1));
            
            v2x = c;
            v2y = (r + 1);
            v2z = convertToKm((int)mapFile.at<unsigned char>(r + 1,c));
            
            v3x = (c + 1);
            v3y = (r + 1);
            v3z = convertToKm((int)mapFile.at<unsigned char>(r + 1,c + 1));
       
            computeNormal(normal, v1x,v1y,v1z,
                                    v2x,v2y,v2z,
                                    v3x,v3y,v3z);
              
            wf.write((char*)&normal[0], 4);
            wf.write((char*)&normal[1], 4);
            wf.write((char*)&normal[2], 4);
            
            wf.write((char*)&v1x, 4);
            wf.write((char*)&v1y, 4);
            wf.write((char*)&v1z, 4);
            
            wf.write((char*)&v2x, 4);
            wf.write((char*)&v2y, 4);
            wf.write((char*)&v2z, 4);
            
            wf.write((char*)&v3x, 4);
            wf.write((char*)&v3y, 4);
            wf.write((char*)&v3z, 4);
            
            wf.write(attribute, 2);
            
            tCount++;
        }
    }
    
    std::cout << "Finished loop " << double( clock() - startTimeloop ) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;

    std::cout << "number of triangles written: "  << tCount << std::endl;
    
    return 0;
}