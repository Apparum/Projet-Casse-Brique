#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <QMainWindow>
#include <QTimer>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

namespace Ui {
class Breakout;
}

class Breakout : public QMainWindow
{
    Q_OBJECT

public:
    explicit Breakout(QWidget *parent = 0);
    ~Breakout();

private:
    Ui::Breakout *ui;
    QTimer camTimer;
    VideoCapture cap;
    Mat frame1,frame2,frameRect1,frameRect2,resultImage;
    Rect workingRect, templateRect;
    Point workingCenter,vect_;

    void initCam();
public slots:
    void getCamPic();
private slots:
    void on_playButton_clicked();
    void on_decreasePuckSize_clicked();
    void on_increasePuckSize_clicked();
    void on_nextLevel_clicked();
    void on_previousLevel_clicked();
};

#endif // BREAKOUT_H
