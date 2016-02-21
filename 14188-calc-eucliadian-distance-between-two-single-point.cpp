#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class TickMeter
{
public:
    TickMeter();
    void start();
    void stop();

    int64 getTimeTicks() const;
    double getTimeMicro() const;
    double getTimeMilli() const;
    double getTimeSec()   const;
    int64 getCounter() const;

    void reset();
private:
    int64 counter;
    int64 sumTime;
    int64 startTime;
};

TickMeter::TickMeter()
{
    reset();
}
int64 TickMeter::getTimeTicks() const
{
    return sumTime;
}
double TickMeter::getTimeMicro() const
{
    return  getTimeMilli()*1e3;
}
double TickMeter::getTimeMilli() const
{
    return getTimeSec()*1e3;
}
double TickMeter::getTimeSec() const
{
    return (double)getTimeTicks()/cv::getTickFrequency();
}
int64 TickMeter::getCounter() const
{
    return counter;
}
void TickMeter::reset()
{
    startTime = 0;
    sumTime = 0;
    counter = 0;
}

void TickMeter::start()
{
    startTime = cv::getTickCount();
}
void TickMeter::stop()
{
    int64 time = cv::getTickCount();
    if ( startTime == 0 )
        return;
    ++counter;
    sumTime += ( time - startTime );
    startTime = 0;
}

float euclideanDist(cv::Point2f& p, cv::Point2f& q)
{
    cv::Point diff = p - q;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

static double distanceBtwPoints(const cv::Point2f &a, const cv::Point2f &b)
{
    double xDiff = a.x - b.x;
    double yDiff = a.y - b.y;

    return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

int main(int argc, char**argv)
{
    cv::Mat img(500,500,CV_8UC3,cv::Scalar(0,0,0));
    cv::Point2f a(0,0);
    cv::Point2f b(15000,15000);

    std::cout << "euclideanDist : " << euclideanDist( a, b ) << std::endl;
    std::cout << "distanceBtwPoints : " << distanceBtwPoints( a, b ) << std::endl;
    std::cout << "cv::norm : " << cv::norm( a - b ) << std::endl;

    cv::RNG& rng = cv::theRNG();
    std::vector<cv::Point2f> points;

    for( int i = 0; i < 15000; i++ )
    {
        cv::Point pt;
        pt.x = rng.uniform(0, 15000);
        pt.y = rng.uniform(0, 15000);
        points.push_back(pt);
    }

    cv::Point2f a_, b_;
    double distance;
    double max_distance = 0;

    TickMeter tm;

    tm.start();
    for( int i = 0; i < 14999; i++ )
    {
        for( int j = i+1; j < 15000; j++ )
        {
            a = points[i];
            b = points[j];

            distance = euclideanDist( a, b );
            if( distance > max_distance )
            {
                max_distance = distance;
                a_ = a;
                b_ = b;
            }
        }
    }

    tm.stop();
    std::cout << "max_distance euclideanDist : " << euclideanDist( a_, b_ ) << " time passed :" <<tm.getTimeSec() << std::endl;

    tm.reset();
    tm.start();
    for( int i = 0; i < 14999; i++ )
    {
        for( int j = i+1; j < 15000; j++ )
        {
            a = points[i];
            b = points[j];

            distance = distanceBtwPoints( a, b );
            if( distance > max_distance )
            {
                max_distance = distance;
                a_ = a;
                b_ = b;
            }
        }
    }

    tm.stop();
    std::cout << "max_distance distanceBtwPoints : " << distanceBtwPoints( a_, b_ ) << " time passed :" <<tm.getTimeSec() << std::endl;

    tm.reset();
    tm.start();
    for( int i = 0; i < 14999; i++ )
    {
        for( int j = i+1; j < 15000; j++ )
        {
            a = points[i];
            b = points[j];

            distance = cv::norm( a - b );
            if( distance > max_distance )
            {
                max_distance = distance;
                a_ = a;
                b_ = b;
            }
        }
    }

    tm.stop();
    std::cout << "max_distance cv::norm : " << cv::norm( a_ - b_ ) << " time passed :" <<tm.getTimeSec() << std::endl;

    return 0;
}
