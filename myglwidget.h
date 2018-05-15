#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "brick.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "ball.h"
#include "puck.h"

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class MyGLWidget : public QGLWidget{

    Q_OBJECT

public:
    // Constructor of the openGL widget
    MyGLWidget(QWidget * parent = nullptr);

    // Timers that rhythm the game and animation
    QTimer animationTimer_;
    QTimer gameTimer_;

    // Methods
    // Display a wall at a position (x1,y1) with certain dimension (dx,dy,dz) and the color (color1,color2,color3)
    void displayWall(float x1, float dx, float y1, float dy, float color1, float color2, float color3, float dz);
    // Initialize the game parameters
    void initGame();
    // Make the ball bounce on the puck
    void bouncePuck();
    // Make the ball bounce on a brick given the direction it's coming from
    void bounceBrick(int dir);
    // Generate a level given its ID
    void generateLevel(int level_);

    // Getters & Setters
    int getLevel() {return level_;}
    void setVect(Point vect){vect_ = vect;}
    void setGameState(int state) {gameState_ = state;}
    int getGameState() {return gameState_;}
    Puck getPuck() {return puck_;}
    void setPuck(Puck puck) {puck_ = puck;}

// OpenGL functions
protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

// Update de game (bouncing/moving/game state)
private slots:
    void updateGame();

private:
    Ball ball_; // The ball
    Puck puck_; // The puck
    Point vect_; // The movement vector
    std::vector<float> vectX_, vectY_; // The movement vector upon each axis
    std::vector<Brick> bricks_; // The vector of the bricks
    int gameState_ = 0; // 0 : Playing, 1 : Lose, 2 : Win, 3 : Lose Transition, 4 : Win Transition, 5 : Ready for Restart
    int score_ = 0; // The score of the player
    int level_ = 1; // The level of the game
    int livesLeft_ = 3; // The lives left of the player
    float averageX_ = 0, averageY_ = 0; // The mean of the movement vector
};

#endif // MYGLWIDGET_H
