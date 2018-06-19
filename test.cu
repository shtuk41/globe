#include <algorithm>
#include <fstream>
#include <iostream>

#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <ctime>

#include <opencv2/opencv.hpp>

#define CHECK(call) \
{                                                       \
    const cudaError_t error = call;                     \
    if (error != cudaSuccess)                           \
    {                                                   \
        printf("Error: %s,%d, ", __FILE__, __LINE__);   \
        printf("code: %d, reason: %s\n", error, cudaGetErrorString(error));\
        exit(1);\
    }\
}

struct RGB
{
    float   R;
    float   G;
    float   B;

    __device__  __host__ void Set(float r, float g, float b)
    {
        R = r;
        G = g;
        B = b;
    }

};

__device__ void SetRgb(RGB *rgb, float r, float g, float b)
{
    rgb->R = r;
    rgb->G = g;
    rgb->B = b;
}


__device__ RGB ConvertHSVtoRGB(float H, float S, float V)
{
  float C = V * S;
  float HH = H / 60;
  float X = C * (1.0f - fabs(fmod(HH, 2.0f) - 1.0f));

  RGB rgb_temp;

  if (HH >= 0 && HH <= 1)
  {
      SetRgb(&rgb_temp,C,X,0);
  }
  else if (HH >= 1 && HH <= 2)
  {
      SetRgb(&rgb_temp,X,C,0);
  }
  else if (HH >= 2 && HH <= 3)
  {
      SetRgb(&rgb_temp,0,C,X);
  }
  else if (HH >= 3 && HH <= 4)
  {
      SetRgb(&rgb_temp,0,X,C);
  } 
  else if (HH >= 4 && HH <= 5)
  {
      SetRgb(&rgb_temp,X,0,C);
  }
  else if (HH >= 5 && HH < 6)
  {
      SetRgb(&rgb_temp,C,0,X);
  }

  float m = V - C;

  RGB rgb;

  SetRgb(&rgb, rgb_temp.R + m, rgb_temp.G + m, rgb_temp.B + m);

  return rgb;
}


__device__ float function_table(float in)//-407
{
    //5,                         -407.0, 0.1,//8752
    //                            3345.0, 70.0,//5000
    //                            7344.0, 135.0,//1000
    //                            8345.0, 150.0,//0
    //                            8752.0, 250.0,

   float input[5] = {-407.0f,3345.0f, 7344.0f,8345.0f,8752.0f};
   float output[5] = {0.1f,70.0f,135.0f,150.0f,250.0f};
   

   float out;
    
    if (in <= input[0])
    {
        out = output[0];
    }
    else if (in > input[0] && in < input[4])
    {
        for (int ii = 0; ii < 4; ii++)
        {
            if (in > input[ii] && in <= input[ii+1])
            {
                out = output[ii] + (in - input[ii]) / (input[ii + 1] - input[ii]) * (output[ii + 1] - output[ii]);
                break;
            }
        }
    }
    else
    {
        out = output[4];
    }


    return out;
}

__global__ void ElvationToRGB(short *elev, RGB* rgbValues)
{
    int threadId = blockIdx.x * blockDim.x + threadIdx.x;

    if (threadId >= 10800 * 4)
        return;

    short mb =   elev[threadId];

    if (mb > 8752)
        mb = 8752;
    else if (mb < -407)
        mb = -407;

    mb =  8752 - 407 - mb;

    float H = function_table(mb);

    RGB rgb  = ConvertHSVtoRGB(H,1,1);

    rgbValues[threadId] = rgb;
}

int main(int argc, char **argv)
{  

    int totalColumns = 10800 * 4;

    int columns = 10800;
    int rows[4] = {4800, 6000, 6000, 4800};

    int totalRows = rows[0] + rows[1] + rows[2] + rows[3];

    const char* fileNames[4][4] = { "all10//a10g", "all10//b10g","all10//c10g","all10//d10g",
                            "all10//e10g", "all10//f10g","all10//g10g","all10//h10g",
                            "all10//i10g", "all10//j10g","all10//k10g","all10//l10g",
                            "all10//m10g", "all10//n10g","all10//o10g","all10//p10g"};

    //std::ofstream frame;
    //frame.open("c10_43200_21600.ppm", std::ios::trunc);

    //frame << "P3\n" << totalColumns<< " " << 4800 + 6000 + 6000 + 4800<< "\n255\n";

    short *memblock = new short[totalColumns * totalRows];
    
    int rowCount = 0;

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

        if (file1.is_open() && file2.is_open() && file3.is_open() && file4.is_open()/* && frame.is_open()*/)
        {

            for (int jj = 0; jj < rows[ii]; jj++)
            {
                file1.read((char*)&memblock[totalColumns * rowCount + 0], columns * sizeof(short));
                file2.read((char*)&memblock[totalColumns * rowCount + columns], columns * sizeof(short));
                file3.read((char*)&memblock[totalColumns * rowCount + columns * 2], columns * sizeof(short));
                file4.read((char*)&memblock[totalColumns * rowCount + columns * 3], columns * sizeof(short));

                //std::cout << "major: " << ii << ", row: " << jj << std::endl;
                rowCount++;
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

    std::cout << "Finished reading elevations" << std::endl;

    cv::Size globeSize(totalColumns, totalRows);
    cv::Mat globeImage(globeSize, CV_8UC3);

    std::cout << "Created globeImage" << std::endl;

    short *d_memblock;
    CHECK(cudaMalloc((short **)&d_memblock, totalColumns * sizeof(short)));
    RGB *d_rgbValues;
    CHECK(cudaMalloc((RGB **)&d_rgbValues, totalColumns * sizeof(RGB)));
    RGB  *h_rgbValues = (RGB*)malloc(sizeof(RGB) * totalColumns);


    clock_t startTimeCuda = clock();

    rowCount = 0;

    while (rowCount < totalRows)
    {

      CHECK(cudaMemcpy(d_memblock, &memblock[rowCount * totalColumns], sizeof(short) * totalColumns, cudaMemcpyHostToDevice));

      int blockSize = 1024;
      int numBlocks = (totalColumns + blockSize - 1) / blockSize;

      ElvationToRGB <<<numBlocks, blockSize>>>(d_memblock,d_rgbValues);   

      cudaError_t err = cudaGetLastError();

      char any;
       
      if (err != cudaSuccess) 
      {
          printf("Error: %s\n", cudaGetErrorString(err));
          std::cin >> any;
      }

      CHECK(cudaDeviceSynchronize()); 

      CHECK(cudaMemcpy(h_rgbValues, d_rgbValues, sizeof(RGB) * totalColumns, cudaMemcpyDeviceToHost));



      uchar *ptr = globeImage.ptr(rowCount);
      for (int col = 0; col < globeImage.cols; col++)
      {
          uchar * uc_pixel = ptr;
          uc_pixel[0] = (int)(h_rgbValues[col].B * 255);
          uc_pixel[1] = (int)(h_rgbValues[col].G * 255);
          uc_pixel[2] = (int)(h_rgbValues[col].R * 255);           
          ptr += 3;
      }


      rowCount++;
    }

    std::cout << "Finished computing rgb values in " << double( clock() - startTimeCuda ) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;

    std::cout << "Started globe.png write" << std::endl;

    clock_t startTime = clock();

    cv::imwrite( "globe.png", globeImage );

    std::cout << "Finished globe.png in " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;


 

    delete [] memblock;
    cudaFree(d_memblock);
    cudaFree(d_rgbValues);
    free(h_rgbValues);

    cudaDeviceReset();

}




        
