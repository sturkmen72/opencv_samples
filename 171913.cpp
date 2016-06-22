#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define EPSILON 1E-5

//! Return the maximum of the provided numbers
static double maximum(double number1, double number2, double number3) {
	return std::max(std::max(number1, number2), number3);
}

//! Check if the two numbers are equal (almost)
/*!
* The expression for determining if two real numbers are equal is:
* if (Abs(x - y) <= EPSILON * Max(1.0f, Abs(x), Abs(y))).
*
* @param number1 First number
* @param number2 Second number
*/
static bool almostEqual(double number1, double number2) {
	return (std::abs(number1 - number2) <= (EPSILON * maximum(1.0, std::abs(number1), std::abs(number2))));
}

//! Determine the intersection point of two lines, if this point exists
/*! Two lines intersect if they are not parallel (Parallel lines intersect at
* +/- infinity, but we do not consider this case here).
*
* The lines are specified by a pair of points each. If they intersect, then
* the function returns true, else it returns false.
*
* Lines can be specified in the following form:
*      A1x + B1x = C1
*      A2x + B2x = C2
*
* If det (= A1*B2 - A2*B1) == 0, then lines are parallel
*                                else they intersect
*
* If they intersect, then let us denote the intersection point with P(x, y) where:
*      x = (C1*B2 - C2*B1) / (det)
*      y = (C2*A1 - C1*A2) / (det)
*
* @param a1 First point for determining the first line
* @param b1 Second point for determining the first line
* @param a2 First point for determining the second line
* @param b2 Second point for determining the second line
* @param intersection The intersection point, if this point exists
*/
static bool lineIntersection(const cv::Point2f &a1, const cv::Point2f &b1, const cv::Point2f &a2,
	const cv::Point2f &b2, cv::Point2f &intersection) {
	double A1 = b1.y - a1.y;
	double B1 = a1.x - b1.x;
	double C1 = (a1.x * A1) + (a1.y * B1);

	double A2 = b2.y - a2.y;
	double B2 = a2.x - b2.x;
	double C2 = (a2.x * A2) + (a2.y * B2);

	double det = (A1 * B2) - (A2 * B1);

	if (!almostEqual(det, 0)) {
		intersection.x = static_cast<float>(((C1 * B2) - (C2 * B1)) / (det));
		intersection.y = static_cast<float>(((C2 * A1) - (C1 * A2)) / (det));

		return true;
	}

	return false;
}

struct vector_sorter
{
	bool operator ()(const std::vector<cv::Point>& a, const std::vector<cv::Point> & b)
	{
		double dist_a = norm(a[0] - a[1]);
		double dist_b = norm(b[0] - b[1]);
		return dist_a > dist_b;
	}
};

void sortCorners(std::vector<cv::Point2f>& corners)
{
	std::vector<cv::Point2f> top, bot;
	cv::Point2f center;
	// Get mass center
	for (int i = 0; i < corners.size(); i++)
		center += corners[i];
	center *= (1. / corners.size());

	for (int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}
	corners.clear();

	if (top.size() == 2 && bot.size() == 2) {
		cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
		cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
		cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
		cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

		corners.push_back(tl);
		corners.push_back(tr);
		corners.push_back(br);
		corners.push_back(bl);
	}
}


int main(int argc, char** argv)
{
	bool showsteps = true; // set it to false to see only result; 
	Mat src, src_copy,edges;
	src = imread(argv[1]);
	if (src.empty())
	{
		src = Mat(400, 400, CV_8UC3, Scalar(127, 127, 127));
		rectangle(src, Rect(20, 200, 150, 50), Scalar(0, 0, 255), 8);
		rectangle(src, Rect(200, 200, 50, 50), Scalar(0, 0, 255), 8);
	}
	src_copy = src.clone();

	cvtColor(src, edges, COLOR_BGR2GRAY);
	GaussianBlur(edges, edges, Size(5, 5), 1.5, 1.5);

	erode(edges, edges, Mat());// these lines may need to be optimized 
	dilate(edges, edges, Mat());
	dilate(edges, edges, Mat());
	erode(edges, edges, Mat());

	Canny(edges, edges, 50, 150, 3); // canny parameters may need to be optimized 
	
	if (showsteps) imshow("edges", edges);

	cv::waitKey(1); // add this line

	vector<Point> selected_points;
	vector<vector<Point> > contours;

	std::cout << "1. findcontours\n"; // add this line

	findContours(edges, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	std::cout << "1. findcontours...OK\n"; // add this line

	for (size_t i = 0; i < contours.size(); i++)
	{
		Rect minRect = boundingRect(contours[i]);

		if ((minRect.width > src.cols / 6) | (minRect.height > src.rows / 6)) // this line also need to be optimized 
		{
			selected_points.insert(selected_points.end(), contours[i].begin(), contours[i].end());

			if (showsteps)
			{
				drawContours(src_copy, contours, i, Scalar(0, 0, 255), 3);
			}
		}
	}
	
	if (showsteps) imshow("Selected contours", src_copy );
	waitKey(1);
	vector<Point2f> selected_points_f;
	vector<Point2f> corners;
	Mat(selected_points).convertTo(selected_points_f, CV_32F);
	Mat hull;
	convexHull(selected_points_f, hull, true, true);

	RotatedRect RRect = minAreaRect(hull);
	std::vector<cv::Point2f> RR_corners;
	Point2f four_points[4];
	RRect.points(four_points);
	RR_corners.push_back(four_points[0]);
	RR_corners.push_back(four_points[1]);
	RR_corners.push_back(four_points[2]);
	RR_corners.push_back(four_points[3]);

	for (int j = 0; j < 4; j++)
	{
		Point2f pt = RR_corners[j];
		Point2f nearest_pt = hull.at<Point2f>(j, 0);
		float dist = norm(pt - nearest_pt);
		for (int k = 1; k < hull.rows; k++)
		{
			Point2f hull_point = hull.at<Point2f>(k, 0);
			if (norm(pt - hull_point) < dist)
			{
				dist = norm(pt - hull_point);
				nearest_pt = hull_point;
			}
		}
		corners.push_back(nearest_pt);
	}
	sortCorners(corners);

	Mat(corners).convertTo(selected_points, CV_32S);

	Rect r = boundingRect(corners);
	cv::Mat quad = cv::Mat::zeros(norm(corners[1] - corners[2]), norm(corners[2] - corners[3]), CV_8UC3);

	std::vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
	quad_pts.push_back(cv::Point2f(0, quad.rows));

	cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
	cv::warpPerspective(src, quad, transmtx, quad.size());
	
	if (showsteps)
	{
		src_copy = src.clone();
		polylines(src_copy, selected_points, true, Scalar(0, 0, 255), 3);
		imshow("selected quadrilateral part", src_copy);
	}

	imshow("Result Image", quad);
	waitKey(0);

	return 0;
}
