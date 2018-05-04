#include "breakout.h"
#include "ui_breakout.h"
#include "QtConcurrent/qtconcurrentrun.h"
#include <QThread>
#include <cstdio>
#include <iostream>

using namespace QtConcurrent;
using namespace cv;
using namespace std;

Breakout::Breakout(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Breakout)
{
    ui->setupUi(this);
    ui->openGLWidget->animationTimer.stop();
    initCam();

    connect(&camTimer,  &QTimer::timeout, [&] {
        getCamPic();
    });
    camTimer.setInterval(50);
    camTimer.start();
}

void Breakout::initCam() {
    int frameWidth=320;
    int frameHeight=240;
    int subImageWidth=100;
    int subImageHeight=100;
    int templateWidth=25;
    int templateHeight=25;
    workingRect = Rect((frameWidth-subImageWidth)/2,frameHeight/2+(frameHeight/2-subImageHeight)/2,subImageWidth,subImageHeight);
    templateRect = Rect((workingRect.width-templateWidth)/2,(workingRect.height-templateHeight)/2,templateWidth,templateHeight);
    workingCenter = Point(workingRect.x+subImageWidth/2,workingRect.y+subImageHeight/2);
    cap = 0;
    cout<<"width :"<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"height :"<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,frameWidth);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight);
    if(!cap.isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
    }
    cap >> frame1;
    cv::flip(frame1,frame1,1);
    cv::cvtColor(Mat(frame1,workingRect),frameRect1,COLOR_BGR2GRAY);
    int result_cols =  frame1.cols-templateWidth  + 1;
    int result_rows = frame1.rows-templateHeight + 1;
    resultImage.create( result_cols, result_rows, CV_32FC1 );
}

void Breakout::getCamPic(){
    cap >> frame2;
    cv::flip(frame2,frame2,1);
    cv::cvtColor(Mat(frame2,workingRect),frameRect2,COLOR_BGR2GRAY);
    Mat templateImage(frameRect1,templateRect);
    matchTemplate( frameRect2, templateImage, resultImage, TM_CCORR_NORMED );
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
    vect_.x = maxLoc.x-templateRect.x;
    rectangle(frame2,workingRect,Scalar( 0, 255, 0),2);;
    Point p(workingCenter.x+vect_.x,workingCenter.y);
    arrowedLine(frame2,workingCenter,p,Scalar(255,255,255),2);
    swap(frameRect1,frameRect2);
    ui->labelCam->setPixmap(QPixmap::fromImage(QImage(frame2.data, frame2.cols, frame2.rows,frame2.step, QImage::Format_RGB888)));
    ui->openGLWidget->setVect(vect_);

    // Update button
    if (ui->openGLWidget->animationTimer.isActive() && ui->openGLWidget->getGameState() == 0){
        ui->playButton->setText("Press to Pause");
    }else if (ui->openGLWidget->getGameState() == 0){
        ui->playButton->setText("Press to Play");
    }

    // Check endGame
    if (ui->openGLWidget->getGameState() == 1){
        ui->playButton->setText("Rejouer ?");
        ui->openGLWidget->setGameState(3);
        ui->openGLWidget->updateGL();
    }

    // Check winGame
    if (ui->openGLWidget->getGameState() == 2){
        ui->playButton->setText("Niveau suivant !");
        ui->openGLWidget->setGameState(4);
        ui->openGLWidget->updateGL();
    }
}

Breakout::~Breakout()
{
    delete ui;
}

void Breakout::on_playButton_clicked()
{
    if (ui->openGLWidget->getGameState() == 0){
        (ui->openGLWidget->animationTimer.isActive()) ? ui->openGLWidget->animationTimer.stop() : ui->openGLWidget->animationTimer.start();
    }

    if (ui->openGLWidget->getGameState() == 5){
        ui->openGLWidget->initGame();
        ui->openGLWidget->animationTimer.start();
        ui->openGLWidget->gameTimer.start();
        ui->openGLWidget->setGameState(0);
    }
}

void Breakout::on_decreasePuckSize_clicked()
{
    Puck puck = ui->openGLWidget->getPuck();
    puck.setdX(puck.getdX()-1);
    ui->openGLWidget->setPuck(puck);
    ui->openGLWidget->updateGL();
}

void Breakout::on_increasePuckSize_clicked()
{
    Puck puck = ui->openGLWidget->getPuck();
    puck.setdX(puck.getdX()+1);
    ui->openGLWidget->setPuck(puck);
    ui->openGLWidget->updateGL();
}
