#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
    vector<String> filenames;
    String folder = "./*.jpg";

    glob(folder, filenames);

    for(size_t i = 0; i < filenames.size(); ++i)
    {
        Mat src = imread(filenames[i]);

        if(src.data)
        {
            imshow( "glob test", src );
            waitKey(3000);
        }
    }
    return 0;
}
