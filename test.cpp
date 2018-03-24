#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{  

    short * memblock;
 
    std::ifstream file("all10//a10g", std::ios::in | std::ios::binary|std::ios::ate);


    int nx = 10800;
    int ny = 4800;

    //max 6098


    std::ofstream frame;
    frame.open("a10_10800_4800.ppm", std::ios::trunc);

 
    if (file.is_open() && frame.is_open())
    {

          frame << "P3\n" << nx << " " << ny << "\n255\n";

          int size = file.tellg();
          std::cout << "file size in bytes: " << (int)size << std::endl;
          memblock = new short [size / 2];
          file.seekg (0, std::ios::beg);
          int test = 0;
 
          while(file.tellg()!=size)
          {       
            file.read((char*)&memblock[test], 2);

            short mb =   memblock[test];

            if (mb > 0 && mb <= 1000)
            {
                frame << 0 << " " << 255 << " " << 0 << "\n";    
            }
            else if (mb > 1000 && mb <= 3000)
            {
                frame << 255 << " " << 125 << " " << 0 << "\n";    
            }
            else if (mb > 3000 && mb <= 6098)
            {
                frame << 255 << " " << 0 << " " << 0 << "\n";    
            }
            else
            {
              frame << 0 << " " << 0 << " " << 0 << "\n";    
            }


            
            
            if ((test % 5400) == 0 && false)
            {
              std::cout << "Currently at position" << file.tellg() << " value: " << memblock[test] << std::endl;
            }
              
            test++;
          }
 
         delete [] memblock;
       }
}




        
