#define _USE_MATH_DEFINES

#include "myglwidget.h"
#include "brick.h"
#include <QApplication>
#include <QDesktopWidget>
#include <cmath>
#include <random>

const unsigned int WIN_WIDTH  = 640;
const unsigned int WIN_HEIGHT = 480;
const float MAX_DIMENSION     = 15.0f;

/** Constructor of the OpenGL Widget
 * @brief MyGLWidget::MyGLWidget
 * @param parent : QWidget holding the OpenGL Widget
 */
MyGLWidget::MyGLWidget(QWidget * parent) : QGLWidget(parent)
{
    // Update the graphic timer
    connect(&animationTimer_,  &QTimer::timeout, [&] {
        updateGL();
    });
    animationTimer_.setInterval(10);
    animationTimer_.start();

    // Update the timer of the game
    connect(&gameTimer_,  &QTimer::timeout, [&] {
        updateGame();
    });
    gameTimer_.setInterval(10);
    gameTimer_.start();

    // Initialize the game
    initGame();
}

/** Initialize the game by positionning the ball and puck, setting the initial speed and initial parameters
 * @brief MyGLWidget::initGame
 */
void MyGLWidget::initGame(){
    generateLevel(level_);
    ball_.init();
    puck_.init();
    ball_.setVx(ball_.getVx()*(level_+10)/10);
    ball_.setVy(ball_.getVy()*(level_+10)/10);
    averageX_ = 0;
    averageY_ = 0;
}

/** OpenGL initialization
 * @brief MyGLWidget::initializeGL
 */
void MyGLWidget::initializeGL()
{
    // Background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // ZBuffer activation
    glEnable(GL_DEPTH_TEST);
    // Light Setup
    glEnable(GL_LIGHT0);
    GLfloat color_ambiant[] = {1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,color_ambiant);
    GLfloat pos[] = {0.0f,0.0f,5.0f,1.0f};
    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    glEnable(GL_LIGHTING);
}

/** OpenGL resize function
 * @brief MyGLWidget::resizeGL
 * @param width : current window width
 * @param height : current window height
 */
void MyGLWidget::resizeGL(int width, int height)
{
    // Viewport definition
    glViewport(0, 0, width, height);
    // Projection matrix mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(width != 0)
        glOrtho(-MAX_DIMENSION, MAX_DIMENSION, -MAX_DIMENSION * height / static_cast<float>(width), MAX_DIMENSION * height / static_cast<float>(width), -MAX_DIMENSION * 2.0f, MAX_DIMENSION * 2.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/** OpenGL painting function
 * @brief MyGLWidget::paintGL
 */
void MyGLWidget::paintGL()
{
    // Buffer reset
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // Camera setup
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.5f, 1.0f, 10.0f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    // Display & Bounce
    QFont font("Monotype Corsiva", 16);
    if (gameState_ == 0){ // Playing
        for(unsigned int i=0;i<bricks_.size();i++)
        {
            bricks_[i].display();
            // If the ball is at the brick level in Y
            if((ball_.getY() + ball_.getR()) > (bricks_[i].getY()) && (ball_.getY() - ball_.getR()) < (bricks_[i].getY() + bricks_[i].getdY()))
            {
                // If the ball is at the brick level in X
                if ((ball_.getX() + ball_.getR()) > bricks_[i].getX() && (ball_.getX() - ball_.getR()) < (bricks_[i].getX() + bricks_[i].getdX()))
                {
                    float ymin = 0, ymax = 0, xmin = 0, xmax = 0;
                    (bricks_[i].getY() > ball_.getY()) ? ymin = bricks_[i].getY() : ymin = ball_.getY();
                    (bricks_[i].getY()+bricks_[i].getdY() <ball_.getY()+ball_.getR()) ? ymax = bricks_[i].getY()+bricks_[i].getdY() : ymax = ball_.getY()+ball_.getR();
                    (bricks_[i].getX() > ball_.getX()) ? xmin = bricks_[i].getX() : xmin = ball_.getX();
                    (bricks_[i].getX()+bricks_[i].getdX() < ball_.getX()+ball_.getR()) ? xmax = bricks_[i].getX()+bricks_[i].getdX() : xmax = ball_.getX()+ball_.getR();
                    float ydelta = ymax - ymin;
                    float xdelta = xmax - xmin;
                    int dir;
                    (xdelta > ydelta) ? ((ball_.getY() > bricks_[i].getY()) ? dir = 3 : dir = 1) : ((ball_.getX() < bricks_[i].getX()) ? dir = 0 : dir = 2);
                    bounceBrick(dir);
                    bricks_.erase(bricks_.begin() + i);
                    score_ ++;
                }
            }
        }
        ball_.display();
        puck_.display();

        // Create the walls and background
        displayWall(-14,1,-12.5,23.5,0.8,0.8,0.8, 2); // Left wall
        displayWall(14,1,-12.5,23.5,0.8,0.8,0.8, 2); // Right wall
        displayWall(-14,29,10,1,0.8,0.8,0.8, 2); // Top wall
        displayWall(-14,29,-13,1,0.8,0.8,0.8, 2); // Bottom wall
        displayWall(-13,27,-12,22,0.2,0.2,0.2,0); // Background

        // Put the life left indicator
        int x=0;
        for (int i=0; i<livesLeft_-1; i++){
            GLUquadric* tmpQuad=gluNewQuadric();
            float color[] = {1.0,0.0,0.0};
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
            glPushMatrix();
            glTranslatef(x-6.3,12.5,4);
            gluSphere(tmpQuad,0.5,20,20);
            glPopMatrix();
            gluDeleteQuadric(tmpQuad);
            x += 1.1;
        }

        // Print text
        QString str("Lives Left :  ");
        renderText(-13, 10.8, -4, str, font);
        str = "Level : " + QString::number(level_);
        renderText(-2, 10.8, -4, str, font);
        str = "Score : " + QString::number(score_);
        renderText(7, 10.8, -4, str, font);
    }
    else if (gameState_ == 3){ // Lost
        QString str("You lose !");
        renderText(-5,0,-6,str,font);
        level_ = 1;
        score_ = 0;
        livesLeft_ = 3;
        gameTimer_.stop();
        animationTimer_.stop();
        gameState_ = 5;
    }
    else if (gameState_ == 4){ // Win
        QString str("You Win !");
        renderText(-5,0,-6,str,font);
        level_++;
        gameTimer_.stop();
        animationTimer_.stop();
        gameState_ = 5;
    }
}

/** Game update
 * @brief MyGLWidget::updateGame
 */
void MyGLWidget::updateGame(){
    // If the game is running
    if (animationTimer_.isActive()){
        // Move the ball
        ball_.move();
        // Puck boucing
        if((ball_.getY() - ball_.getR()) < (puck_.getY()+puck_.getdY()) && (ball_.getX() + ball_.getR()) > puck_.getX() && (ball_.getX() - ball_.getR()) < (puck_.getX() + puck_.getdX())) bouncePuck();
        puck_.move(averageX_/50);
        if(ball_.getX()-ball_.getR()< -13 || ball_.getX()+ball_.getR() > 14) ball_.setVx(-ball_.getVx()); //Left & Right wall bounce
        if(ball_.getY()+ball_.getR() > 10.5) ball_.setVy(-ball_.getVy()); //Top wall bounce
        if(ball_.getY() < puck_.getY()){ // Check if the ball is too low -> Lost it !
            ball_.init();
            puck_.init();
            ball_.display();
            puck_.display();
            animationTimer_.stop();
            livesLeft_ --;
        }
        // Mean the vectX_ variable to keep movement smooth
        if (vectX_.size() > 60){
            averageX_ = accumulate( vectX_.begin(), vectX_.end(), 0.0)/vectX_.size();
            vectX_.erase(vectX_.begin(), vectX_.begin() + 40);
        }
        else {
            vectX_.push_back(vect_.x);
        }
        // Same on Y
        if (vectY_.size() > 60){
            averageY_ = accumulate( vectY_.begin(), vectY_.end(), 0.0)/vectY_.size();
            vectY_.erase(vectY_.begin(), vectY_.begin() + 40);
        }
        else {
            vectY_.push_back(vect_.y);
        }

        // Stop the puck movement if the Y intensity is big enough to overcome the X intensity.
        // /!\ This decrease the gameplay experience, only here for specification purpose.
        if (abs(averageY_) > 3*abs(averageX_)) {
            averageX_ = 0;
            averageY_ = 0;
        }

        // Lose or Win the game
        if (livesLeft_ == 0) gameState_ = 1;
        if (bricks_.size() == 0) gameState_ = 2;
    }
}


/** Make it bounce on the puck with an angle dependant of the point of contact
 * @brief MyGLWidget::bouncePuck
 */
void MyGLWidget::bouncePuck(){
    float alpha = atan(ball_.getX()-(puck_.getX()+puck_.getdX()/2));
    ball_.setVy((cos(alpha)/8)*(level_+10)/10);
    ball_.setVx((sin(alpha)/8)*(level_+10)/10);
}

/** Make the brick bounce in the right direction
 * Inspired from http://rembound.com/articles/the-breakout-tutorial-with-cpp-and-sdl-2
 * @brief MyGLWidget::bounceBrick
 * @param dir : the incoming direction of the ball (0: Left, 1: Top, 2: Right, 3: Bottom)
 */
void MyGLWidget::bounceBrick(int dir) {
    int xmodifier=1,ymodifier=1;
    if (ball_.getVx() >= 0) {
        // Ball is moving in the positive x direction
        if (ball_.getVy() > 0) {
            // Ball is moving in the positive y direction
            (dir == 0 || dir == 3) ? xmodifier = -1 : ymodifier = -1;
        } else if (ball_.getVy() < 0) {
            // Ball is moving in the negative y direction
            (dir == 0 || dir == 1) ? xmodifier = -1 : ymodifier = -1;
        }
    } else if (ball_.getVx() < 0) {
        // ball is moving in the negative x direction
        if (ball_.getVy() > 0) {
            // ball is moving in the positive y direction
            (dir == 2 || dir == 3) ? xmodifier = -1 : ymodifier = -1;
        } else if (ball_.getVy() < 0) {
            // ball is moving in the negative y direction
            (dir == 1 || dir == 2) ? xmodifier = -1 : ymodifier = -1;
        }
    }
    // Set the new direction of the ball, by multiplying the old direction
    // with the determined direction factors
    ball_.setVx(ball_.getVx()*xmodifier);
    ball_.setVy(ball_.getVy()*ymodifier);
}

/** Display a wall
 * @brief MyGLWidget::displayWall
 * @param x1 : horizontal position of the wall
 * @param dx : width of the wall
 * @param y1 : vertical position of the wall
 * @param dy : heigth of the wall
 * @param color1 : red color of the wall
 * @param color2 : green color of the wall
 * @param color3 : blue color of the wall
 * @param dz : wall thickness
 */
void MyGLWidget::displayWall(float x1, float dx, float y1, float dy, float color1, float color2, float color3, float dz){
    glBegin(GL_QUADS);
    float color[] = {color1,color2,color3};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);

    glNormal3f(1.0,0.0,0.0);
    glVertex3f(x1,y1,0);
    glVertex3f(x1,y1+dy,0);
    glVertex3f(x1,y1+dy,0+dz);
    glVertex3f(x1,y1,0+dz);

    glNormal3f(1.0,0.0,0.0);
    glVertex3f(x1+dx,y1,0);
    glVertex3f(x1+dx,y1+dy,0);
    glVertex3f(x1+dx,y1+dy,0+dz);
    glVertex3f(x1+dx,y1,0+dz);

    glNormal3f(0.0,1.0,0.0);
    glVertex3f(x1,y1,0);
    glVertex3f(x1+dx,y1,0);
    glVertex3f(x1+dx,y1,0+dz);
    glVertex3f(x1,y1,0+dz);

    glNormal3f(0.0,1.0,0.0);
    glVertex3f(x1,y1+dy,0);
    glVertex3f(x1+dx,y1+dy,0);
    glVertex3f(x1+dx,y1+dy,0+dz);
    glVertex3f(x1,y1+dy,0+dz);

    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x1,y1,0);
    glVertex3f(x1,y1+dy,0);
    glVertex3f(x1+dx,y1+dy,0);
    glVertex3f(x1+dx,y1,0);

    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x1,y1,0+dz);
    glVertex3f(x1,y1+dy,0+dz);
    glVertex3f(x1+dx,y1+dy,0+dz);
    glVertex3f(x1+dx,y1,0+dz);

    glEnd();
}

/** Generate the level_ structure
 * @brief MyGLWidget::generateLevel
 * @param level_Nb : the level_ id
 */
void MyGLWidget::generateLevel(int level_Nb){
    if (level_Nb < 1) level_Nb = 1;
    level_ = level_Nb;
    if (level_Nb > 7) level_Nb = level_Nb%7 + 1;
    bricks_.clear();
    for (float x = -12.5; x < 12; x+=2.2){
        for (float y = 2.0; y < 10; y+=1.7){
            // Generate a random R,G,B triplet
            int R,G,B;
            int randNum = rand()%(2) + 1;
            (randNum == 1) ? R = 255 : R = 0;
            randNum = rand()%(2) + 1;
            (randNum == 1) ? G = 255 : G = 0;
            randNum = rand()%(2) + 1;
            (randNum == 1 || (G==0 && R==0)) ? ((G!=255 || R!=255) ? B = 255 : B = 127) : B = 0;
            // Add the current brick to the list
            bricks_.push_back(Brick(x,y,R,G,B));
        }
    }
    if (level_Nb == 1){
        bricks_.erase(bricks_.begin()+2);
        bricks_.erase(bricks_.begin()+2);
        bricks_.erase(bricks_.begin()+2);
        bricks_.erase(bricks_.begin()+4);
        bricks_.erase(bricks_.begin()+4);
        bricks_.erase(bricks_.begin()+4);
        bricks_.erase(bricks_.begin()+7);
        bricks_.erase(bricks_.begin()+7);
        bricks_.erase(bricks_.begin()+9);
        bricks_.erase(bricks_.begin()+10);
        bricks_.erase(bricks_.begin()+11);
        bricks_.erase(bricks_.begin()+14);
        bricks_.erase(bricks_.begin()+18);
        bricks_.erase(bricks_.begin()+23);
        bricks_.erase(bricks_.begin()+28);
        bricks_.erase(bricks_.begin()+29);
        bricks_.erase(bricks_.begin()+32);
        bricks_.erase(bricks_.begin()+32);
        bricks_.erase(bricks_.begin()+34);
        bricks_.erase(bricks_.begin()+34);
        bricks_.erase(bricks_.begin()+34);
        bricks_.erase(bricks_.begin()+36);
        bricks_.erase(bricks_.begin()+36);
        bricks_.erase(bricks_.begin()+36);
    }
    else if(level_Nb == 2){
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+3);
        bricks_.erase(bricks_.begin()+4);
        bricks_.erase(bricks_.begin()+5);
        bricks_.erase(bricks_.begin()+8);
        bricks_.erase(bricks_.begin()+11);
        bricks_.erase(bricks_.begin()+12);
        bricks_.erase(bricks_.begin()+15);
        bricks_.erase(bricks_.begin()+18);
        bricks_.erase(bricks_.begin()+19);
        bricks_.erase(bricks_.begin()+21);
        bricks_.erase(bricks_.begin()+22);
        bricks_.erase(bricks_.begin()+25);
        bricks_.erase(bricks_.begin()+28);
        bricks_.erase(bricks_.begin()+29);
        bricks_.erase(bricks_.begin()+32);
        bricks_.erase(bricks_.begin()+35);
        bricks_.erase(bricks_.begin()+36);
        bricks_.erase(bricks_.begin()+37);
        bricks_.erase(bricks_.begin()+40);
    }
    else if (level_Nb == 3){
        bricks_.erase(bricks_.begin()+6);
        bricks_.erase(bricks_.begin()+6);
        bricks_.erase(bricks_.begin()+6);
        bricks_.erase(bricks_.begin()+8);
        bricks_.erase(bricks_.begin()+8);
        bricks_.erase(bricks_.begin()+8);
        bricks_.erase(bricks_.begin()+10);
        bricks_.erase(bricks_.begin()+10);
        bricks_.erase(bricks_.begin()+10);
        bricks_.erase(bricks_.begin()+17);
        bricks_.erase(bricks_.begin()+18);
        bricks_.erase(bricks_.begin()+20);
        bricks_.erase(bricks_.begin()+21);
        bricks_.erase(bricks_.begin()+28);
        bricks_.erase(bricks_.begin()+28);
        bricks_.erase(bricks_.begin()+28);
        bricks_.erase(bricks_.begin()+30);
        bricks_.erase(bricks_.begin()+30);
        bricks_.erase(bricks_.begin()+30);
        bricks_.erase(bricks_.begin()+32);
        bricks_.erase(bricks_.begin()+32);
        bricks_.erase(bricks_.begin()+32);
    }
    else if (level_Nb == 4){
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+1);
        bricks_.erase(bricks_.begin()+1);
        bricks_.erase(bricks_.begin()+1);
        bricks_.erase(bricks_.begin()+3);
        bricks_.erase(bricks_.begin()+3);
        bricks_.erase(bricks_.begin()+6);
        bricks_.erase(bricks_.begin()+15);
        bricks_.erase(bricks_.begin()+19);
        bricks_.erase(bricks_.begin()+28);
        bricks_.erase(bricks_.begin()+32);
        bricks_.erase(bricks_.begin()+32);
        bricks_.erase(bricks_.begin()+35);
        bricks_.erase(bricks_.begin()+35);
        bricks_.erase(bricks_.begin()+35);
        bricks_.erase(bricks_.begin()+37);
        bricks_.erase(bricks_.begin()+37);
        bricks_.erase(bricks_.begin()+37);
        bricks_.erase(bricks_.begin()+37);
    }
    else if (level_Nb == 5){
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+3);
        bricks_.erase(bricks_.begin()+18);
        bricks_.erase(bricks_.begin()+21);
        bricks_.erase(bricks_.begin()+21);
        bricks_.erase(bricks_.begin()+21);
        bricks_.erase(bricks_.begin()+22);
        bricks_.erase(bricks_.begin()+22);
        bricks_.erase(bricks_.begin()+22);
        bricks_.erase(bricks_.begin()+22);
        bricks_.erase(bricks_.begin()+23);
        bricks_.erase(bricks_.begin()+23);
        bricks_.erase(bricks_.begin()+23);
        bricks_.erase(bricks_.begin()+26);
        bricks_.erase(bricks_.begin()+41);
        bricks_.erase(bricks_.begin()+44);
    }
    else if (level_Nb == 6){
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+0);
        bricks_.erase(bricks_.begin()+11);
        bricks_.erase(bricks_.begin()+11);
        bricks_.erase(bricks_.begin()+11);
        bricks_.erase(bricks_.begin()+11);
        bricks_.erase(bricks_.begin()+13);
        bricks_.erase(bricks_.begin()+17);
        bricks_.erase(bricks_.begin()+18);
        bricks_.erase(bricks_.begin()+20);
        bricks_.erase(bricks_.begin()+21);
        bricks_.erase(bricks_.begin()+25);
        bricks_.erase(bricks_.begin()+32);
        bricks_.erase(bricks_.begin()+33);
        bricks_.erase(bricks_.begin()+35);
        bricks_.erase(bricks_.begin()+36);
        bricks_.erase(bricks_.begin()+38);
        bricks_.erase(bricks_.begin()+39);
    }
}
