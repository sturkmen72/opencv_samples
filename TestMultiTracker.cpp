#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/bgsegm.hpp"
#include <opencv2/tracking.hpp>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace cv;
using namespace std;


    class AssignmentProblemSolver
    {
    private:
        // --------------------------------------------------------------------------
        // Computes the optimal assignment (minimum overall costs) using Munkres algorithm.
        // --------------------------------------------------------------------------
        void assignmentoptimal(int* assignment, double* cost, double* distMatrix, int nOfRows, int nOfColumns);
        void buildassignmentvector(int* assignment, bool* starMatrix, int nOfRows, int nOfColumns);
        void computeassignmentcost(int* assignment, double* cost, double* distMatrix, int nOfRows);
        void step2a(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim);
        void step2b(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim);
        void step3(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim);
        void step4(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim, int row, int col);
        void step5(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim);
        // --------------------------------------------------------------------------
        // Computes a suboptimal solution. Good for cases with many forbidden assignments.
        // --------------------------------------------------------------------------
        void assignmentsuboptimal1(int* assignment, double* cost, double* distMatrixIn, int nOfRows, int nOfColumns);
        // --------------------------------------------------------------------------
        // Computes a suboptimal solution. Good for cases with many forbidden assignments.
        // --------------------------------------------------------------------------
        void assignmentsuboptimal2(int* assignment, double* cost, double* distMatrixIn, int nOfRows, int nOfColumns);
    public:
        enum TMethod { optimal, many_forbidden_assignments, without_forbidden_assignments };
        AssignmentProblemSolver();
        ~AssignmentProblemSolver();
        double Solve(std::vector<std::vector<double> >& DistMatrix, std::vector<int>& Assignment, TMethod Method = optimal);
    };

    class TKalmanFilter
    {
    public:
        KalmanFilter* kalman;
        double deltatime;
        Point2f LastResult;
        TKalmanFilter(Point2f p, float dt = 0.2, float Accel_noise_mag = 0.5);
        ~TKalmanFilter();
        Point2f GetPrediction();
        Point2f Update(Point2f p, bool DataCorrect);
    };



    class CTrack
    {
    public:
        std::vector<Point2d> trace;
        static size_t NextTrackID;
        std::vector<Rect> detected_object_rect;
        size_t detected_object_id;
        size_t track_id;
        size_t skipped_frames;
        Point2d prediction;
        TKalmanFilter* KF;
        CTrack(Point2f p, float dt, float Accel_noise_mag);
        ~CTrack();
    };


    class CTracker
    {
    public:

        float dt;
        float Accel_noise_mag;
        double dist_thres;
        int maximum_allowed_skipped_frames;
        int max_trace_length;

        std::vector<CTrack*> tracks;
        void Update(std::vector<Point2d>& detections);
        CTracker(float _dt, float _Accel_noise_mag, double _dist_thres = 60, int _maximum_allowed_skipped_frames = 10, int _max_trace_length = 10);
        ~CTracker(void);
    };
    //

    struct TestMultiTrackerVisParams
    {
        bool drawMotionRects = true;
        Scalar MotionRectsColor = Scalar(0, 100, 200);
        int MotionRectsPenSize = 3;
        bool drawDetections = true;
        int DetectionsPenSize = 3;
        bool drawMotionHistory = true;
        int MotionHistoryPenSize = 3;
        int showDetectionID = 1;
    };

    class CV_EXPORTS_W TestMultiTracker
    {
    public:

        CV_WRAP TestMultiTracker(const String& cascade, const String& TrackingAlg, const String& HumanDetector_model_file_path);
        ~TestMultiTracker();

        CV_WRAP void setHOGDescriptor(HOGDescriptor _HOGDescriptor);
        CV_WRAP void setCascadeClassifier(CascadeClassifier _CascadeClassifier);
        CV_WRAP void setBackgroundSubtractor(Ptr<BackgroundSubtractorMOG2> _BackgroundSubtractor);
        CV_WRAP bool init(const Mat& image);
        CV_WRAP bool update(Mat& image);
        CV_WRAP std::vector<Rect> findObjects(Mat& image);
        CV_WRAP int setDebug(int Value);
        CV_WRAP bool setPedestrianDetection(bool Value);
        CV_WRAP bool setHumanDetection(bool Value);
        CV_WRAP bool setCarDetection(bool Value);
        CV_WRAP int setScale(int Value);
        CV_WRAP int setUpscaleValue(int Value);
        CV_WRAP int setMaxTrackedCount(int Value);
        CV_WRAP Size setMinTrackedSize(Size Value);
        CV_WRAP TestMultiTrackerVisParams setVisParams(TestMultiTrackerVisParams Value);
        CV_WRAP bool loadVisParams(const String& file_name);
        CV_WRAP bool saveVisParams(const String& file_name);
        CV_WRAP bool getObjects(CV_OUT std::vector<Rect>& obj_rects, CV_OUT std::vector<int>& obj_ids, CV_OUT std::vector<int>& obj_type, CV_OUT std::vector<std::vector<Point> >& obj_history);
        std::vector<Rect> objects;
        std::vector<int> objects_type;
        std::vector<int> objects_id;
        std::vector<std::vector<Point> > objects_history;
    protected:
        TestMultiTrackerVisParams visparams;
        CTracker* _CTracker;
        MultiTracker _MultiTracker;
        //!<  storage for the objects..

        std::vector<Point2d> centers;
        Mat bgs_output;
        HOGDescriptor _HOGDescriptor;
        CascadeClassifier _CascadeClassifier;
        Ptr<BackgroundSubtractorMOG2> _BackgroundSubtractor;
        bool isHOGDescriptorDefined;
        bool isCascadeClassifierrDefined;
        bool isBackgroundSubtractorDefined;
        int ShowDebugWindow;
        int DebugWaitTime;
        int scale;
        int frame_counter;
        bool PedestrianDetection;
        bool HumanDetection;
        bool CarDetection;
        int maxTrackedCount;
        Size minTrackedSize;
        String _HumanDetector_model_file_path;
        bool isNotTrackedObject(Rect r);
        int detected_object_counter;
        int upscale_value;
    };



    Scalar Colors[] = { Scalar(255,0,0),Scalar(0,255,0),Scalar(0,0,255),Scalar(255,255,0),Scalar(0,255,255),Scalar(255,0,255),
                        Scalar(127,0,0),Scalar(0,127,0),Scalar(0,0,127),Scalar(127,127,0),Scalar(0,127,127),Scalar(127,0,127) };
    Rect shrinkRect(Rect rect, int width_percent, int height_percent)
    {
        if (width_percent > 100) width_percent = 100;
        if (height_percent > 100) height_percent = 100;

        Rect newrect;
        newrect.width = (rect.width * width_percent) / 100;
        newrect.height = (rect.height * height_percent) / 100;
        newrect.x = rect.x + (rect.width - newrect.width) / 2;
        newrect.y = rect.y + (rect.height - newrect.height) / 2;

        return newrect;
    }

    Rect expandRect(Rect rect, int width_percent, int height_percent)
    {
        // not tested strongly
        Rect newrect;
        newrect.width = rect.width + ((rect.width * width_percent) / 100);
        newrect.height = rect.height + ((rect.height * height_percent) / 100);
        newrect.x = rect.x + (rect.width - newrect.width) / 2;
        newrect.y = rect.y + (rect.height - newrect.height) / 2;

        return newrect;
    }

    TestMultiTracker::TestMultiTracker(const String& cascade, const String& TrackingAlg, const String& HumanDetector_model_file_path)
    {
        _HumanDetector_model_file_path = HumanDetector_model_file_path;
        _CTracker = new CTracker(0.2, 0.5, 60.0, 20, 20);
        isHOGDescriptorDefined = false;
        isCascadeClassifierrDefined = false;
        isBackgroundSubtractorDefined = false;
        ShowDebugWindow = false;
        PedestrianDetection = true;
        HumanDetection = false;
        CarDetection = true;
        scale = 1;
        upscale_value = 0;
        frame_counter = 0;
        detected_object_counter = 0;
        maxTrackedCount = 20;
        _CascadeClassifier = CascadeClassifier();
        //_MultiTracker = MultiTracker(TrackingAlg);
        if (cascade != "")
        {
            _CascadeClassifier.load(cascade);
            isCascadeClassifierrDefined = true;
        }
    }

    TestMultiTracker::~TestMultiTracker()
    {
    }

    void TestMultiTracker::setHOGDescriptor(HOGDescriptor HOGDescriptor_)
    {
        _HOGDescriptor = HOGDescriptor_;
        isHOGDescriptorDefined = true;
    }

    void TestMultiTracker::setCascadeClassifier(CascadeClassifier CascadeClassifier_)
    {
        _CascadeClassifier = CascadeClassifier_;
        isCascadeClassifierrDefined = true;
    }

    void TestMultiTracker::setBackgroundSubtractor(Ptr<BackgroundSubtractorMOG2> BackgroundSubtractor_)
    {
        _BackgroundSubtractor = BackgroundSubtractor_;
        isBackgroundSubtractorDefined = true;
    }

    bool TestMultiTracker::init(const Mat& image)
    {

        if (!isBackgroundSubtractorDefined)
        {
            _BackgroundSubtractor = createBackgroundSubtractorMOG2();
            _BackgroundSubtractor->setDetectShadows(true);
            _BackgroundSubtractor->setShadowValue(255);
            isBackgroundSubtractorDefined = true;
        }

        if (!isHOGDescriptorDefined)
        {
            _HOGDescriptor = HOGDescriptor();
            _HOGDescriptor.setSVMDetector(_HOGDescriptor.getDefaultPeopleDetector());
            //_HumanDetector = HumanDetector();
            //_HumanDetector.load(_HumanDetector_model_file_path);
        }

        std::vector<Rect> rects;
        _HOGDescriptor.detectMultiScale(image, rects);
        //_HumanDetector.detectMultiScale(image, rects);

        //for (size_t i = 0; i<rects.size(); i++)
        //{
        //	objects.push_back(rects[i]);
        //}
        if (isCascadeClassifierrDefined)
        {
            std::vector<Rect> rects;
            _CascadeClassifier.detectMultiScale(image, rects, 1.1, 3, 0, Size(image.cols / 2, image.cols / 2));

            for (size_t i = 0; i < rects.size(); i++)
            {
                // objects.push_back(rects[i]);
                // _MultiTracker.add(image, rects[i]);
            }
        }

        _BackgroundSubtractor->apply(image, bgs_output);

        return isBackgroundSubtractorDefined;
    }

    Scalar swpScalar(Scalar color)
    {
        Scalar newScalar = Scalar(color(2) / 2, 0, color(0) / 3);
        newScalar = Scalar(255, 255, 255) - color;
        return newScalar;

    }

    bool TestMultiTracker::update(Mat& image)
    {
        frame_counter++;
        //_MultiTracker.update(image, _MultiTracker.objects);
        Mat resized;
        resize(image, resized, Size(), (double)1 / scale, (double)1 / scale);
        if (!isBackgroundSubtractorDefined)
        {
            init(resized);
        }
        else
        {
            _BackgroundSubtractor->apply(resized, bgs_output);
            Mat output;
            Mat image_dbg;
            if (ShowDebugWindow)
            {
                image_dbg = image.clone();
            }
            //dilate(bgs_output, output, Mat());
            erode(bgs_output, output, Mat(), Point(-1, -1), 1);
            dilate(output, output, Mat(), Point(-1, -1), 2);

            if (ShowDebugWindow > 1)
            {
                imshow("bgs_output", output);
                Mat bg;
                _BackgroundSubtractor->getBackgroundImage(bg);
                imshow("bg", bg);
                Mat gray;
                std::vector<Rect> detected_rects;

                cvtColor(image, gray, COLOR_BGR2GRAY);
                //TM.start();
                _HOGDescriptor.detectMultiScale(gray, detected_rects);
                if (frame_counter % 10 == 11)
                    //_HumanDetector.detectMultiScale(gray, detected_rects);
                    //TM.stop();
                    //cout << TM.getTimeSec() << endl;
                    for (size_t i = 0; i < detected_rects.size(); i++)
                    {
                        line(image_dbg, detected_rects[i].tl(), detected_rects[i].br(), Scalar(0, 0, 255), 4);
                    }
            }

            // Find contours
            std::vector<std::vector<Point> > contours;
            findContours(output, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            std::vector<Rect> motion_rects;
            std::vector<Rect> all_detected_rects;
            centers.clear();
            for (size_t i = 0; i < contours.size(); i++)
            {
                Rect r = boundingRect(contours[i]);
                r.x *= scale;
                r.y *= scale;
                r.width *= scale;
                r.height *= scale;
                //rectangle(image_dbg, r, Scalar(0, 0, 255), 5);
                r = expandRect(r, 130, 130);
                r = r & Rect(0, 0, image.cols, image.rows);

                if (PedestrianDetection & (r.height > minTrackedSize.width& r.width < r.height)& isNotTrackedObject(r))
                    motion_rects.push_back(r);
                if (CarDetection & (r.width > minTrackedSize.width& abs(r.width - r.height) < r.width * 0.2)& isNotTrackedObject(r))
                    motion_rects.push_back(r);

                motion_rects.push_back(r);
            }

            groupRectangles(motion_rects, 1, 0.2);

            for (size_t i = 0; i < motion_rects.size(); i++)
            {
                Rect motion_rect = motion_rects[i];

                if (PedestrianDetection/* & isNotTrackedObject(motion_rect)*/)
                {
                    Mat roi;
                    std::vector<Rect> detected_rects;

                    if (motion_rect.width > _HOGDescriptor.winSize.width& motion_rect.height > _HOGDescriptor.winSize.height)
                    {
                        if (ShowDebugWindow)
                        {
                            //	rectangle(image_dbg, motion_rect, Scalar(0, 0, 255), 2);
                        }

                        cvtColor(image(motion_rect), roi, COLOR_BGR2GRAY);
                        //imshow(format("roi%d",i), roi);
                        //waitKey();
                        _HOGDescriptor.detectMultiScale(roi, detected_rects);
                        //_HumanDetector.detectMultiScale(roi, detected_rects);
                    }
                    else
                    {

                        if (motion_rect.width * upscale_value > _HOGDescriptor.winSize.width& motion_rect.height* upscale_value > _HOGDescriptor.winSize.height)
                        {
                            cvtColor(image(motion_rect), roi, COLOR_BGR2GRAY);
                            resize(roi, roi, Size(), upscale_value, upscale_value);
                            _HOGDescriptor.detectMultiScale(roi, detected_rects);
                            //cout << motion_rect.width << "  " << motion_rect.height << endl;
                            //imshow(format("roi%d", i), roi);
                            //waitKey();

                            for (size_t i = 0; i < detected_rects.size(); i++)
                            {
                                detected_rects[i].x = (detected_rects[i].x / upscale_value) + motion_rect.x;
                                detected_rects[i].y = (detected_rects[i].y / upscale_value) + motion_rect.y;
                                detected_rects[i].width = (detected_rects[i].width / upscale_value);
                                detected_rects[i].height = (detected_rects[i].height / upscale_value);
                                all_detected_rects.push_back(expandRect(detected_rects[i], upscale_value * 15, upscale_value * 15));
                            }
                        }

                    }
                    for (size_t i = 0; i < detected_rects.size(); i++)
                    {
                        detected_rects[i].x += motion_rect.x;
                        detected_rects[i].y += motion_rect.y;
                       // if (_MultiTracker.objects.size() < maxTrackedCount)
                           // _MultiTracker.add(image, detected_rects[i]);// objects.push_back(rects[i]);
                            //detected_objects.push_back(rects[i]);
                        all_detected_rects.push_back(detected_rects[i]);
                        //rectangle(image_dbg, detected_rects[i], Scalar(0, 0, 255),5);
                        //centers.push_back((detected_rects[i].br() + detected_rects[i].tl())*0.5);
                    }
                }


                if (CarDetection & (motion_rect.width > minTrackedSize.width& abs(motion_rect.width - motion_rect.height) < motion_rect.width * 0.2)& isNotTrackedObject(motion_rect))
                {
                    Mat roi;

                    std::vector<Rect> detected_rects;

                    cvtColor(image, roi, COLOR_BGR2GRAY);
                    _CascadeClassifier.detectMultiScale(roi, detected_rects, 1.1, 3, 0, Size(image.cols / 10, image.cols / 10));
                    for (size_t i = 0; i < detected_rects.size(); i++)
                    {
                        rectangle(image_dbg, detected_rects[i], Scalar(255, 255, 255), 4);
                        detected_rects[i].x += motion_rect.x;
                        detected_rects[i].y += motion_rect.y;
                        if (detected_rects[i].width > minTrackedSize.width&
                            detected_rects[i].height > minTrackedSize.height/*&
                            _MultiTracker.objects.size() < maxTrackedCount*/)

                            //_MultiTracker.add(image, detected_rects[i]);// objects.push_back(rects[i]);
                        //detected_objects.push_back(detected_rects[i]);
                        //if (ShowDebugWindow)
                        {
                            rectangle(image_dbg, detected_rects[i], Scalar(255, 255, 255), 4);
                        }
                    }
                }
            }

           /* if (ShowDebugWindow)
            {
                for (size_t i = 0; i < _MultiTracker.objects.size(); i++)
                {

                    rectangle(image_dbg, _MultiTracker.objects[i], Scalar(0, 0, 255), 5);
                }
            }*/

            for (size_t i = 0; i < motion_rects.size(); i++)
            {
                centers.push_back((motion_rects[i].br() + motion_rects[i].tl()) * 0.5);

                if (visparams.drawMotionRects)
                {
                    rectangle(image, shrinkRect(motion_rects[i], 75, 75), visparams.MotionRectsColor, visparams.MotionRectsPenSize);
                    rectangle(image_dbg, shrinkRect(motion_rects[i], 75, 75), visparams.MotionRectsColor, visparams.MotionRectsPenSize);
                }
            }

            if (centers.size() > 0)
            {
                _CTracker->Update(centers);

                for (int i = 0; i < _CTracker->tracks.size(); i++)
                {
                    if (_CTracker->tracks[i]->trace.size() > 1)
                    {
                        for (int j = 0; j < _CTracker->tracks[i]->trace.size() - 1; j++)
                        {
                            if (visparams.drawMotionHistory)
                            {
                                Scalar clr = Colors[_CTracker->tracks[i]->track_id % 12];
                                Rect r(_CTracker->tracks[i]->trace[j].x - 2, _CTracker->tracks[i]->trace[j].y - 2, (j) / 2, (j) / 2);
                                rectangle(image_dbg, r, clr, 2);
                                rectangle(image, r, clr, 2);
                                //line(image_dbg, _CTracker->tracks[i]->trace[j], _CTracker->tracks[i]->trace[j + 1], clr, visparams.MotionHistoryPenSize, LINE_AA);
                                //line(image, _CTracker->tracks[i]->trace[j], _CTracker->tracks[i]->trace[j + 1], clr, visparams.MotionHistoryPenSize, LINE_AA);
                            }
                        }

                        //for (int obj = 0; obj<objects.size(); obj++)
                        //{
                        //	Point2d pt = (objects[obj].br() + objects[obj].tl())*0.5;
                        //	cout << pt << std::endl;
                        //	cout <<  _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1] << std::endl;
                        //	if (pt == _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1])
                        //{
                        //	rectangle(image_dbg, objects[obj], Colors[obj % 9], 1 );
                        //}
                        //} 
                        //circle(image_dbg, _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1], 30, Colors[_CTracker->tracks[i]->track_id % 9], 3);
                        for (int k = 0; k < all_detected_rects.size(); k++)
                        {
                            Rect r = shrinkRect(all_detected_rects[k], 30, 30);
                            Rect center_rect;
                            center_rect.x = _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1].x;
                            center_rect.y = _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1].y;
                            center_rect.width = 1;
                            center_rect.height = 1;

                            if ((r & center_rect) == center_rect)
                            {
                                if (_CTracker->tracks[i]->detected_object_rect.size() == 0)
                                {
                                    detected_object_counter++;
                                    _CTracker->tracks[i]->detected_object_id = detected_object_counter;
                                }
                                _CTracker->tracks[i]->detected_object_rect.push_back(all_detected_rects[k]);
                            }
                            //rectangle(image_dbg, r, Scalar(0, 0, 255), 5);

                        }

                    }
                }

                objects.clear();
                objects_id.clear();
                objects_type.clear();
                objects_history.clear();
                for (int i = 0; i < _CTracker->tracks.size(); i++)
                {
                    if (_CTracker->tracks[i]->detected_object_rect.size() > 0)
                    {
                        Rect r;
                        r.x = _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1].x;
                        r.y = _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1].y;
                        r.width = _CTracker->tracks[i]->detected_object_rect[0].width;
                        r.height = _CTracker->tracks[i]->detected_object_rect[0].height;
                        r.x -= r.width / 2;
                        r.y -= r.height / 2;

                        Rect cnt_rect = shrinkRect(r, 50, 50);
                        cnt_rect.x = cnt_rect.x / scale;
                        cnt_rect.y = cnt_rect.y / scale;
                        cnt_rect.width = cnt_rect.width / scale;
                        cnt_rect.height = cnt_rect.height / scale;
                        cnt_rect = cnt_rect & Rect(0, 0, output.cols, output.rows);

                        int nonZeroPixels = 0;

                        if (cnt_rect.area() > 10)
                            nonZeroPixels = countNonZero(output(cnt_rect));

                        if (cnt_rect.area() / 10 < nonZeroPixels)
                        {
                            int trackID = _CTracker->tracks[i]->detected_object_id;

                            objects.push_back(r);
                            objects_id.push_back(trackID);
                            objects_type.push_back(1);
                            std::vector<Point> pts;
                            for (int pt_idx = 0; pt_idx < _CTracker->tracks[i]->trace.size(); pt_idx++)
                            {
                                pts.push_back((Point)_CTracker->tracks[i]->trace[pt_idx]);
                            }
                            //putText(image_dbg, format("%d", pts.size()), Point(50,50), FONT_HERSHEY_SIMPLEX, 1, Colors[0], 2);
                            objects_history.push_back(pts);
                            //drawContours(image_dbg, objects_history, -1, Scalar(0, 0, 0), 2);
                            if (visparams.drawDetections)
                            {
                                rectangle(image, r, Colors[(trackID - 1) % 12], visparams.DetectionsPenSize);
                                rectangle(image_dbg, r, Colors[(trackID - 1) % 12], visparams.DetectionsPenSize);
                            }

                            if (visparams.showDetectionID)
                            {
                                double fontscale = (double)r.height / 150;
                                putText(image, format("%d", trackID), r.tl(), FONT_HERSHEY_SIMPLEX, fontscale, swpScalar(Colors[trackID % 12]), 5);
                                putText(image_dbg, format("%d", trackID), r.tl(), FONT_HERSHEY_SIMPLEX, fontscale, swpScalar(Colors[trackID % 12]), 5);
                                putText(image, format("%d", trackID), r.tl(), FONT_HERSHEY_SIMPLEX, fontscale, Colors[trackID % 12], 2);
                                putText(image_dbg, format("%d", trackID), r.tl(), FONT_HERSHEY_SIMPLEX, fontscale, Colors[trackID % 12], 2);
                            }
                        }


                    }

                }

            }

            if (ShowDebugWindow)
            {
                namedWindow("detections", 0);
                imshow("detections", image_dbg);
                int key = waitKey(DebugWaitTime);

                if (key == 49)
                {
                    if (DebugWaitTime > 0)
                        DebugWaitTime = 0;
                    else
                        DebugWaitTime = 1;
                }


                if (key == 50)
                    scale = 2;

                if (key == 51)
                {
                    scale = 1;
                    //_MultiTracker = MultiTracker("DSST");
                }


                if (key == 52)
                {
                    scale = 4;
                    //_MultiTracker = MultiTracker("DSST");
                }
            }

            return isBackgroundSubtractorDefined;
        }
        return isBackgroundSubtractorDefined;
    }

    int TestMultiTracker::setScale(int Value)
    {
        int previousValue = scale;
        scale = Value;
        return previousValue;
    }

    int TestMultiTracker::setUpscaleValue(int Value)
    {
        int previousValue = upscale_value;
        upscale_value = Value;
        return previousValue;
    }

    int TestMultiTracker::setMaxTrackedCount(int Value)
    {
        int previousValue = maxTrackedCount;
        maxTrackedCount = Value;
        return previousValue;
    }

    Size TestMultiTracker::setMinTrackedSize(Size Value)
    {
        Size previousValue = minTrackedSize;
        minTrackedSize = Value;
        return previousValue;
    }

    int TestMultiTracker::setDebug(int Value)
    {
        int previousValue = ShowDebugWindow;
        ShowDebugWindow = Value;
        return previousValue;
    }

    bool TestMultiTracker::setPedestrianDetection(bool Value)
    {
        bool previousValue = PedestrianDetection;
        PedestrianDetection = Value;
        return previousValue;
    }

    bool TestMultiTracker::setHumanDetection(bool Value)
    {
        bool previousValue = HumanDetection;
        HumanDetection = Value;
        return previousValue;
    }

    bool TestMultiTracker::setCarDetection(bool Value)
    {
        bool previousValue = CarDetection;
        CarDetection = Value;
        return previousValue;
    }

    std::vector<Rect> TestMultiTracker::findObjects(Mat& image)
    {
        std::vector<Rect> result_rects;
        std::vector<Rect> rects;

        if (!isHOGDescriptorDefined)
        {
            _HOGDescriptor = HOGDescriptor();
            _HOGDescriptor.setSVMDetector(_HOGDescriptor.getDefaultPeopleDetector());
            //_HumanDetector = HumanDetector();
            //_HumanDetector.load(_HumanDetector_model_file_path);
        }

        _HOGDescriptor.detectMultiScale(image, result_rects);
        //_HumanDetector.detectMultiScale(image, rects);

        if (CarDetection & isCascadeClassifierrDefined)
        {
            _CascadeClassifier.detectMultiScale(image, rects, 1.1, 3, 0, Size(image.cols / 15, image.cols / 15));

            for (size_t i = 0; i < rects.size(); i++)
            {
                result_rects.push_back(rects[i]);
            }
        }

        return result_rects;
    }

    TestMultiTrackerVisParams TestMultiTracker::setVisParams(TestMultiTrackerVisParams Value)
    {
        TestMultiTrackerVisParams previousValue = visparams;
        visparams = Value;
        return previousValue;
    }

    bool TestMultiTracker::getObjects(CV_OUT std::vector<Rect>& obj_rects, CV_OUT std::vector<int>& obj_ids, CV_OUT std::vector<int>& obj_type, CV_OUT std::vector<std::vector<Point> >& obj_history)
    {

        obj_rects.clear();
        obj_ids.clear();
        obj_type.clear();
        obj_history.clear();

        for (int i = 0; i < objects.size(); i++)
        {
            obj_rects.push_back(objects[i]);
            obj_ids.push_back(objects_id[i]);
            obj_type.push_back(objects_type[i]);
            obj_history.push_back(objects_history[i]);
        }
        return true;
    }

    bool TestMultiTracker::loadVisParams(const String& file_name)
    {
        FileStorage fs(file_name, FileStorage::READ);
        FileNode fn = fs.root();

        if ((String)fn["name"] == "params")
        {
            visparams.drawMotionRects = (int)fn["drawMotionRects"];
            visparams.drawMotionHistory = (int)fn["drawMotionHistory"];
            visparams.drawDetections = (int)fn["drawDetections"];
            visparams.MotionRectsColor = (Scalar)fn["MotionRectsColor"];
            visparams.MotionRectsPenSize = (int)fn["MotionRectsPenSize"];
            visparams.MotionHistoryPenSize = (int)fn["MotionHistoryPenSize"];
            visparams.DetectionsPenSize = (int)fn["DetectionsPenSize"];
            visparams.showDetectionID = (int)fn["showDetectionID"];
            return true;
        }
        return false;
    }

    bool TestMultiTracker::saveVisParams(const String& file_name)
    {
        FileStorage fs(file_name, FileStorage::WRITE);

        if (!fs.isOpened()) return false;

        fs << "name" << "params";
        fs << "drawMotionRects" << visparams.drawMotionRects;
        fs << "drawMotionHistory" << visparams.drawMotionHistory;
        fs << "drawDetections" << visparams.drawDetections;
        fs << "MotionRectsColor" << visparams.MotionRectsColor;
        fs << "MotionRectsPenSize" << visparams.MotionRectsPenSize;
        fs << "MotionHistoryPenSize" << visparams.MotionHistoryPenSize;
        fs << "DetectionsPenSize" << visparams.DetectionsPenSize;
        fs << "showDetectionID" << visparams.showDetectionID;

        fs.release();

        return true;
    }

    bool overlapRoi(Rect rect1, Rect rect2, Rect& roi)
    {
        Point tl1 = rect1.tl();
        Point tl2 = rect2.tl();
        Size sz1 = rect1.size();
        Size sz2 = rect2.size();

        int x_tl = std::max(tl1.x, tl2.x);
        int y_tl = std::max(tl1.y, tl2.y);
        int x_br = std::min(tl1.x + sz1.width, tl2.x + sz2.width);
        int y_br = std::min(tl1.y + sz1.height, tl2.y + sz2.height);
        if (x_tl < x_br && y_tl < y_br)
        {
            roi = Rect(x_tl, y_tl, x_br - x_tl, y_br - y_tl);
            return true;
        }
        return false;
    }

    bool TestMultiTracker::isNotTrackedObject(Rect rect)
    {
        for (int i = 0; i < _CTracker->tracks.size(); i++)
        {
            if (_CTracker->tracks[i]->detected_object_rect.size() > 0)
            {
                Rect r;
                r.x = _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1].x;
                r.y = _CTracker->tracks[i]->trace[_CTracker->tracks[i]->trace.size() - 1].y;
                r.width = _CTracker->tracks[i]->detected_object_rect[0].width;
                r.height = _CTracker->tracks[i]->detected_object_rect[0].height;
                r.x -= r.width / 2;
                r.y -= r.height / 2;

                Rect roi;
                if (overlapRoi(r, rect, roi))
                {
                    return true;
                }
            }

        }

        return true;
    }


    //
    AssignmentProblemSolver::AssignmentProblemSolver()
    {
    }

    AssignmentProblemSolver::~AssignmentProblemSolver()
    {
    }

    double AssignmentProblemSolver::Solve(std::vector<std::vector<double> >& DistMatrix, std::vector<int>& Assignment, TMethod Method)
    {
        int N = DistMatrix.size(); // number of columns (tracks)
        int M = DistMatrix[0].size(); // number of rows (measurements)

        int* assignment = new int[N];
        double* distIn = new double[N * M];

        double  cost;
        // Fill matrix with random numbers
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                distIn[i + N * j] = DistMatrix[i][j];
            }
        }
        switch (Method)
        {
        case optimal: assignmentoptimal(assignment, &cost, distIn, N, M); break;

        case many_forbidden_assignments: assignmentoptimal(assignment, &cost, distIn, N, M); break;

        case without_forbidden_assignments: assignmentoptimal(assignment, &cost, distIn, N, M); break;
        }

        // form result 
        Assignment.clear();
        for (int x = 0; x < N; x++)
        {
            Assignment.push_back(assignment[x]);
        }

        delete[] assignment;
        delete[] distIn;
        return cost;
    }
    // --------------------------------------------------------------------------
    // Computes the optimal assignment (minimum overall costs) using Munkres algorithm.
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::assignmentoptimal(int* assignment, double* cost, double* distMatrixIn, int nOfRows, int nOfColumns)
    {
        double* distMatrix;
        double* distMatrixTemp;
        double* distMatrixEnd;
        double* columnEnd;
        double  value;
        double  minValue;

        bool* coveredColumns;
        bool* coveredRows;
        bool* starMatrix;
        bool* newStarMatrix;
        bool* primeMatrix;

        int nOfElements;
        int minDim;
        int row;
        int col;

        // Init
        *cost = 0;
        for (row = 0; row < nOfRows; row++)
        {
            assignment[row] = -1.0;
        }

        // Generate distance matrix 
        // and check matrix elements positiveness :)

        // Total elements number
        nOfElements = nOfRows * nOfColumns;
        // Memory allocation
        distMatrix = (double*)malloc(nOfElements * sizeof(double));
        // Pointer to last element
        distMatrixEnd = distMatrix + nOfElements;

        // 
        for (row = 0; row < nOfElements; row++)
        {
            value = distMatrixIn[row];
            if (value < 0)
            {
                cout << "All matrix elements have to be non-negative." << endl;
            }
            distMatrix[row] = value;
        }

        // Memory allocation
        coveredColumns = (bool*)calloc(nOfColumns, sizeof(bool));
        coveredRows = (bool*)calloc(nOfRows, sizeof(bool));
        starMatrix = (bool*)calloc(nOfElements, sizeof(bool));
        primeMatrix = (bool*)calloc(nOfElements, sizeof(bool));
        newStarMatrix = (bool*)calloc(nOfElements, sizeof(bool)); /* used in step4 */

                                                                   /* preliminary steps */
        if (nOfRows <= nOfColumns)
        {
            minDim = nOfRows;
            for (row = 0; row < nOfRows; row++)
            {
                /* find the smallest element in the row */
                distMatrixTemp = distMatrix + row;
                minValue = *distMatrixTemp;
                distMatrixTemp += nOfRows;
                while (distMatrixTemp < distMatrixEnd)
                {
                    value = *distMatrixTemp;
                    if (value < minValue)
                    {
                        minValue = value;
                    }
                    distMatrixTemp += nOfRows;
                }
                /* subtract the smallest element from each element of the row */
                distMatrixTemp = distMatrix + row;
                while (distMatrixTemp < distMatrixEnd)
                {
                    *distMatrixTemp -= minValue;
                    distMatrixTemp += nOfRows;
                }
            }
            /* Steps 1 and 2a */
            for (row = 0; row < nOfRows; row++)
            {
                for (col = 0; col < nOfColumns; col++)
                {
                    if (distMatrix[row + nOfRows * col] == 0)
                    {
                        if (!coveredColumns[col])
                        {
                            starMatrix[row + nOfRows * col] = true;
                            coveredColumns[col] = true;
                            break;
                        }
                    }
                }
            }
        }
        else /* if(nOfRows > nOfColumns) */
        {
            minDim = nOfColumns;
            for (col = 0; col < nOfColumns; col++)
            {
                /* find the smallest element in the column */
                distMatrixTemp = distMatrix + nOfRows * col;
                columnEnd = distMatrixTemp + nOfRows;
                minValue = *distMatrixTemp++;
                while (distMatrixTemp < columnEnd)
                {
                    value = *distMatrixTemp++;
                    if (value < minValue)
                    {
                        minValue = value;
                    }
                }
                /* subtract the smallest element from each element of the column */
                distMatrixTemp = distMatrix + nOfRows * col;
                while (distMatrixTemp < columnEnd)
                {
                    *distMatrixTemp++ -= minValue;
                }
            }
            /* Steps 1 and 2a */
            for (col = 0; col < nOfColumns; col++)
            {
                for (row = 0; row < nOfRows; row++)
                {
                    if (distMatrix[row + nOfRows * col] == 0)
                    {
                        if (!coveredRows[row])
                        {
                            starMatrix[row + nOfRows * col] = true;
                            coveredColumns[col] = true;
                            coveredRows[row] = true;
                            break;
                        }
                    }
                }
            }

            for (row = 0; row < nOfRows; row++)
            {
                coveredRows[row] = false;
            }
        }
        /* move to step 2b */
        step2b(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
        /* compute cost and remove invalid assignments */
        computeassignmentcost(assignment, cost, distMatrixIn, nOfRows);
        /* free allocated memory */
        free(distMatrix);
        free(coveredColumns);
        free(coveredRows);
        free(starMatrix);
        free(primeMatrix);
        free(newStarMatrix);
        return;
    }
    // --------------------------------------------------------------------------
    //
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::buildassignmentvector(int* assignment, bool* starMatrix, int nOfRows, int nOfColumns)
    {
        int row, col;
        for (row = 0; row < nOfRows; row++)
        {
            for (col = 0; col < nOfColumns; col++)
            {
                if (starMatrix[row + nOfRows * col])
                {
                    assignment[row] = col;
                    break;
                }
            }
        }
    }
    // --------------------------------------------------------------------------
    //
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::computeassignmentcost(int* assignment, double* cost, double* distMatrix, int nOfRows)
    {
        int row, col;
        for (row = 0; row < nOfRows; row++)
        {
            col = assignment[row];
            if (col >= 0)
            {
                *cost += distMatrix[row + nOfRows * col];
            }
        }
    }

    // --------------------------------------------------------------------------
    //
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::step2a(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim)
    {
        bool* starMatrixTemp, * columnEnd;
        int col;
        /* cover every column containing a starred zero */
        for (col = 0; col < nOfColumns; col++)
        {
            starMatrixTemp = starMatrix + nOfRows * col;
            columnEnd = starMatrixTemp + nOfRows;
            while (starMatrixTemp < columnEnd)
            {
                if (*starMatrixTemp++)
                {
                    coveredColumns[col] = true;
                    break;
                }
            }
        }
        /* move to step 3 */
        step2b(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
    }

    // --------------------------------------------------------------------------
    //
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::step2b(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim)
    {
        int col, nOfCoveredColumns;
        /* count covered columns */
        nOfCoveredColumns = 0;
        for (col = 0; col < nOfColumns; col++)
        {
            if (coveredColumns[col])
            {
                nOfCoveredColumns++;
            }
        }
        if (nOfCoveredColumns == minDim)
        {
            /* algorithm finished */
            buildassignmentvector(assignment, starMatrix, nOfRows, nOfColumns);
        }
        else
        {
            /* move to step 3 */
            step3(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
        }
    }

    // --------------------------------------------------------------------------
    //
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::step3(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim)
    {
        bool zerosFound;
        int row, col, starCol;
        zerosFound = true;
        while (zerosFound)
        {
            zerosFound = false;
            for (col = 0; col < nOfColumns; col++)
            {
                if (!coveredColumns[col])
                {
                    for (row = 0; row < nOfRows; row++)
                    {
                        if ((!coveredRows[row]) && (distMatrix[row + nOfRows * col] == 0))
                        {
                            /* prime zero */
                            primeMatrix[row + nOfRows * col] = true;
                            /* find starred zero in current row */
                            for (starCol = 0; starCol < nOfColumns; starCol++)
                                if (starMatrix[row + nOfRows * starCol])
                                {
                                    break;
                                }
                            if (starCol == nOfColumns) /* no starred zero found */
                            {
                                /* move to step 4 */
                                step4(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim, row, col);
                                return;
                            }
                            else
                            {
                                coveredRows[row] = true;
                                coveredColumns[starCol] = false;
                                zerosFound = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        /* move to step 5 */
        step5(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
    }

    // --------------------------------------------------------------------------
    //
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::step4(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim, int row, int col)
    {
        int n, starRow, starCol, primeRow, primeCol;
        int nOfElements = nOfRows * nOfColumns;
        /* generate temporary copy of starMatrix */
        for (n = 0; n < nOfElements; n++)
        {
            newStarMatrix[n] = starMatrix[n];
        }
        /* star current zero */
        newStarMatrix[row + nOfRows * col] = true;
        /* find starred zero in current column */
        starCol = col;
        for (starRow = 0; starRow < nOfRows; starRow++)
        {
            if (starMatrix[starRow + nOfRows * starCol])
            {
                break;
            }
        }
        while (starRow < nOfRows)
        {
            /* unstar the starred zero */
            newStarMatrix[starRow + nOfRows * starCol] = false;
            /* find primed zero in current row */
            primeRow = starRow;
            for (primeCol = 0; primeCol < nOfColumns; primeCol++)
            {
                if (primeMatrix[primeRow + nOfRows * primeCol])
                {
                    break;
                }
            }
            /* star the primed zero */
            newStarMatrix[primeRow + nOfRows * primeCol] = true;
            /* find starred zero in current column */
            starCol = primeCol;
            for (starRow = 0; starRow < nOfRows; starRow++)
            {
                if (starMatrix[starRow + nOfRows * starCol])
                {
                    break;
                }
            }
        }
        /* use temporary copy as new starMatrix */
        /* delete all primes, uncover all rows */
        for (n = 0; n < nOfElements; n++)
        {
            primeMatrix[n] = false;
            starMatrix[n] = newStarMatrix[n];
        }
        for (n = 0; n < nOfRows; n++)
        {
            coveredRows[n] = false;
        }
        /* move to step 2a */
        step2a(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
    }

    // --------------------------------------------------------------------------
    //
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::step5(int* assignment, double* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim)
    {
        double h, value;
        int row, col;
        /* find smallest uncovered element h */
        h = DBL_MAX;
        for (row = 0; row < nOfRows; row++)
        {
            if (!coveredRows[row])
            {
                for (col = 0; col < nOfColumns; col++)
                {
                    if (!coveredColumns[col])
                    {
                        value = distMatrix[row + nOfRows * col];
                        if (value < h)
                        {
                            h = value;
                        }
                    }
                }
            }
        }
        /* add h to each covered row */
        for (row = 0; row < nOfRows; row++)
        {
            if (coveredRows[row])
            {
                for (col = 0; col < nOfColumns; col++)
                {
                    distMatrix[row + nOfRows * col] += h;
                }
            }
        }
        /* subtract h from each uncovered column */
        for (col = 0; col < nOfColumns; col++)
        {
            if (!coveredColumns[col])
            {
                for (row = 0; row < nOfRows; row++)
                {
                    distMatrix[row + nOfRows * col] -= h;
                }
            }
        }
        /* move to step 3 */
        step3(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
    }


    // --------------------------------------------------------------------------
    // Computes a suboptimal solution. Good for cases without forbidden assignments.
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::assignmentsuboptimal2(int* assignment, double* cost, double* distMatrixIn, int nOfRows, int nOfColumns)
    {
        int n, row, col, tmpRow, tmpCol, nOfElements;
        double value, minValue, * distMatrix;


        /* make working copy of distance Matrix */
        nOfElements = nOfRows * nOfColumns;
        distMatrix = (double*)malloc(nOfElements * sizeof(double));
        for (n = 0; n < nOfElements; n++)
        {
            distMatrix[n] = distMatrixIn[n];
        }

        /* initialization */
        *cost = 0;
        for (row = 0; row < nOfRows; row++)
        {
            assignment[row] = -1.0;
        }

        /* recursively search for the minimum element and do the assignment */
        while (true)
        {
            /* find minimum distance observation-to-track pair */
            minValue = DBL_MAX;
            for (row = 0; row < nOfRows; row++)
                for (col = 0; col < nOfColumns; col++)
                {
                    value = distMatrix[row + nOfRows * col];
                    if (value != DBL_MAX && (value < minValue))
                    {
                        minValue = value;
                        tmpRow = row;
                        tmpCol = col;
                    }
                }

            if (minValue != DBL_MAX)
            {
                assignment[tmpRow] = tmpCol;
                *cost += minValue;
                for (n = 0; n < nOfRows; n++)
                {
                    distMatrix[n + nOfRows * tmpCol] = DBL_MAX;
                }
                for (n = 0; n < nOfColumns; n++)
                {
                    distMatrix[tmpRow + nOfRows * n] = DBL_MAX;
                }
            }
            else
                break;

        } /* while(true) */

        free(distMatrix);
    }
    // --------------------------------------------------------------------------
    // Computes a suboptimal solution. Good for cases with many forbidden assignments.
    // --------------------------------------------------------------------------
    void AssignmentProblemSolver::assignmentsuboptimal1(int* assignment, double* cost, double* distMatrixIn, int nOfRows, int nOfColumns)
    {
        bool infiniteValueFound, finiteValueFound, repeatSteps, allSinglyValidated, singleValidationFound;
        int n, row, col, tmpRow, tmpCol, nOfElements;
        int* nOfValidObservations, * nOfValidTracks;
        double value, minValue, * distMatrix;


        /* make working copy of distance Matrix */
        nOfElements = nOfRows * nOfColumns;
        distMatrix = (double*)malloc(nOfElements * sizeof(double));
        for (n = 0; n < nOfElements; n++)
        {
            distMatrix[n] = distMatrixIn[n];
        }
        /* initialization */
        *cost = 0;

        for (row = 0; row < nOfRows; row++)
        {
            assignment[row] = -1.0;
        }

        /* allocate memory */
        nOfValidObservations = (int*)calloc(nOfRows, sizeof(int));
        nOfValidTracks = (int*)calloc(nOfColumns, sizeof(int));

        /* compute number of validations */
        infiniteValueFound = false;
        finiteValueFound = false;
        for (row = 0; row < nOfRows; row++)
        {
            for (col = 0; col < nOfColumns; col++)
            {
                if (distMatrix[row + nOfRows * col] != DBL_MAX)
                {
                    nOfValidTracks[col] += 1;
                    nOfValidObservations[row] += 1;
                    finiteValueFound = true;
                }
                else
                    infiniteValueFound = true;
            }
        }

        if (infiniteValueFound)
        {
            if (!finiteValueFound)
            {
                return;
            }
            repeatSteps = true;

            while (repeatSteps)
            {
                repeatSteps = false;

                /* step 1: reject assignments of multiply validated tracks to singly validated observations		 */
                for (col = 0; col < nOfColumns; col++)
                {
                    singleValidationFound = false;
                    for (row = 0; row < nOfRows; row++)
                        if (distMatrix[row + nOfRows * col] != DBL_MAX && (nOfValidObservations[row] == 1))
                        {
                            singleValidationFound = true;
                            break;
                        }

                    if (singleValidationFound)
                    {
                        for (row = 0; row < nOfRows; row++)
                            if ((nOfValidObservations[row] > 1) && distMatrix[row + nOfRows * col] != DBL_MAX)
                            {
                                distMatrix[row + nOfRows * col] = DBL_MAX;
                                nOfValidObservations[row] -= 1;
                                nOfValidTracks[col] -= 1;
                                repeatSteps = true;
                            }
                    }
                }

                /* step 2: reject assignments of multiply validated observations to singly validated tracks */
                if (nOfColumns > 1)
                {
                    for (row = 0; row < nOfRows; row++)
                    {
                        singleValidationFound = false;
                        for (col = 0; col < nOfColumns; col++)
                        {
                            if (distMatrix[row + nOfRows * col] != DBL_MAX && (nOfValidTracks[col] == 1))
                            {
                                singleValidationFound = true;
                                break;
                            }
                        }

                        if (singleValidationFound)
                        {
                            for (col = 0; col < nOfColumns; col++)
                            {
                                if ((nOfValidTracks[col] > 1) && distMatrix[row + nOfRows * col] != DBL_MAX)
                                {
                                    distMatrix[row + nOfRows * col] = DBL_MAX;
                                    nOfValidObservations[row] -= 1;
                                    nOfValidTracks[col] -= 1;
                                    repeatSteps = true;
                                }
                            }
                        }
                    }
                }
            } /* while(repeatSteps) */

              /* for each multiply validated track that validates only with singly validated  */
              /* observations, choose the observation with minimum distance */
            for (row = 0; row < nOfRows; row++)
            {
                if (nOfValidObservations[row] > 1)
                {
                    allSinglyValidated = true;
                    minValue = DBL_MAX;
                    for (col = 0; col < nOfColumns; col++)
                    {
                        value = distMatrix[row + nOfRows * col];
                        if (value != DBL_MAX)
                        {
                            if (nOfValidTracks[col] > 1)
                            {
                                allSinglyValidated = false;
                                break;
                            }
                            else if ((nOfValidTracks[col] == 1) && (value < minValue))
                            {
                                tmpCol = col;
                                minValue = value;
                            }
                        }
                    }

                    if (allSinglyValidated)
                    {
                        assignment[row] = tmpCol;
                        *cost += minValue;
                        for (n = 0; n < nOfRows; n++)
                        {
                            distMatrix[n + nOfRows * tmpCol] = DBL_MAX;
                        }
                        for (n = 0; n < nOfColumns; n++)
                        {
                            distMatrix[row + nOfRows * n] = DBL_MAX;
                        }
                    }
                }
            }

            /* for each multiply validated observation that validates only with singly validated  */
            /* track, choose the track with minimum distance */
            for (col = 0; col < nOfColumns; col++)
            {
                if (nOfValidTracks[col] > 1)
                {
                    allSinglyValidated = true;
                    minValue = DBL_MAX;
                    for (row = 0; row < nOfRows; row++)
                    {
                        value = distMatrix[row + nOfRows * col];
                        if (value != DBL_MAX)
                        {
                            if (nOfValidObservations[row] > 1)
                            {
                                allSinglyValidated = false;
                                break;
                            }
                            else if ((nOfValidObservations[row] == 1) && (value < minValue))
                            {
                                tmpRow = row;
                                minValue = value;
                            }
                        }
                    }

                    if (allSinglyValidated)
                    {
                        assignment[tmpRow] = col;
                        *cost += minValue;
                        for (n = 0; n < nOfRows; n++)
                            distMatrix[n + nOfRows * col] = DBL_MAX;
                        for (n = 0; n < nOfColumns; n++)
                            distMatrix[tmpRow + nOfRows * n] = DBL_MAX;
                    }
                }
            }
        } /* if(infiniteValueFound) */


          /* now, recursively search for the minimum element and do the assignment */
        while (true)
        {
            /* find minimum distance observation-to-track pair */
            minValue = DBL_MAX;
            for (row = 0; row < nOfRows; row++)
                for (col = 0; col < nOfColumns; col++)
                {
                    value = distMatrix[row + nOfRows * col];
                    if (value != DBL_MAX && (value < minValue))
                    {
                        minValue = value;
                        tmpRow = row;
                        tmpCol = col;
                    }
                }

            if (minValue != DBL_MAX)
            {
                assignment[tmpRow] = tmpCol;
                *cost += minValue;
                for (n = 0; n < nOfRows; n++)
                    distMatrix[n + nOfRows * tmpCol] = DBL_MAX;
                for (n = 0; n < nOfColumns; n++)
                    distMatrix[tmpRow + nOfRows * n] = DBL_MAX;
            }
            else
                break;

        } /* while(true) */

          /* free allocated memory */
        free(nOfValidObservations);
        free(nOfValidTracks);
    }

    TKalmanFilter::TKalmanFilter(Point2f pt, float dt, float Accel_noise_mag)
    {
        //time increment (lower values makes target more "massive")
        deltatime = dt; //0.2

                        // We don't know acceleration, so, assume it to process noise.
                        // But we can guess, the range of acceleration values thich can be achieved by tracked object. 
                        // Process noise. (standard deviation of acceleration: ì/ñ^2)
                        // shows, woh much target can accelerate.
                        //float Accel_noise_mag = 0.5; 

                        //4 state variables, 2 measurements
        kalman = new KalmanFilter(4, 2, 0);
        // Transition matrix
        kalman->transitionMatrix = (Mat_<float>(4, 4) << 1, 0, deltatime, 0, 0, 1, 0, deltatime, 0, 0, 1, 0, 0, 0, 0, 1);

        // init... 
        LastResult = pt;
        kalman->statePre.at<float>(0) = pt.x; // x
        kalman->statePre.at<float>(1) = pt.y; // y

        kalman->statePre.at<float>(2) = 0;
        kalman->statePre.at<float>(3) = 0;

        kalman->statePost.at<float>(0) = pt.x;
        kalman->statePost.at<float>(1) = pt.y;

        setIdentity(kalman->measurementMatrix);

        kalman->processNoiseCov = (Mat_<float>(4, 4) <<
            pow(deltatime, 4.0) / 4.0, 0, pow(deltatime, 3.0) / 2.0, 0,
            0, pow(deltatime, 4.0) / 4.0, 0, pow(deltatime, 3.0) / 2.0,
            pow(deltatime, 3.0) / 2.0, 0, pow(deltatime, 2.0), 0,
            0, pow(deltatime, 3.0) / 2.0, 0, pow(deltatime, 2.0));


        kalman->processNoiseCov *= Accel_noise_mag;

        setIdentity(kalman->measurementNoiseCov, Scalar::all(0.1));

        setIdentity(kalman->errorCovPost, Scalar::all(.1));

    }
    //---------------------------------------------------------------------------
    TKalmanFilter::~TKalmanFilter()
    {
        delete kalman;
    }

    //---------------------------------------------------------------------------
    Point2f TKalmanFilter::GetPrediction()
    {
        Mat prediction = kalman->predict();
        LastResult = Point2f(prediction.at<float>(0), prediction.at<float>(1));
        return LastResult;
    }
    //---------------------------------------------------------------------------
    Point2f TKalmanFilter::Update(Point2f p, bool DataCorrect)
    {
        Mat measurement(2, 1, CV_32FC1);
        if (!DataCorrect)
        {
            measurement.at<float>(0) = LastResult.x;  //update using prediction
            measurement.at<float>(1) = LastResult.y;
        }
        else
        {
            measurement.at<float>(0) = p.x;  //update using measurements
            measurement.at<float>(1) = p.y;
        }
        // Correction
        Mat estimated = kalman->correct(measurement);
        LastResult.x = estimated.at<float>(0);   //update using measurements
        LastResult.y = estimated.at<float>(1);
        return LastResult;
    }

    size_t CTrack::NextTrackID = 0;

    CTrack::CTrack(Point2f pt, float dt, float Accel_noise_mag)
    {
        track_id = NextTrackID;
        detected_object_id = 0;
        NextTrackID++;
        // Every track have its own Kalman filter,
        // it user for next point position prediction.
        KF = new TKalmanFilter(pt, dt, Accel_noise_mag);
        // Here stored points coordinates, used for next position prediction.
        prediction = pt;
        skipped_frames = 0;
    }
    // ---------------------------------------------------------------------------
    //
    // ---------------------------------------------------------------------------
    CTrack::~CTrack()
    {
        // Free resources.
        delete KF;
    }

    // ---------------------------------------------------------------------------
    // Tracker. Manage tracks. Create, remove, update.
    // ---------------------------------------------------------------------------
    CTracker::CTracker(float _dt, float _Accel_noise_mag, double _dist_thres, int _maximum_allowed_skipped_frames, int _max_trace_length)
    {
        dt = _dt;
        Accel_noise_mag = _Accel_noise_mag;
        dist_thres = _dist_thres;
        maximum_allowed_skipped_frames = _maximum_allowed_skipped_frames;
        max_trace_length = _max_trace_length;
    }


    void CTracker::Update(std::vector<Point2d>& detections)
    {
        // -----------------------------------
        // If there is no tracks yet, then every point begins its own track.
        // -----------------------------------
        if (tracks.size() == 0)
        {
            // If no tracks yet
            for (int i = 0; i < detections.size(); i++)
            {
                CTrack* tr = new CTrack(detections[i], dt, Accel_noise_mag);
                tracks.push_back(tr);
            }
        }

        int N = tracks.size();
        int M = detections.size();

        std::vector< std::vector<double> > Cost(N, std::vector<double>(M));
        std::vector<int> assignment;

        double dist;
        for (int i = 0; i < tracks.size(); i++)
        {
            // Point2d prediction=tracks[i]->prediction;
            // cout << prediction << endl;
            for (int j = 0; j < detections.size(); j++)
            {
                Point2d diff = (tracks[i]->prediction - detections[j]);
                dist = sqrtf(diff.x * diff.x + diff.y * diff.y);
                Cost[i][j] = dist;
            }
        }
        // -----------------------------------
        // Solving assignment problem (tracks and predictions of Kalman filter)
        // -----------------------------------
        AssignmentProblemSolver APS;
        APS.Solve(Cost, assignment, AssignmentProblemSolver::optimal);

        // -----------------------------------
        // clean assignment from pairs with large distance
        // -----------------------------------
        // Not assigned tracks
        vector<int> not_assigned_tracks;

        for (int i = 0; i < assignment.size(); i++)
        {
            if (assignment[i] != -1)
            {
                if (Cost[i][assignment[i]] > dist_thres)
                {
                    assignment[i] = -1;
                    // Mark unassigned tracks, and increment skipped frames counter,
                    // when skipped frames counter will be larger than threshold, track will be deleted.
                    not_assigned_tracks.push_back(i);
                }
            }
            else
            {
                // If track have no assigned detect, then increment skipped frames counter.
                tracks[i]->skipped_frames++;
            }

        }

        // -----------------------------------
        // If track didn't get detects long time, remove it.
        // -----------------------------------
        for (int i = 0; i < tracks.size(); i++)
        {
            if (tracks[i]->skipped_frames > maximum_allowed_skipped_frames)
            {
                delete tracks[i];
                tracks.erase(tracks.begin() + i);
                assignment.erase(assignment.begin() + i);
                i--;
            }
        }
        // -----------------------------------
        // Search for unassigned detects
        // -----------------------------------
        std::vector<int> not_assigned_detections;
        std::vector<int>::iterator it;
        for (int i = 0; i < detections.size(); i++)
        {
            it = find(assignment.begin(), assignment.end(), i);
            if (it == assignment.end())
            {
                not_assigned_detections.push_back(i);
            }
        }

        // -----------------------------------
        // and start new tracks for them.
        // -----------------------------------
        if (not_assigned_detections.size() != 0)
        {
            for (int i = 0; i < not_assigned_detections.size(); i++)
            {
                CTrack* tr = new CTrack(detections[not_assigned_detections[i]], dt, Accel_noise_mag);
                tracks.push_back(tr);
            }
        }

        // Update Kalman Filters state

        for (int i = 0; i < assignment.size(); i++)
        {
            // If track updated less than one time, than filter state is not correct.

            tracks[i]->KF->GetPrediction();

            if (assignment[i] != -1) // If we have assigned detect, then update using its coordinates,
            {
                tracks[i]->skipped_frames = 0;
                tracks[i]->prediction = tracks[i]->KF->Update(detections[assignment[i]], 1);
            }
            else				  // if not continue using predictions
            {
                tracks[i]->prediction = tracks[i]->KF->Update(Point2f(0, 0), 0);
            }

            if (tracks[i]->trace.size() > max_trace_length)
            {
                tracks[i]->trace.erase(tracks[i]->trace.begin(), tracks[i]->trace.end() - max_trace_length);
            }

            tracks[i]->trace.push_back(tracks[i]->prediction);
            tracks[i]->KF->LastResult = tracks[i]->prediction;
        }

    }
    // ---------------------------------------------------------------------------
    //
    // ---------------------------------------------------------------------------
    CTracker::~CTracker(void)
    {
        for (int i = 0; i < tracks.size(); i++)
        {
            delete tracks[i];
        }
        tracks.clear();
    }




Scalar negScalar(Scalar color)
{
    Scalar newScalar = Scalar(255, 255, 255) - color;
    return newScalar;

}

int main(int argc, char** argv)
{

    char* filename = argc >= 2 ? argv[1] : (char*)"vtest.avi";
    //char* filename = argc >= 2 ? argv[1] : (char*)"D:/database/DB-pedestrian/video/TownCentreXVID.avi";

    VideoCapture capture(filename);
    TestMultiTracker TestMT("./haarcascade_fullbody.xml", "KCF", "");
    TestMultiTrackerVisParams visparams;
    visparams.drawMotionRects = false;
    //visparams.MotionRectsColor = Scalar(200, 200, 200);
    visparams.MotionRectsPenSize = 2;
    visparams.drawMotionHistory = false;
    //visparams.drawDetections = false;
    visparams.showDetectionID = 1;
    visparams.drawMotionHistory = 1;
    TestMT.setVisParams(visparams);
    //TestMT.saveVisParams("d:/visual_params.txt");
    //TestMT.loadVisParams("d:/visual_params.txt");
    TestMT.setDebug(0);

    //TestMT.setPedestrianDetection(false);
    TestMT.setCarDetection(false);
    //TestMT.setScale(2);
    TestMT.setMaxTrackedCount(2);
    TestMT.setMinTrackedSize(Size(10, 10));

    //TestMT.setUpscaleValue(2);
   // namedWindow("frame", 0);
    Mat frame;
    FileStorage fs("e:/test.txt", FileStorage::WRITE);
    int frame_num = 0;
    int wait_time = 0;
    while (true)
    {
        capture.read(frame);
        if (!frame.data)
        {
            capture.set(CAP_PROP_POS_FRAMES, 0);
            continue;
        }
        //resize(frame, frame, Size(), 0.35, 0.35);

        std::vector<Rect> humans;// = TestMT.findObjects(frame);
        TestMT.update(frame);
        fs << format("rects_%d", frame_num) << Mat(TestMT.objects);
        fs << format("ids_%d", frame_num) << Mat(TestMT.objects_id);
        frame_num++;
        if (!visparams.drawDetections)
            for (int i = 0; i < TestMT.objects.size(); i++)
            {
                Point center = (TestMT.objects[i].br() + TestMT.objects[i].tl()) / 2;
                int radius = TestMT.objects[i].height / 2;
                int trackID = TestMT.objects_id[i];

                circle(frame, center, radius, Colors[trackID % 12], 2);
                putText(frame, format("%d", trackID), center, FONT_HERSHEY_SIMPLEX, 2, negScalar(Colors[trackID % 12]), 5);
            }

        std::vector<Rect> _objects;
        std::vector<int> _objects_type;
        std::vector<int> _objects_id;
        std::vector<std::vector<Point> > _objects_history;
        //TestMT.getObjects(_objects, _objects_id, _objects_type, _objects_history);
        //drawContours(frame, _objects_history, -1, Colors[2], 5);

        for (int i = 0; i < humans.size(); i++)
        {
            Mat m = frame(humans[i]);
            m = m * 1.4;
        }
        imshow("frame", frame);
        int key = cv::waitKey(wait_time);

        if (key == 27)
        {
            return 0;
        }

        if (key == 32)
        {
            wait_time = wait_time > 0 ? 0 : 30;
            //visparams.drawDetections = !visparams.drawDetections;
            //visparams.showDetectionID  = !visparams.showDetectionID;
            //TestMT.setVisParams(visparams);
        }
    }
    return 0;
}
