//http://stackoverflow.com/questions/32332920/efficiently-load-a-large-mat-into-memory-in-opencv
#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;


void matwrite(const string& filename, const Mat& mat)
{
    ofstream fs(filename, fstream::binary);

    // Header
    int type = mat.type();
    int channels = mat.channels();
    fs.write((char*)&mat.rows, sizeof(int));    // rows
    fs.write((char*)&mat.cols, sizeof(int));    // cols
    fs.write((char*)&type, sizeof(int));        // type
    fs.write((char*)&channels, sizeof(int));    // channels

    // Data
    if (mat.isContinuous())
    {
        fs.write(mat.ptr<char>(0), (mat.dataend - mat.datastart));
    }
    else
    {
        int rowsz = CV_ELEM_SIZE(type) * mat.cols;
        for (int r = 0; r < mat.rows; ++r)
        {
            fs.write(mat.ptr<char>(r), rowsz);
        }
    }
}

Mat matread(const string& filename)
{
    ifstream fs(filename, fstream::binary);

    // Header
    int rows, cols, type, channels;
    fs.read((char*)&rows, sizeof(int));         // rows
    fs.read((char*)&cols, sizeof(int));         // cols
    fs.read((char*)&type, sizeof(int));         // type
    fs.read((char*)&channels, sizeof(int));     // channels

    // Data
    Mat mat(rows, cols, type);
    fs.read((char*)mat.data, CV_ELEM_SIZE(type) * rows * cols);

    return mat;
}

int main()
{
    // Save the random generated data
    {
        Mat m(1024*256, 192, CV_8UC1);
        randu(m, 0, 1000);

        FileStorage fs("fs.yml", FileStorage::WRITE);
        fs << "m" << m;

        matwrite("raw.bin", m);
    }

    // Load the saved matrix

    {
        // Method 1: using FileStorage
        double tic = double(getTickCount());

        FileStorage fs("fs.yml", FileStorage::READ);
        Mat m1;
        fs["m"] >> m1;

        double toc = (double(getTickCount()) - tic) * 1000. / getTickFrequency();
        cout << "Using FileStorage: " << toc << endl;
    }

    {
        // Method 2: usign raw binary data
        double tic = double(getTickCount());

        Mat m2 = matread("raw.bin");

        double toc = (double(getTickCount()) - tic) * 1000. / getTickFrequency();
        cout << "Using Raw: " << toc << endl;
    }

    int dummy;
    cin >> dummy;

    return 0;
}
