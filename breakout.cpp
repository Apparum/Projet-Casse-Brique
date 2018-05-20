#include "breakout.h"
#include "ui_breakout.h"
#include <cstdio>
#include <iostream>


using namespace cv;
using namespace std;

Breakout::Breakout(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::Breakout)
{
    ui_->setupUi(this);
    // Pause the game at start
    ui_->openGLWidget->animationTimer_.stop();
    // Initialize the camera
    initCam();

    // Setup the timer of the game
    connect(&gameTimer_,  &QTimer::timeout, [&] {
        updateGame();
    });
    gameTimer_.setInterval(50);
    gameTimer_.start();
}
/** The camera is initialize with the webcam input, the matrix are created and the rectangles are setup
 * @brief Breakout::initCam
 */
void Breakout::initCam() {
    int frameWidth=320;
    int frameHeight=240;
    int subImageWidth=100;
    int subImageHeight=100;
    int templateWidth=25;
    int templateHeight=25;
    workingRect_ = Rect((frameWidth-subImageWidth)/2,frameHeight/2+(frameHeight/2-subImageHeight)/2,subImageWidth,subImageHeight);
    templateRect_ = Rect((workingRect_.width-templateWidth)/2,(workingRect_.height-templateHeight)/2,templateWidth,templateHeight);
    workingCenter_ = Point(workingRect_.x+subImageWidth/2,workingRect_.y+subImageHeight/2);
    cap_ = 0;
    cap_.set(CV_CAP_PROP_FRAME_WIDTH,frameWidth);
    cap_.set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight);
    if(!cap_.isOpened())
    {
        cerr<<"Error openning the default camera"<<endl;
    }
    cap_ >> frame1_;
    cv::flip(frame1_,frame1_,1);
    cv::cvtColor(Mat(frame1_,workingRect_),frameRect1_,COLOR_BGR2GRAY);
    int result_cols =  frame1_.cols-templateWidth  + 1;
    int result_rows = frame1_.rows-templateHeight + 1;
    resultImage_.create( result_cols, result_rows, CV_32FC1 );
}

/** Update the camera and check the game state
 * @brief Breakout::updateGame
 */
void Breakout::updateGame(){
    cap_ >> frame2_;
    cv::flip(frame2_,frame2_,1);
    cv::cvtColor(Mat(frame2_,workingRect_),frameRect2_,COLOR_BGR2GRAY);
    Mat templateImage(frameRect1_,templateRect_);
    matchTemplate( frameRect2_, templateImage, resultImage_, TM_CCORR_NORMED );
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc( resultImage_, &minVal, &maxVal, &minLoc, &maxLoc);
    vect_.x = maxLoc.x-templateRect_.x;
    vect_.y = maxLoc.y-templateRect_.y;
    rectangle(frame2_,workingRect_,Scalar( 0, 255, 0),2);;
    Point p(workingCenter_.x+vect_.x,workingCenter_.y+vect_.y);
    arrowedLine(frame2_,workingCenter_,p,Scalar(255,255,255),2);
    swap(frameRect1_,frameRect2_);
    ui_->labelCam->setPixmap(QPixmap::fromImage(QImage(frame2_.data, frame2_.cols, frame2_.rows,frame2_.step, QImage::Format_RGB888)));
    ui_->labelCam->setScaledContents(true);
    ui_->openGLWidget->setVect(vect_);

    // Update button between play and pause
    if (ui_->openGLWidget->animationTimer_.isActive() && ui_->openGLWidget->getGameState() == 0){
        ui_->playButton->setText("Press to Pause");
    }else if (ui_->openGLWidget->getGameState() == 0){
        ui_->playButton->setText("Press to Play");
    }

    // Check endGame
    if (ui_->openGLWidget->getGameState() == 1){
        ui_->playButton->setText("Play Again ?");
        ui_->openGLWidget->setGameState(3);
        ui_->openGLWidget->updateGL();
    }

    // Check winGame
    if (ui_->openGLWidget->getGameState() == 2){
        ui_->playButton->setText("Next Level !");
        ui_->openGLWidget->setGameState(4);
        ui_->openGLWidget->updateGL();
    }
}

/** Destructor of the class
 * @brief Breakout::~Breakout
 */
Breakout::~Breakout()
{
    delete ui_;
}

/** Switch the game state if in game or restart the game.
 * @brief Breakout::on_playButton_clicked
 */
void Breakout::on_playButton_clicked()
{
    if (ui_->openGLWidget->getGameState() == 0){
        (ui_->openGLWidget->animationTimer_.isActive()) ? ui_->openGLWidget->animationTimer_.stop() : ui_->openGLWidget->animationTimer_.start();
    }

    if (ui_->openGLWidget->getGameState() == 5){
        ui_->openGLWidget->initGame();
        ui_->openGLWidget->animationTimer_.start();
        ui_->openGLWidget->gameTimer_.start();
        ui_->openGLWidget->setGameState(0);
    }
}

/** Decrease the puck size
 * @brief Breakout::on_decreasePuckSize_clicked
 */
void Breakout::on_decreasePuckSize_clicked()
{
    Puck puck = ui_->openGLWidget->getPuck();
    puck.setdX(puck.getdX()-1);
    ui_->openGLWidget->setPuck(puck);
    ui_->openGLWidget->updateGL();
}

/** Increase the puck size
 * @brief Breakout::on_increasePuckSize_clicked
 */
void Breakout::on_increasePuckSize_clicked()
{
    Puck puck = ui_->openGLWidget->getPuck();
    puck.setdX(puck.getdX()+1);
    ui_->openGLWidget->setPuck(puck);
    ui_->openGLWidget->updateGL();
}

/** Go to next level
 * @brief Breakout::on_nextLevel_clicked
 */
void Breakout::on_nextLevel_clicked()
{
    ui_->openGLWidget->generateLevel(ui_->openGLWidget->getLevel() + 1);
    ui_->openGLWidget->initGame();
    ui_->openGLWidget->updateGL();
}

/** Go to previous level
 * @brief Breakout::on_previousLevel_clicked
 */
void Breakout::on_previousLevel_clicked()
{
    ui_->openGLWidget->generateLevel(ui_->openGLWidget->getLevel() - 1);
    ui_->openGLWidget->initGame();
    ui_->openGLWidget->updateGL();
}
