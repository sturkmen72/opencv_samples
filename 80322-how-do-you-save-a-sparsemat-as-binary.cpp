#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;

typedef Vec<float,5> Vec5f;

#include <fstream>
#define SPARSEBINARY

bool write2d(const char *fn, SparseMat &m)
{
    ofstream s(fn, ios::out | ios::binary);

    int h = m.hdr->size[0];
    int w = m.hdr->size[1];
    #ifdef SPARSEBINARY
        s.write((char*)&h, sizeof(int));
        s.write((char*)&w, sizeof(int));
    #else // text
        s << h << " " << w << endl;
    #endif

    for (int i=0; i<h; i++)
    {
        for (int j=0; j<w; j++)
        {
            if (0 == m.ptr(i,j,false))
                continue;
            Vec5f e = m.ref<Vec5f>(i,j);
            #ifdef SPARSEBINARY
                s.write((char*)&i, sizeof(int));
                s.write((char*)&j, sizeof(int));
                s.write((char*)&e, sizeof(Vec5f));
            #else // text
                s << i << " " << j << " ";
                s << e[0] << " " << e[1] << " " << e[2] << " " << e[3] << " " << e[4] << endl;
            #endif
        }
    }
    s.close();
    return true;
}

bool read2d(const char *fn, SparseMat &m)
{
    ifstream s(fn, ios::out | ios::binary);
    if (!s) return false;

    int h=0,w=0;
    #ifdef SPARSEBINARY
        s.read((char*)&h,sizeof(int));
        s.read((char*)&w,sizeof(int));
    #else
        s >> h >> w;
    #endif

    int sz[] = {h,w};
    m = SparseMat(2,sz,CV_32FC(5));
    while(!s.eof())
    {
        int i,j;
        Vec5f e;
        #ifdef SPARSEBINARY
            s.read((char*)&i,sizeof(int));
            s.read((char*)&j,sizeof(int));
            s.read((char*)&e,sizeof(Vec5f));
        #else
            s >> i >> j >> e[0] >> e[1] >> e[2] >> e[3] >> e[4];
        #endif
        if (e == Vec5f::zeros()) // weird..
            break;
        m.ref<Vec5f>(i,j) = e;
    }
    s.close();
    return true;
}

int main(int argc, char **argv)
{
    int sz[] = {10,10};
    SparseMat m(2,sz,CV_32FC(5));

    m.ref<Vec5f>(2,2) = Vec5f(1,2,3,4,5);
    m.ref<Vec5f>(4,7) = Vec5f(2.2,1.1,3,3,3);
    m.ref<Vec5f>(7,3) = Vec5f(6,7,8.1,9,0);
    write2d("my.xxx",m);

    SparseMat m2;
    read2d("my.xxx",m2);
    cerr << m2.ref<Vec5f>(2,2) << endl;
    cerr << m2.ref<Vec5f>(4,7) << endl;
    cerr << m2.ref<Vec5f>(7,3) << endl;
    cerr << m2.ref<Vec5f>(9,9) << endl; // does not exist
    return 0;
}
