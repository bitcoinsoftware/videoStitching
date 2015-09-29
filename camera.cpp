//#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"

#include "opencv2/videoio/videoio_c.h"
#include "opencv2/highgui/highgui_c.h"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

static void help()
{
    cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
            "This classifier can recognize many kinds of rigid objects, once the appropriate classifier is trained.\n"
            "It's most known use is for faces.\n"
            "Usage:\n"
            "./facedetect"
               "   [--scale=<image scale greater or equal to 1, try 1.3 for example>]\n"
               "   [--try-flip]\n"
               "   [filename|camera_index]\n\n"
            "see facedetect.cmd for one call:\n"
            "./facedetect  --scale=1.3\n\n"
            "During execution:\n\tHit any key to quit.\n"
            "\tUsing OpenCV version " << CV_VERSION << "\n" << endl;
}

int main( int argc, const char** argv )
{
	size_t size = 8;
    vector<CvCapture*> capture(size);

    Mat frame, frameCopy, image;
    const string scaleOpt = "--scale=";
    size_t scaleOptLen = scaleOpt.length();
    const string tryFlipOpt = "--try-flip";
    size_t tryFlipOptLen = tryFlipOpt.length();
    string inputName;
    bool tryflip = false;

    help();
    double scale = 1;
	//read the inline arguments
    for( int i = 1; i < argc; i++ )
    {
        cout << "Processing " << i << " " <<  argv[i] << endl;
        if( scaleOpt.compare( 0, scaleOptLen, argv[i], scaleOptLen ) == 0 )
        {
            if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
                scale = 1;
            cout << " from which we read scale = " << scale << endl;
        }
        else if( tryFlipOpt.compare( 0, tryFlipOptLen, argv[i], tryFlipOptLen ) == 0 )
        {
            tryflip = true;
            cout << " will try to flip image horizontally to detect assymetric objects\n";
        }
        else if( argv[i][0] == '-' )
        {
            cerr << "WARNING: Unknown option %s" << argv[i] << endl;
        }
        else
            inputName.assign( argv[i] );
    }
	// prepare the capture
	int erased =0;
	vector <string> windowNames;
	vector<string>::iterator it;
	it = windowNames.begin();
	
	for (int i = 0;i<capture.size(); i++)
	{
		if( inputName.empty() || (isdigit(inputName.c_str()[0]) && inputName.c_str()[1] == '\0') )
		{
			capture[i] = cvCaptureFromCAM( i );
			//int c = inputName.empty() ? 0 : inputName.c_str()[0] - '0' ;
			if(!capture[i]){
				cout << "Capture from CAM " <<  i << " didn't work" << endl;
				capture.erase(capture.begin()+i-erased);
				erased++;
			}
			else
			{
				cout << "Capture "<<i <<" OK \n";
				//different window names
				stringstream ss;
				ss << i;
				string winName = "result"+ss.str();
				it = windowNames.insert(it, winName);
				cvNamedWindow( winName.c_str(), 1 );
			}
		} 
	}
	for (int i=0; i<capture.size(); i++)
	{
		//TODO run concurent
	    cout << "In capture ..." << endl;
        for(;;)
        {
            IplImage* iplImg = cvQueryFrame( capture[0] );
            frame = cv::cvarrToMat(iplImg);
            if( frame.empty() )
                break;
            if( iplImg->origin == IPL_ORIGIN_TL )
                frame.copyTo( frameCopy );
            else
                flip( frame, frameCopy, 0 );
            cv::imshow( windowNames[i].c_str(), frameCopy );

            if( waitKey( 10 ) >= 0 )
                goto _cleanup_;
        }
        waitKey(0);
	}
_cleanup_:
	// Release captures
	for (int i=0; i <capture.size(); i++)
	{
		cvReleaseCapture( &capture[i] );
		cvDestroyWindow(windowNames[i].c_str());
	}
    return 0;
}


