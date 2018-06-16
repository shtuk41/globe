#include "fc.h"

int main()
{
	CTable hue;

	hue.Init(5,
        -407.0, 1.0,//8752
        3345.0, 70.0,//5000
        8344.0, 100.0,//1
        8345.0, 200.0,//0
        8752.0, 300.0);//-407

	std::cout << (hue << 8752.0) << std::endl;
	return 0;
}