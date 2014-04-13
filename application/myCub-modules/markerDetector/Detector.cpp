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


void Detector::loop()
{    
    ImageOf<PixelRgb> *image = imagePort.read();  // read an image
    if (image != NULL) 
    { 
        IplImage *cvImage = (IplImage*)image->getIplImage();        
        ImageOf<PixelRgb> &outImage = outPort.prepare(); //get an output image
        outImage = *image;
        display = (IplImage*) outImage.getIplImage();
        Mat imgMat = display;

        arParamChangeSize(&cameraParam, imgMat.rows, imgMat.cols, &cameraParam);

        ARMarkerInfo* markerInfo = NULL;
        int markerNum = -1;
        if (arDetectMarker((ARUint8 *)imgMat.data, 100, &markerInfo, &markerNum) < 0) {
                printf("Marker detector error\n");
                return;
        }
        
        // Check which one of the found markers is the one we seek
        int k = -1;
        for (int i = 0; i < markerNum; i++) {
            if (patternID == markerInfo[i].id) {
                if (k == -1) {
                    k = i;
                } else if (markerInfo[k].cf < markerInfo[i].cf) {
                    k = i;
                }
            }
        }

       
       if( k != -1)
       {
            // mark the marker in the image
            printf("Marker at (%.2f, %.2f) \n", markerInfo[k].pos[0], markerInfo[k].pos[1]);  
            //printf("vertex (%.2f, %.2f) (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n",
            //        markerInfo[k].vertex[0][0], markerInfo[k].vertex[0][1],
            //        markerInfo[k].vertex[1][0], markerInfo[k].vertex[1][1],
            //        markerInfo[k].vertex[2][0], markerInfo[k].vertex[2][1],
            //        markerInfo[k].vertex[3][0], markerInfo[k].vertex[3][1]);  

            cvLine(display, cvPoint(markerInfo[k].vertex[0][0], markerInfo[k].vertex[0][1]), 
                            cvPoint(markerInfo[k].vertex[1][0], markerInfo[k].vertex[1][1]), CV_RGB(0,0,255), 2); 
            
            cvCircle(display, cvPoint((int)markerInfo[k].pos[0], (int)markerInfo[k].pos[1]), 
                     3, CV_RGB(0, 255, 0), 2);

            //Bottle &target=targetPort.prepare();
            Bottle target;
            Bottle resp;
            target.clear();
            target.addString("cam");
            Bottle &pos = target.addList();
            pos.addDouble(markerInfo[k].pos[0]);
            pos.addDouble(markerInfo[k].pos[1]);
            pos.addDouble(1.0);
            //target.addInt(markerInfo[k].id);
            //target.addDouble(markerInfo[k].cf);
            targetPort.write(target); 
       }
        outPort.write();   
    }
}


bool Detector::open(yarp::os::ResourceFinder &rf)
{
    // Load (default) camera parameters
    //if (arParamLoad("../../bin/Data/camera_para.dat", 1, &cameraParam) < 0) {
    //        printf("Camera calibration parameters file load error\n");
    //        return false;
    //}

    cameraParam.xsize = 160;
    cameraParam.ysize = 120;

    cameraParam.mat[0][0] = 234.73;
    cameraParam.mat[0][1] = 0.0;
    cameraParam.mat[0][2] = 84.65;
    cameraParam.mat[0][3] = 0.0;
    cameraParam.mat[1][0] = 0.0; 
    cameraParam.mat[1][1] = 234.31;
    cameraParam.mat[1][2] = 59.08;
    cameraParam.mat[1][3] = 0.0;
    cameraParam.mat[2][0] = 0.0;
    cameraParam.mat[2][1] = 0.0;
    cameraParam.mat[2][2] = 1.0;
    cameraParam.mat[2][3] = 0.0;
  
    cameraParam.dist_factor[0] = 1;//-0.279;
    cameraParam.dist_factor[1] = 1;//3.083;
    cameraParam.dist_factor[2] = 1;//-0.005;
    cameraParam.dist_factor[3] = 1;//0.012;

    arInitCparam(&cameraParam);
    arParamDisp( &cameraParam );

    // Load kanji pattern
    if ((patternID = arLoadPatt(strPattern.c_str())) < 0) 
    {
            printf("Cannot load pattern file %s\n", strPattern.c_str());
            return false;
    }

    bool ret = imagePort.open("/markerDetector/image:i");  // give the port a name
    ret = ret && outPort.open("/markerDetector/image:o");
    ret = ret && targetPort.open("/markerDetector/target");
    return ret;
}

bool Detector::close()
{
    imagePort.close();
    outPort.close();
    targetPort.close();
    return true;
}

bool Detector::interrupt()
{
    imagePort.interrupt();
    return true;
}


