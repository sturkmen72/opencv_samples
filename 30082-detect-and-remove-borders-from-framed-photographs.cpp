#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

Rect deleteBorder(InputArray _src,int size){
    Mat src = _src.getMat();
    Mat sbl_x, sbl_y;
    int ksize = 2 * size + 1;
    Sobel(src, sbl_x, CV_32FC1, 2, 0, ksize);
    Sobel(src, sbl_y, CV_32FC1, 0, 2, ksize);
    Mat sum_img = sbl_x + sbl_y;

    Mat gray;
    normalize(sum_img, gray, 0, 255, CV_MINMAX, CV_8UC1);

    Mat row_proj, col_proj;
    reduce(gray, row_proj, 1, CV_REDUCE_AVG, CV_8UC1);
    reduce(gray, col_proj, 0, CV_REDUCE_AVG, CV_8UC1);
    Sobel(row_proj, row_proj, CV_8UC1, 0, 2, 3);
    Sobel(col_proj, col_proj, CV_8UC1, 2, 0, 3);


    Point peak_pos;
    int half_pos = row_proj.total() / 2;
    Rect result;

    minMaxLoc(row_proj(Range(0,half_pos), Range(0, 1)), 0, 0, 0, &peak_pos);
    result.y = peak_pos.y;
    minMaxLoc(row_proj(Range(half_pos, row_proj.total()), Range(0, 1)), 0, 0, 0, &peak_pos);
    result.height = (peak_pos.y + half_pos - result.y);

    half_pos = col_proj.total() / 2;
    minMaxLoc(col_proj(Range(0, 1), Range(0, half_pos)), 0, 0, 0, &peak_pos);
    result.x = peak_pos.x;
    minMaxLoc(col_proj(Range(0, 1), Range(half_pos, col_proj.total())), 0, 0, 0, &peak_pos);
    result.width = (peak_pos.x + half_pos - result.x);

    return result;
}

int main( int argc, const char** argv )
{
Mat img = imread(argv[1]);
Mat gray_img;
cvtColor(img, gray_img, CV_BGR2GRAY);
Rect r = deleteBorder(gray_img, 2);
Mat color_img;

rectangle(img, r, Scalar(0, 0, 255), 2);
imshow("result", img);
waitKey(0);

return 0;
}
