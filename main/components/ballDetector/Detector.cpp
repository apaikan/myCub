// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
/* 
 * Copyright (C) 2012 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Author: Ali Paikan
 * email:  ali.paikan@iit.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

#include "Detector.h"
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

using namespace yarp::sig;
using namespace yarp::os;
using namespace std;
using namespace cv;

typedef struct __circle_t {
    float x;
    float y;
    float r;
}circle_t;



void Detector::loop()
{    
    ImageOf<PixelRgb> *image = imagePort.read();  // read an image
    if (image != NULL) 
    { 
        IplImage *cvImage = (IplImage*)image->getIplImage();        
        ImageOf<PixelRgb> &outImage = outPort.prepare(); //get an output image
        outImage = *image;
        display = (IplImage*) outImage.getIplImage();
        //Mat imgMat = display;

        CvSize size = cvSize(cvImage->width, cvImage->height);

        // Detect a red ball
        CvScalar hsv_min = cvScalar(55, 64, 102, 0);
        CvScalar hsv_max = cvScalar(75, 255, 255, 0);
        IplImage* hsv_frame = cvCreateImage(size, IPL_DEPTH_8U, 3);
        IplImage* thresholded = cvCreateImage(size, IPL_DEPTH_8U, 1);

        // Covert color space to HSV as it is much easier to filter colors in the HSV color-space.
        cvCvtColor(cvImage, hsv_frame, CV_BGR2HSV);
        // Filter out colors which are out of range.
        cvInRangeS(hsv_frame, hsv_min, hsv_max, thresholded);
        // Memory for hough circles
        CvMemStorage* storage = cvCreateMemStorage(0);
        // hough detector works better with some smoothing of the image
        cvDilate(thresholded, thresholded, NULL, 3);
        cvSmooth( thresholded, thresholded, CV_GAUSSIAN, 5, 5);
        CvSeq* circles = cvHoughCircles(thresholded, storage, CV_HOUGH_GRADIENT, 1,
                                        thresholded->height/4, 100, 15, 
                                        thresholded->height/30, thresholded->height/2);

        cvCvtColor(thresholded, display, CV_GRAY2RGB);

        circle_t c;
        c.r = 0;
        for (int i = 0; i < circles->total; i++)
        {
            float* p = (float*)cvGetSeqElem( circles, i );
            if(p[2] > c.r)
            {
                c.x = p[0];
                c.y = p[1];
                c.r = p[2];
            }                       
        }


        if( c.r > 0 )
        {
            printf("Ball! x=%f y=%f r=%f\n\r", c.x, c.y, c.r); fflush(stdout);
            cvCircle(display, cvPoint(cvRound(c.x), cvRound(c.y)), 2, CV_RGB(0,255,0), 3, 8, 0 );
            cvCircle(display, cvPoint(cvRound(c.x), cvRound(c.y)), cvRound(c.r), CV_RGB(0,255,0), 2, 8, 0 );
            
            Bottle &target=targetPort.prepare();
            target.clear();
            target.addString("cam");
            Bottle &pos = target.addList();
            pos.addDouble(c.x);
            pos.addDouble(c.y);
            pos.addDouble(1.0);
            pos.addDouble(0.0); //control speed
            targetPort.write();                      
        }     
        outPort.write();   

        cvReleaseMemStorage(&storage);
        cvReleaseImage(&thresholded);
        cvReleaseImage(&hsv_frame);         
    }
}



bool Detector::open(yarp::os::ResourceFinder &rf)
{
    bool ret = imagePort.open("/ballDetector/image:i");  // give the port a name
    ret = ret && outPort.open("/ballDetector/image:o");
    ret = ret && targetPort.open("/ballDetector/target");
    return ret;
}

bool Detector::close()
{
    imagePort.close();
    outPort.close();
    // restore to the default context 
    targetPort.setStrict();
    Bottle &target = targetPort.prepare();
    target.addString("joint");
    Bottle &pos = target.addList();
    pos.addDouble(105);
    pos.addDouble(100);
    pos.addDouble(2.0); //speed
    targetPort.write();
    targetPort.close();
    return true;
}

bool Detector::interrupt()
{
    imagePort.interrupt();
    return true;
}


