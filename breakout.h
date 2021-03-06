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
    // Constructor of a game Breakout instance
    explicit Breakout(QWidget *parent = 0);

    // Destructor of a Breakout instance
    ~Breakout();

private:
    Ui::Breakout *ui_; // Control of the ui
    QTimer gameTimer_; // Timer of the game
    VideoCapture cap_; // Camera listener for the webcam
    Mat frame1_,frame2_,frameRect1_,frameRect2_,resultImage_; // Different matrix for the calcul and the display of the webcam
    Rect workingRect_, templateRect_; // Rectangle of the selection of the part of the image
    Point workingCenter_,vect_; // Point of the center of the rectangle and of the vector of the movement

    // Initialize the camera parameters
    void initCam();

private slots:
    void updateGame(); // update the game state and the camera
    void on_playButton_clicked(); // Start/Restart the game or switch between Pause/Play (depending on game state)
    void on_decreasePuckSize_clicked();
    void on_increasePuckSize_clicked();
    void on_nextLevel_clicked();
    void on_previousLevel_clicked();
};

#endif // BREAKOUT_H
