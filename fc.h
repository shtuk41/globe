#ifndef FC_GLOBE
#define FC_GLOBE

#include <stdarg.h>
#include <iostream>

class CTable
{
public:
    CTable();
    
    template<typename... Args>
    void Init(int n_points, Args... args);
    double operator<<(double in);
    ~CTable();
private:
    double *input;
    double *output;
    int numberPoints;
};

#endif
