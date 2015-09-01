#include "qa.hpp"
#include "opencv2/core.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
   a5147( argc, argv );
}
#endif

int a5147(int argc, char** argv)
{
    Mat m = (Mat_<int>(6,8) <<
             1,-2,1,0,0,0,0,0,
             0,1,-2,1,0,0,0,0,
             0,0,1,-2,1,0,0,0,
             0,0,0,1,-2,1,0,0,
             0,0,0,0,1,-2,1,0,
             0,0,0,0,0,1,-2,1);

    cout << "Mat m = (Mat_<int>(6,8) << 1,-2,...)\n" << m << "\n" << endl;

    m = m.t();

    cout << "m = m.t()\n" << m << "\n" << endl;

    return 0;
}

