
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

struct contour_sorter // 'less' for contours
{
    bool operator ()( const vector<Point>& a, const vector<Point> & b )
    {
        Rect ra(boundingRect(a));
        Rect rb(boundingRect(b));
        // scale factor for y should be larger than img.width
        return ( (ra.x + 1000*ra.y) < (rb.x + 1000*rb.y) );
    }
};

int main()
{
vector<vector<Point> > contours(4);
contours[0].push_back(Point(3,111));
contours[0].push_back(Point(3,121));
contours[1].push_back(Point(81,13));
contours[1].push_back(Point(84,14));
contours[2].push_back(Point(33,55));
contours[2].push_back(Point(36,57));
contours[3].push_back(Point(133,25));
contours[3].push_back(Point(136,27));
for ( int i=0; i<contours.size(); i++ )
    cerr << Mat(contours[i]) << endl;

// apply it to the contours:
std::sort(contours.begin(), contours.end(), contour_sorter());

for ( int i=0; i<contours.size(); i++ )
    cerr << Mat(contours[i]) << endl;
    return 0;
}
