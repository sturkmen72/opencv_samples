
    #include "opencv2/highgui.hpp"
    #include "opencv2/imgproc.hpp"
    #include <iostream>
    
    using namespace cv;
    using namespace std;
    
    
    int thresh = 50, N = 11;
    const char* wndname = "Square Detection Demo";
    
    // helper function:
    // finds a cosine of angle between vectors
    // from pt0->pt1 and from pt0->pt2
    static double angle( Point pt1, Point pt2, Point pt0 )
    {
        double dx1 = pt1.x - pt0.x;
        double dy1 = pt1.y - pt0.y;
        double dx2 = pt2.x - pt0.x;
        double dy2 = pt2.y - pt0.y;
        return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    }
    
    // returns sequence of squares detected on the image.
    // the sequence is stored in the specified memory storage
    
    static void findSquares( const Mat& image, vector<Rect>& squares )
    {
        squares.clear();
    	erode(image, image, Mat(), Point(-1, -1), 5);
        Mat pyr, timg, gray0(image.size(), CV_8U), gray;
    
        // down-scale and upscale the image to filter out the noise
        pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
        pyrUp(pyr, timg, image.size());
        vector<vector<Point> > contours;
    
        // find squares in every color plane of the image
        for( int c = 0; c < 3; c++ )
        {
            int ch[] = {c, 0};
            mixChannels(&timg, 1, &gray0, 1, ch, 1);
    
            // try several threshold levels
            for( int l = 0; l < N; l++ )
            {
                // hack: use Canny instead of zero threshold level.
                // Canny helps to catch squares with gradient shading
                if( l == 0 )
                {
                    // apply Canny. Take the upper threshold from slider
                    // and set the lower to 0 (which forces edges merging)
                    Canny(gray0, gray, 0, thresh, 5);
                    // dilate canny output to remove potential
                    // holes between edge segments
                    dilate(gray, gray, Mat(), Point(-1,-1));
    
                }
                else
                {
                    // apply threshold if l!=0:
                    //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                    gray = gray0 >= (l+1)*255/N;
                }
    
                // find contours and store them all as a list
                findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    
                vector<Point> approx;
    
                // test each contour
                for( size_t i = 0; i < contours.size(); i++ )
                {
                    // approximate contour with accuracy proportional
                    // to the contour perimeter
                    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
    
                    // square contours should have 4 vertices after approximation
                    // relatively large area (to filter out noisy contours)
                    // and be convex.
                    // Note: absolute value of an area is used because
                    // area may be positive or negative - in accordance with the
                    // contour orientation
                    if( approx.size() == 4 &&
                        fabs(contourArea(Mat(approx))) > 1000 &&
                        isContourConvex(Mat(approx)) )
                    {
                        double maxCosine = 0;
    
                        for( int j = 2; j < 5; j++ )
                        {
                            // find the maximum cosine of the angle between joint edges
                            double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                            maxCosine = MAX(maxCosine, cosine);
                        }
    
                        // if cosines of all angles are small
                        // (all angles are ~90 degree) then write quandrange
                        // vertices to resultant sequence
    					if (maxCosine < 0.3)
    					{
    						Rect r = boundingRect(approx);
    						if(r.width < (image.cols * 0.8))
    						squares.push_back( r );
    					}
                    }
                }
            }
        }
    }
    
    
    // the function draws all the squares in the image
    static void drawSquares( Mat& image, const vector<Rect>& squares )
    {
        for( size_t i = 0; i < squares.size(); i++ )
        {
            rectangle(image, squares[i],Scalar(0,255,0), 1, LINE_AA);
        }
    
        imshow(wndname, image);
    }
    
    
    int main(int argc, char** argv)
    {
    
        namedWindow( wndname, 1 );
        vector<Rect> squares;
    
            Mat image = imread(argv[1], 1);
            if( image.empty() )
            {
                cout << "Couldn't load " << argv[1] << endl;
    			return -1;
            }
    
    		findSquares(image, squares);
            drawSquares(image, squares);
    
            waitKey();
    
        return 0;
    }
