
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2/shape.hpp"

#include <iostream>
#include <conio.h>
#include <fstream>
#include <string.h>

#define NOMINMAX

using namespace cv;
using namespace std;

Point RotatePoint(const Mat &R, const Point &p)
{
	Point2f rp;
	rp.x = (float)(R.at<double>(0, 0)*p.x + R.at<double>(0, 1)*p.y + R.at<double>(0, 2));
	rp.y = (float)(R.at<double>(1, 0)*p.x + R.at<double>(1, 1)*p.y + R.at<double>(1, 2));
	return rp;
}

bool FindBlobs(Mat mI, vector<vector<Point>> &contours, vector<Point2f> &MassCentre, vector<float>&DiagonalLength)
{
	if (mI.empty() || mI.depth() != CV_8UC1)
	{
		cout << "Invalid Input Image!";
		return 0;
	}

	vector<Vec4i> hierarchy;
	/// Find contours
	findContours(mI, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Rect> boundRect(contours.size());
	vector<Moments> mu(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		/// Find Minimum bounding Rect
		boundRect[i] = boundingRect(Mat(contours[i]));

		/// Get the moments 
		mu[i] = moments(contours[i], false);

		/// Find the Mass Centre
		MassCentre.push_back(Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00)));

		//Find Distance between the TopLeft  and Bottrom Right Points
		DiagonalLength.push_back((float)norm(boundRect[i].tl() - boundRect[i].br()));
	}
	return 1;
}

bool TransformContour(vector<vector<Point>> contours, vector<vector<Point>> &contours_Trans, Vec2f Translate)
{
	if (contours.size() == 0)
	{
		cout << "Invalid input!";
		return 0;
	}

	contours_Trans = contours;

	for (size_t i = 0; i < contours.size(); i++)
	{
		for (int idx = 0; idx < contours[i].size(); idx++)
		{
			contours_Trans[i][idx] = Point(contours[i][idx].x + Translate(0), contours[i][idx].y + Translate(1));
		}
	}
	return 1;
}

bool RotateContour(vector<vector<Point>> contours, vector<vector<Point>> &contours_Rotated, double Angle, Point2f Centre)
{
	if (contours.size() == 0)
	{
		cout << "Invalid input!";
		return 0;
	}

	Mat mRot = getRotationMatrix2D(Centre, Angle, 1.0);
	contours_Rotated = contours;

	for (size_t i = 0; i < contours.size(); i++)
	{
		for (int idx = 0; idx < contours[i].size(); idx++)
		{
			contours_Rotated[i][idx] = RotatePoint(mRot, contours[i][idx]);
		}
	}
	return 1;
}

static vector<Point> simpleContour(vector<vector<Point> > _contoursQuery, int n = 300)
{
	vector <Point> contoursQuery;
	for (size_t border = 0; border<_contoursQuery.size(); border++)
	{
		for (size_t p = 0; p<_contoursQuery[border].size(); p++)
		{
			contoursQuery.push_back(_contoursQuery[border][p]);
		}
	}

	// In case actual number of points is less than n
	int dummy = 0;
	for (int add = (int)contoursQuery.size() - 1; add<n; add++)
	{
		contoursQuery.push_back(contoursQuery[dummy++]); //adding dummy values
	}

	// Uniformly sampling
	random_shuffle(contoursQuery.begin(), contoursQuery.end());
	vector<Point> cont;
	for (int i = 0; i<n; i++)
	{
		cont.push_back(contoursQuery[i]);
	}
	return cont;
}

int main()
{
	string sFileName = "e:\\git\\opencv\\samples\\data\\shape_sample\\6.png";
	Mat mSrc;
	mSrc = imread(sFileName, 0);

	if (mSrc.empty()) {
		std::cerr << "[Error] imread() ...filename=" << sFileName << std::endl;
		return -1;
	}

	imshow("Source Image", mSrc);

	vector<vector<Point>> contours;
	vector<Point2f> MassCentre;
	vector<float>DiagonalLength;
	FindBlobs(mSrc, contours, MassCentre, DiagonalLength);

	vector<vector<Point>> contours_Trans(contours.size());
	vector<vector<Point>> contours_Rotated(contours.size());
	Point2f ptCCentre(DiagonalLength[0] / 2, DiagonalLength[0] / 2);
	Vec2f Translation(ptCCentre.x - MassCentre[0].x, ptCCentre.y - MassCentre[0].y);

	//Move the contour Mass Center to the Image Space Centre!
	TransformContour(contours, contours_Trans, Translation);

	//Just used for Visualisation
	Mat mContourSpace(Size(DiagonalLength[0], DiagonalLength[0]), CV_8UC3);

	drawContours(mContourSpace, contours_Trans, 0, Scalar(255, 0, 0), 2, 8);
	//imshow("Contour Image ",mContourSpace);

	Mat mContourMatch;
	double Angle = 45.0;
	char key;

	vector<Point> QueryContour, TestContour;
	cv::Ptr <cv::ShapeContextDistanceExtractor> mysc = cv::createShapeContextDistanceExtractor();

	QueryContour = simpleContour(contours_Trans);


	float bestMatch = 0;
	float bestDis = FLT_MAX;

	for (int i = 0; i < 8; i++)
	{
		mContourMatch = mContourSpace.clone();
		RotateContour(contours_Trans, contours_Rotated, Angle, ptCCentre);

		TestContour = simpleContour(contours_Rotated);
		float dis = mysc->computeDistance(QueryContour, TestContour);

		drawContours(mContourMatch, contours_Rotated, (int)0, Scalar(0, 255, 0), 2, 8);
		putText(mContourMatch, format("Distance: %f ", dis), Point2f(ptCCentre.x + 30, 20), CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 0.5);

		imshow("Rotated ContourImage ", mContourMatch);

		if (Angle >= 360)
			Angle = 0.0;
		else
			Angle += 45.0;

		if (dis<bestDis)
		{
			bestMatch = Angle;
			bestDis = dis;
		}

		waitKey(0);
	}

	cout << format("Angle: %f , Score: %f", bestMatch, bestDis);

	waitKey();

}