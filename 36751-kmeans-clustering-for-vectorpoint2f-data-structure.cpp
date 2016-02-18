
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
vector<Point2f> points;
points.push_back(Point2f(1,1));
points.push_back(Point2f(3,1));
points.push_back(Point2f(1,5));
points.push_back(Point2f(2,5));

Mat labels,centers;
int K=2, attempts=3, flags=cv::KMEANS_RANDOM_CENTERS; // hey, just guessing here
TermCriteria tc;

kmeans(points,K,labels,tc,attempts,flags, centers);   // points will get cast implicitly to Mat

for ( int i=0; i<labels.rows; i++ )
{
    int idx = labels.at<int>(i);
    Point2f original_point = points[i];
    Point2f clustered_center;
    clustered_center.x = centers.at<float>( idx,0 );
    clustered_center.y = centers.at<float>( idx,1 );
    cerr << i << " " << idx << " " << original_point << " " << clustered_center << endl;
}

    return 0;
}
