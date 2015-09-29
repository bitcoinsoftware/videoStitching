#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
int main()
{
    //initialize and allocate memory to load the video stream from camera 
    cv::VideoCapture camera0(2);
    cv::VideoCapture camera1(1);

    if( !camera0.isOpened() ) return 1;
    if( !camera1.isOpened() ) return 1;
	cv::Mat frame0, frame1;
    while(true) {
        //grab and retrieve each frames of the video sequentially 
        /*
        cv::Mat3b frame0;
        camera0 >> frame0;
        cv::Mat3b frame1;
        camera1 >> frame1; */
        
		bool bSuccess1 = camera0.read(frame0);    
		bool bSuccess2 = camera1.read(frame1);
		cv::Size size0= frame0.size();
		cv::Size size1= frame1.size();
		cout << size0.width << "  " << size0.height << "\n";
		cout << size1.width << "  " << size1.height << "\n";
		if (bSuccess1 && size0.height>0 && size0.width>0); cv::imshow("Video0", frame0);
        if (bSuccess2 && size1.height>0 && size1.width>0){ cv::imshow("Video1", frame1);}

        //wait for 40 milliseconds
        int c = cvWaitKey(40);

        //exit the loop if user press "Esc" key  (ASCII value of "Esc" is 27) 
        if(27 == char(c)) break;
    }

    return 0;
}
