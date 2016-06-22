// https://stackoverflow.com/a/22127181/5294258
// by Andrey Smorodov

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"


using namespace cv;

//-----------------------------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------------------------------
void CalcBlockMeanVariance(Mat& Img, Mat& Res, float blockSide = 21) // blockSide - the parameter (set greater for larger font on image)
{
	Mat I;
	Img.convertTo(I, CV_32FC1);
	Res = Mat::zeros(Img.rows / blockSide, Img.cols / blockSide, CV_32FC1);
	Mat inpaintmask;
	Mat patch;
	Mat smallImg;
	Scalar m, s;

	for (int i = 0; i<Img.rows - blockSide; i += blockSide)
	{
		for (int j = 0; j<Img.cols - blockSide; j += blockSide)
		{
			patch = I(Range(i, i + blockSide + 1), Range(j, j + blockSide + 1));
			cv::meanStdDev(patch, m, s);
			if (s[0]>0.01) // Thresholding parameter (set smaller for lower contrast image)
			{
				Res.at<float>(i / blockSide, j / blockSide) = m[0];
			}
			else
			{
				Res.at<float>(i / blockSide, j / blockSide) = 0;
			}
		}
	}

	cv::resize(I, smallImg, Res.size());

	cv::threshold(Res, inpaintmask, 0.02, 1.0, cv::THRESH_BINARY);

	Mat inpainted;
	smallImg.convertTo(smallImg, CV_8UC1, 255);

	inpaintmask.convertTo(inpaintmask, CV_8UC1);
	inpaint(smallImg, inpaintmask, inpainted, 5, INPAINT_TELEA);

	cv::resize(inpainted, Res, Img.size());
	Res.convertTo(Res, CV_32FC1, 1.0 / 255.0);

}
//-----------------------------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	namedWindow("Img");
	namedWindow("Edges");
	Mat Img = imread(argv[1], 0);
	Mat res;
	Img.convertTo(Img, CV_32FC1, 1.0 / 255.0);
	CalcBlockMeanVariance(Img, res);
	res = 1.0 - res;
	res = Img + res;
	imshow("Img", Img);
	cv::threshold(res, res, 0.85, 1, cv::THRESH_BINARY);
	cv::resize(res, res, cv::Size(res.cols / 2, res.rows / 2));
	imshow("Edges", res);
	waitKey();

	return 0;
}