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
#include <string>
#include <vector>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

typedef struct __circle_t {
    float x;
    float y;
    float r;
}circle_t;



class Detector
{
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > imagePort;  // make a port for reading images
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > outPort;
    yarp::os::BufferedPort<yarp::os::Bottle> targetPort;

public:

    Detector()
    {
        cvImage = display;
        counter = 0;
        face.x = 0;
        face.y = 0;
        face.r = 0;
        prev_x = prev_y = prev_z = 0.0; 
        // constructor
    }

    bool open(yarp::os::ResourceFinder &rf);

    bool close();

    void loop(); 

    bool interrupt();

public: 
    yarp::os::ConstString strCascade;
    //yarp::os::ConstString strNestedCascade;

protected:
    IplImage* cvImage;
    IplImage* display;

    unsigned int counter;
    circle_t face;
    cv::CascadeClassifier cascade;
    //cv::CascadeClassifier nestedCascade;

private:
    bool isIn(circle_t& c1, circle_t& c2);
    void detectAndDraw( cv::Mat& img, double scale, circle_t &c);
    std::string eye;
    double eyeDist;
	int certainty;
    //std::vector<double> rotation;
    double prev_x;
    double prev_y;
    double prev_z;
};

   
   



   
