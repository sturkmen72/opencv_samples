const char *faceCascadeFilename = "lbpcascade_frontalface.xml";     // LBP face detector.
const char *eyeCascadeFilename1 = "haarcascade_mcs_righteye.xml";               // Basic eye detector for open eyes only.
const char *eyeCascadeFilename2 = "haarcascade_mcs_lefteye.xml"; // Basic eye detector for open eyes if they might wear glasses.

const char *windowName = "WebcamFaceRec";   // Name shown in the GUI window.

#include <vector>
#include <string>
#include <iostream>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Rect shrinkRect( Rect rect, int width_percent, int height_percent )
{
    if ( width_percent > 100 ) width_percent = 100;
    if ( height_percent > 100 ) height_percent = 100;

    Rect newrect;
    newrect.width = ( rect.width * width_percent ) / 100;
    newrect.height = ( rect.height * height_percent ) / 100;
    newrect.x = rect.x + ( rect.width - newrect.width ) / 2;
    newrect.y = rect.y + ( rect.height - newrect.height ) / 2;

    return newrect;
}


// Search for just a single object in the image, such as the largest face, storing the result into 'largestObject'.
// Can use Haar cascades or LBP cascades for Face Detection, or even eye, mouth, or car detection.
// Input is temporarily shrunk to 'scaledWidth' for much faster detection, since 200 is enough to find faces.
// Note: detectLargestObject() should be faster than detectManyObjects().
void detectObjectsCustom(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors)
{
    // If the input image is not grayscale, then convert the BGR or BGRA color image to grayscale.
    Mat gray;
    if (img.channels() == 3)
    {
        cvtColor(img, gray, CV_BGR2GRAY);
    }
    else if (img.channels() == 4)
    {
        cvtColor(img, gray, CV_BGRA2GRAY);
    }
    else
    {
        // Access the input image directly, since it is already grayscale.
        gray = img;
    }

    // Possibly shrink the image, to run much faster.
    Mat inputImg;
    float scale = img.cols / (float)scaledWidth;
    if (img.cols > scaledWidth)
    {
        // Shrink the image while keeping the same aspect ratio.
        int scaledHeight = cvRound(img.rows / scale);
        resize(gray, inputImg, Size(scaledWidth, scaledHeight));
    }
    else
    {
        // Access the input image directly, since it is already small.
        inputImg = gray;
    }

    // Standardize the brightness and contrast to improve dark images.
    Mat equalizedImg;
    equalizeHist(inputImg, equalizedImg);

    // Detect objects in the small grayscale image.
    cascade.detectMultiScale(equalizedImg, objects, searchScaleFactor, minNeighbors, flags, minFeatureSize);

    // Enlarge the results if the image was temporarily shrunk before detection.
    if (img.cols > scaledWidth)
    {
        for (int i = 0; i < (int)objects.size(); i++)
        {
            objects[i].x = cvRound(objects[i].x * scale);
            objects[i].y = cvRound(objects[i].y * scale);
            objects[i].width = cvRound(objects[i].width * scale);
            objects[i].height = cvRound(objects[i].height * scale);
        }
    }

    // Make sure the object is completely within the image, in case it was on a border.
    for (int i = 0; i < (int)objects.size(); i++)
    {
        if (objects[i].x < 0)
            objects[i].x = 0;
        if (objects[i].y < 0)
            objects[i].y = 0;
        if (objects[i].x + objects[i].width > img.cols)
            objects[i].x = img.cols - objects[i].width;
        if (objects[i].y + objects[i].height > img.rows)
            objects[i].y = img.rows - objects[i].height;
    }

    // Return with the detected face rectangles stored in "objects".
}


void detectLargestObject(const Mat &img, CascadeClassifier &cascade, Rect &largestObject, int scaledWidth)
{
    // Only search for just 1 object (the biggest in the image).
    int flags = CASCADE_FIND_BIGGEST_OBJECT;// | CASCADE_DO_ROUGH_SEARCH;
    // Smallest object size.
    Size minFeatureSize = Size(20, 20);
    // How detailed should the search be. Must be larger than 1.0.
    float searchScaleFactor = 1.1f;
    // How much the detections should be filtered out. This should depend on how bad false detections are to your system.
    // minNeighbors=2 means lots of good+bad detections, and minNeighbors=6 means only good detections are given but some are missed.
    int minNeighbors = 4;

    // Perform Object or Face Detection, looking for just 1 object (the biggest in the image).
    vector<Rect> objects;
    detectObjectsCustom(img, cascade, objects, scaledWidth, flags, minFeatureSize, searchScaleFactor, minNeighbors);
    if (objects.size() > 0)
    {
        // Return the only detected object.
        largestObject = (Rect)objects.at(0);


    }
    else
    {
        // Return an invalid rect.
        largestObject = Rect(-1, -1, -1, -1);
    }

}

void initDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2)
{
    // Load the Face Detection cascade classifier xml file.
    try     // Surround the OpenCV call by a try/catch block so we can give a useful error message!
    {
        faceCascade.load(faceCascadeFilename);
    }
    catch (cv::Exception &e) {}
    if (faceCascade.empty())
    {
        cerr << "ERROR: Could not load Face Detection cascade classifier [" << faceCascadeFilename << "]!" << endl;
        cerr << "Copy the file from your OpenCV data folder (eg: 'C:\\OpenCV\\data\\lbpcascades') into this WebcamFaceRec folder." << endl;
        exit(1);
    }
    cout << "Loaded the Face Detection cascade classifier [" << faceCascadeFilename << "]." << endl;

    // Load the Eye Detection cascade classifier xml file.


    try     // Surround the OpenCV call by a try/catch block so we can give a useful error message!
    {
        eyeCascade1.load(eyeCascadeFilename1);
    }
    catch (cv::Exception &e) {}
    if (eyeCascade1.empty())
    {
        cerr << "ERROR: Could not load 1st Eye Detection cascade classifier [" << eyeCascadeFilename1<< "]!" << endl;
        cerr << "Copy the file from your OpenCV data folder (eg: 'C:\\OpenCV\\data\\haarcascades') into this WebcamFaceRec folder." << endl;
        exit(1);
    }
    cout << "Loaded the 1st Eye Detection cascade classifier [" << eyeCascadeFilename1 << "]." << endl;

    try     // Surround the OpenCV call by a try/catch block so we can give a useful error message!
    {
        eyeCascade2.load(eyeCascadeFilename2);
    }
    catch (cv::Exception &e) {}
    if (eyeCascade2.empty())
    {
        cerr << "ERROR: Could not load 1st Eye Detection cascade classifier [" << eyeCascadeFilename2<< "]!" << endl;
        cerr << "Copy the file from your OpenCV data folder (eg: 'C:\\OpenCV\\data\\haarcascades') into this WebcamFaceRec folder." << endl;
        exit(1);
    }
    cout << "Loaded the 2nd Eye Detection cascade classifier [" << eyeCascadeFilename2 << "]." << endl;


}





// Get access to the webcam.
void initWebcam(VideoCapture &videoCapture, int cameraNumber)
{
    // Get access to the default camera.
    try     // Surround the OpenCV call by a try/catch block so we can give a useful error message!
    {
        videoCapture.open(cameraNumber);
    }
    catch (cv::Exception &e) {}
    if (!videoCapture.isOpened())
    {
        cerr << "ERROR: Could not access the camera!" << endl;
        exit(1);
    }
    cout << "Loaded camera " << cameraNumber << "." << endl;
}


void detectBothEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, Point &leftEye, Point &rightEye, Rect *searchedLeftEye, Rect *searchedRightEye)
{
    // Skip the borders of the face, since it is usually just hair and ears, that we don't care about.
    /*
    // For "2splits.xml": Finds both eyes in roughly 60% of detected faces, also detects closed eyes.
    const float EYE_SX = 0.12f;
    const float EYE_SY = 0.17f;
    const float EYE_SW = 0.37f;
    const float EYE_SH = 0.36f;
    */

    const float EYE_SX = 0.10f;
    const float EYE_SY = 0.19f;
    const float EYE_SW = 0.40f;
    const float EYE_SH = 0.36f;


    // For default eye.xml or eyeglasses.xml: Finds both eyes in roughly 40% of detected faces, but does not detect closed eyes.
    //const float EYE_SX = 0.16f;
    //const float EYE_SY = 0.26f;
    //const float EYE_SW = 0.30f;
    //const float EYE_SH = 0.28f;

    int leftX = cvRound(face.cols * EYE_SX);
    int topY = cvRound(face.rows * EYE_SY);
    int widthX = cvRound(face.cols * EYE_SW);
    int heightY = cvRound(face.rows * EYE_SH);
    int rightX = cvRound(face.cols * (1.0 - EYE_SX - EYE_SW));  // Start of right-eye corner

    /*
    float centerX = leftX + 0.5*widthX;
    float  centerY = topY + 0.5*heightY;
    float newWidth = 0.3*widthX;
    float newHeight = 0.3*heightY;
    float newX = centerX - 0.5*newWidth;
    float newY = centerY - 0.5*newHeight;
    Mat new_rect= face(Rect(newX, newY, newWidth, newHeight));
    */

    Mat topLeftOfFace = face(Rect(leftX, topY, widthX, heightY));








    Mat topRightOfFace = face(Rect(rightX, topY, widthX, heightY));




    Rect leftEyeRect, rightEyeRect;

    // Return the search windows to the caller, if desired.
    if (searchedLeftEye)
        *searchedLeftEye = Rect(leftX, topY, widthX, heightY);
    if (searchedRightEye)
        *searchedRightEye = Rect(rightX, topY, widthX, heightY);

    // Search the left region, then the right region using the 1st eye detector.
    detectLargestObject(topLeftOfFace, eyeCascade1, leftEyeRect, topLeftOfFace.cols);


    detectLargestObject(topRightOfFace, eyeCascade1, rightEyeRect, topRightOfFace.cols);

    // If the eye was not detected, try a different cascade classifier.
    if (leftEyeRect.width <= 0 && !eyeCascade2.empty())
    {
        detectLargestObject(topLeftOfFace, eyeCascade2, leftEyeRect, topLeftOfFace.cols);
        //if (leftEyeRect.width > 0)
        //    cout << "2nd eye detector LEFT SUCCESS" << endl;
        //else
        //    cout << "2nd eye detector LEFT failed" << endl;
    }
    //else
    //    cout << "1st eye detector LEFT SUCCESS" << endl;

    // If the eye was not detected, try a different cascade classifier.
    if (rightEyeRect.width <= 0 && !eyeCascade2.empty())
    {
        detectLargestObject(topRightOfFace, eyeCascade2, rightEyeRect, topRightOfFace.cols);
        //if (rightEyeRect.width > 0)
        //    cout << "2nd eye detector RIGHT SUCCESS" << endl;
        //else
        //    cout << "2nd eye detector RIGHT failed" << endl;
    }
    //else
    //    cout << "1st eye detector RIGHT SUCCESS" << endl;

    if (leftEyeRect.width > 0)     // Check if the eye was detected.
    {
        leftEyeRect.x += leftX;    // Adjust the left-eye rectangle because the face border was removed.
        leftEyeRect.y += topY;
        leftEye = Point(leftEyeRect.x + leftEyeRect.width / 2, leftEyeRect.y + leftEyeRect.height / 2);
    }
    else
    {
        leftEye = Point(-1, -1);    // Return an invalid point
    }

    if (rightEyeRect.width > 0)   // Check if the eye was detected.
    {
        rightEyeRect.x += rightX; // Adjust the right-eye rectangle, since it starts on the right side of the image.
        rightEyeRect.y += topY;  // Adjust the right-eye rectangle because the face border was removed.
        rightEye = Point(rightEyeRect.x + rightEyeRect.width / 2, rightEyeRect.y + rightEyeRect.height / 2);
    }
    else
    {
        rightEye = Point(-1, -1);    // Return an invalid point
    }
}



int main()

{

    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade1;
    CascadeClassifier eyeCascade2;

    Rect faceRect;
    VideoCapture videoCapture(0 );



    cout << "Face Detection ." << endl;
    cout << "Realtime face detection using LBP " << endl;
    cout << "Compiled with OpenCV version " << CV_VERSION << endl << endl;

    // Load the face and 1 or 2 eye detection XML classifiers.
    initDetectors(faceCascade, eyeCascade1, eyeCascade2);

    Mat thresh, gray;
    while (1)
    {
        Mat frame;
        videoCapture >> frame;
        detectLargestObject(frame, faceCascade, faceRect, frame.cols);
        if (faceRect.width > 0)
        {
            // modification is begin here

            Mat faceImg = frame(faceRect);

            rectangle(frame, faceRect, CV_RGB(255, 0, 0), 2, CV_AA);

            Mat gray;
            if (faceImg.channels() == 3)
            {
                cvtColor(faceImg, gray, CV_BGR2GRAY);
            }
            else if (faceImg.channels() == 4)
            {
                cvtColor(faceImg, gray, CV_BGRA2GRAY);
            }
            else
            {
                // Access the input image directly, since it is already grayscale.
                gray = faceImg;
            }

            Point leftEye, rightEye;
            Rect searchedLeftEye, searchedRightEye;

            detectBothEyes(gray, eyeCascade1, eyeCascade2, leftEye, rightEye, &searchedLeftEye, &searchedRightEye);

            rectangle(faceImg, searchedLeftEye, Scalar(0, 255, 0), 2, 8, 0);
            rectangle(faceImg, searchedRightEye, Scalar(0, 255, 0), 2, 8, 0);

           searchedRightEye.y += searchedRightEye.height / 3;
    		searchedRightEye.height -= searchedRightEye.height / 3;

			int val = searchedRightEye.height;

			searchedRightEye = shrinkRect(searchedRightEye, 50, val);// reduces width and height %50

			Mat eye_region = faceImg(searchedRightEye);



			imshow("eye_  video", eye_region);


        }

        imshow("video", frame);
        // Press 'c' to escape
        if (waitKey(30) == 'c') break;
    }

    return 0;
}
