
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

/// Global variables
Mat src, dst, dst_resized;

// variables for customizing
// changing this values gives different results
int scale = 4;
int vertical_divide = 1;
int canny_threshold1 = 0;
int canny_threshold2 = 255;

const char* window_name = "divideHW Demo";

/** Function Headers */
void process_image( int, void* );
vector<Rect> divideHW( Mat src, int dim, double threshold1, double threshold2 );

/**
 * @function main
 */
int main( int, char** argv )
{
    /// Load an image
    src = imread( argv[1], IMREAD_COLOR );

    if( src.empty() )
    {
        return -1;
    }

    /// Create window
    namedWindow( window_name, WINDOW_NORMAL );

    /// Create Trackbar to select Vertical process
    createTrackbar("Vertical process:", window_name, &vertical_divide, 1, process_image );

    /// Create Trackbar to select scale to resize
    createTrackbar( "Scale:", window_name,
                    &scale, 10,
                    process_image );

    /// Create Trackbar to choose threshold1
    createTrackbar( "threshold1", window_name,
                    &canny_threshold1, 255,
                    process_image );

    /// Create Trackbar to choose threshold2
    createTrackbar( "threshold2", window_name,
                    &canny_threshold2, 255,
                    process_image );

    /// Default start
    process_image( 0, 0 );

    waitKey(0);
    return 0;
}

/**
 * @function process_image
 */
void process_image( int, void* )
{
    dst = src.clone();
    if( scale < 1)
    {
        scale = 1;
    }
    resize( dst, dst_resized, Size(), (double) 1 / scale, (double) 1 / scale );

    vector<Rect> rois_h = divideHW( dst_resized, 1, canny_threshold1, canny_threshold2 );

    for( size_t i=0; i< rois_h.size(); i++ )
    {
        if( vertical_divide )
        {
            Mat roi_h = dst_resized( rois_h[i]);

            vector<Rect> rois_w = divideHW( roi_h, 0, canny_threshold1, canny_threshold2 );

            for( size_t j=0; j< rois_w.size(); j++ )
            {
                rois_w[j].y += rois_h[i].y;
                rectangle( dst_resized, rois_w[j], Scalar( 0, 255, 0 ), 1 );
                rois_w[j].x = rois_w[j].x * scale;
                rois_w[j].y = rois_w[j].y * scale;
                rois_w[j].width = rois_w[j].width * scale;
                rois_w[j].height = rois_w[j].height * scale;

                rectangle( dst, rois_w[j], Scalar( 0, 255, 0 ), 3 );
            }
        }
        rectangle( dst_resized, rois_h[i], Scalar( 0, 0, 255 ), 2 );
        rois_h[i].x = rois_h[i].x * scale;
        rois_h[i].y = rois_h[i].y * scale;
        rois_h[i].width = rois_h[i].width * scale;
        rois_h[i].height = rois_h[i].height * scale;
        rectangle( dst, rois_h[i], Scalar( 0, 0, 255), 3 );
    }

    imshow( "resized", dst_resized );
    imshow( window_name, dst );
}

// helper function returns rectangles according horizontal or vertical projection of given image
// parameters:
// src source image
// dim dimension 1 for horizontal 0 for vertical projection
// threshold1 first threshold for the hysteresis procedure ( used by internal Canny )
// threshold2 second threshold for the hysteresis procedure ( used by internal Canny )

vector<Rect> divideHW( Mat src, int dim, double threshold1, double threshold2 )
{
    Mat gray, reduced, canny;

    if( src.channels() == 1 )
    {
        gray = src;
    }

    if( src.channels() == 3 )
    {
        cvtColor( src, gray, CV_BGR2GRAY );
    }

    reduce( gray, reduced, dim, REDUCE_AVG );

    //GaussianBlur( reduced, reduced, Size(),3);

    Canny( reduced, canny, threshold1, threshold2 );

    vector<Point> pts;
    findNonZero( canny, pts);

    vector<Rect> rects;

    Rect rect(0,0,gray.cols,gray.rows);
    if( !pts.size() )
    {
        rects.push_back( rect );
    }
    int ref_x = 0;
    int ref_y = 0;

    for( size_t i=0; i< pts.size(); i++ )
    {
        if( dim )
        {
            rect.height = pts[i].y-ref_y;
            rects.push_back( rect );
            rect.y = pts[i].y;
            ref_y = rect.y;
            if( i == pts.size()-1 )
            {
                rect.height = gray.rows - pts[i].y;
                rects.push_back( rect );
            }
        }

        else
        {
            rect.width = pts[i].x-ref_x;
            rects.push_back( rect );
            rect.x = pts[i].x;
            ref_x = rect.x;
            if( i == pts.size()-1 )
            {
                rect.width = gray.cols - pts[i].x;
                rects.push_back( rect );
            }
        }

    }
    return rects;
}
