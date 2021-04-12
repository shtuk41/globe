#include "fc.h"

CTable::CTable() : input(0), output(0),numberPoints(0)
{

}

void CTable::Init(int n_points, ...)
{
   numberPoints = n_points;
   input = new double[n_points];
   output = new double[n_points];
   
   va_list ap;
   va_start(ap, n_points);
   for(int ii = 0; ii < n_points; ii++) 
   {
       input[ii]  = va_arg(ap, double);
       output[ii]  = va_arg(ap, double);

   }
   va_end(ap);
}

double CTable::operator<<(double in)
 {
    double out = output[0];
    
    if (in <= input[0])
    {
        return out;
    }
    else if (in > input[0] && in < input[numberPoints - 1])
    {
        for (int ii = 0; ii < numberPoints - 1; ii++)
        {
            if (in > input[ii] && in <= input[ii+1])
            {
                out = output[ii] + (in - input[ii]) / (input[ii + 1] - input[ii]) * (output[ii + 1] - output[ii]);
                return out;
            }
        }
    }
    else
    {
        out = output[numberPoints - 1];
        return out;
    }
 }

 CTable::~CTable()
 {
     if (input != 0)
     {
         delete [] input;
         input = 0;
     }
     
     if (output != 0)
     {
         delete [] output;
         output = 0;
     }
 }