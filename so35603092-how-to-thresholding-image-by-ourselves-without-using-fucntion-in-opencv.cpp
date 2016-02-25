#include <opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
    Mat1b src(720,960);
    randu(src, 0, 256);

    Mat1b src1 = src.clone();
    Mat1b src2 = src.clone();
    Mat1b src3 = src.clone();

    double tic1 = double(getTickCount());

    // Method @HenryChen (corrected)
    for (int i = 0; i < src1.rows; i++)
    {
        //cout << i << endl;
        for (int j = 0; j < src1.cols; j++)
        {
            if (src1.at<uchar>(i, j) > 70) {
                src1.at<uchar>(i, j) = 0;
                //cout << j << endl;
            }
            else
                src1.at<uchar>(i, j) = 255;
        }
    }

    double toc1 = (double(getTickCount()) - tic1) * 1000.0 / getTickFrequency();
    cout << "Time @HenryChen (ms): \t" << toc1 << endl;

    //-------------------------------------

    double tic2 = double(getTickCount());

    // Method @Miki
    int rows = src2.rows;
    int cols = src2.cols;
    if (src2.isContinuous())
    {
        cols = rows * cols;
        rows = 1;
    }

    for (int i = 0; i < rows; i++)
    {
        uchar* pdata = src2.ptr<uchar>(0);
        int base = i*cols;
        for (int j = 0; j < cols; j++)
        {
            pdata[base + j] = (pdata[base + j] > 70) ? uchar(0) : uchar(255);
        }
    }

    double toc2 = (double(getTickCount()) - tic2) * 1000.0 / getTickFrequency();
    cout << "Time @Miki (ms): \t" << toc2 << endl;

    //-------------------------------------

    double tic3 = double(getTickCount());

    // Method @OpenCV
    threshold(src3, src3, 70, 255, THRESH_BINARY_INV);

    double toc3 = (double(getTickCount()) - tic3) * 1000.0 / getTickFrequency();
    cout << "Time @OpenCV (ms): \t" << toc3 << endl;


    getchar();
    return 0;
}
