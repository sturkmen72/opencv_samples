
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    Mat m = (Mat_<uchar>(6,6) <<
             0,2,0,2,0,2,
             0,2,0,2,0,2,
             0,2,0,2,0,2,
             0,2,0,2,0,2,
             0,2,0,2,0,2,
             0,2,0,2,0,2);

    cout << "Mat m = (Mat_<uchar>(6,6) << 0,2,...)\n" << m << "\n" << endl;

    Mat m_resized;

    resize( m, m_resized, Size(), 0.5, 0.5, INTER_NEAREST );

    cout << "resized m ( with INTER_NEAREST flag )\n" << m_resized << "\n" << endl;

    resize( m, m_resized, Size(), 0.5, 0.5, INTER_LINEAR );

    cout << "resized m ( with INTER_LINEAR flag )\n" << m_resized << "\n" << endl;

    resize( m, m_resized, Size(), 0.5, 0.5, INTER_AREA );

    cout << "resized m ( with INTER_AREA flag )\n" << m_resized << "\n" << endl;
    return 0;
}

