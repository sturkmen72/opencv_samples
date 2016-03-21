#include <opencv2/opencv.hpp>
using namespace cv;

#include <vector>
using namespace std;

#define HISTMATCH_EPSILON 0.000001
#define BTM_DEBUG
// Compute histogram and CDF for an image with mask
void do1ChnHist(const Mat_<uchar> &img, const Mat_<uchar> &mask, Mat_<double> &h, Mat_<double> &cdf)
{
    for(size_t p=0; p<img.total(); p++)
    {
        if(mask(p) > 0)
        {
            uchar c = img(p);
            h(c) += 1.0;
        }
    }

    normalize(h,h,1,0,NORM_MINMAX);

    cdf(0) = h(0);
    for(int j=1; j<256; j++)
    {
        cdf(j) = cdf(j-1) + h(j);
    }

    normalize(cdf,cdf,1,0,NORM_MINMAX);
}

// match histograms of 'src' to that of 'dst', according to both masks
void histMatchRGB(Mat &src, const Mat &src_mask, const Mat &dst, const Mat &dst_mask)
{
#ifdef BTM_DEBUG
    namedWindow("original source",CV_WINDOW_AUTOSIZE);
    imshow("original source",src);
    namedWindow("original query",CV_WINDOW_AUTOSIZE);
    imshow("original query",dst);
#endif

    vector<Mat_<uchar>> chns, chns1;
    split(src,chns);
    split(dst,chns1);

    for (int i=0; i<3; i++)
    {
        Mat_<double> src_hist = Mat_<double>::zeros(1,256);
        Mat_<double> dst_hist = Mat_<double>::zeros(1,256);
        Mat_<double> src_cdf  = Mat_<double>::zeros(1,256);
        Mat_<double> dst_cdf  = Mat_<double>::zeros(1,256);

        do1ChnHist(chns[i],  src_mask, src_hist, src_cdf);
        do1ChnHist(chns1[i], dst_mask, dst_hist, dst_cdf);

        uchar last = 0;

        Mat_<uchar> lut(1,256);
        for (int j=0;j<src_cdf.cols;j++)
        {
            double F1j = src_cdf(j);

            for (uchar k = last; k<dst_cdf.cols; k++)
            {
                double F2k = dst_cdf(k);
                if(abs(F2k - F1j) < HISTMATCH_EPSILON || F2k > F1j)
                {
                    lut(j) = k;
                    last = k;
                    break;
                }
            }
        }

        LUT(chns[i],lut,chns[i]);
    }

    Mat res;
    merge(chns,res);

#ifdef BTM_DEBUG
    namedWindow("matched",CV_WINDOW_AUTOSIZE);
    imshow("matched",res);

    waitKey(0);
#endif

    res.copyTo(src);
}

int main(int argc, char **argv)
{
    Mat src = imread("fruits.jpg");
    Mat dst = imread("lena.jpg");
    Mat mask = Mat(src.size(), CV_8U, Scalar(255));
    //Mat src_mask = imread("image008_mask.bmp",0);
    //Mat dst_mask = imread("image003_mask.bmp",0);

    histMatchRGB(dst,mask,src,mask);
    return 0;
}
