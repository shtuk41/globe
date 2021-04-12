#include <stdarg.h>
#include <iostream>

class CTable
{
public:
    CTable();
    void Init(int n_points, ...);
    double operator<<(double in);
    ~CTable();
private:
    double *input;
    double *output;
    int numberPoints;
};