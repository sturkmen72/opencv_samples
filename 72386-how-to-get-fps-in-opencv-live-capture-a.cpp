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

int main(int argc, char**argv)
{
    cv::VideoCapture capture;

    // Read the video stream or open the web cam
    if( argc > 1)
        capture.open( argv[1] );
    else
        capture.open( 0 );

    if (!capture.isOpened())
    {
        std::cout << "Problem connecting to cam " << std::endl;
        return -1;
    }
    else
    if( argc == 1 )
    {
        std::cout << "Successfuly connected to camera " << std::endl;
        //capture.set(cv::CAP_PROP_FRAME_WIDTH,320);
        //capture.set(cv::CAP_PROP_FRAME_HEIGHT,240);
    }

    cv::Mat frame;
    int c;
    TickMeter tm;

    do
    {
        tm.stop();
        tm.start();
        capture.read(frame);

        if( frame.empty() )
        {
            break;
        }

        if(tm.getCounter() > 0)
        {
            cv::putText(frame, cv::format("Average FPS=%d",cvRound( tm.getCounter()/tm.getTimeSec())), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));
        }

        cv::imshow("Video Capture Test", frame);

        c = cv::waitKey(1);
    }
    while(c != 27);

    return 0;
}
