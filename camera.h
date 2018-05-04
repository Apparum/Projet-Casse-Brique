#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QtCore>

using namespace cv;

class Camera : public QThread
{
    Q_OBJECT
public:
    Camera();
    Mat showCamera();
    Point getVect(){return vect_;}
    void run();
    Mat getFrame(){return frame2;}
private :
    Point vect_;
    Mat frame2;
signals:

public slots:
    Mat sendPic();
};

#endif // CAMERA_H
