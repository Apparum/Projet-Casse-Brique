#include "ball.h"

/** Create a new Quadric
 * @brief Ball::Ball
 */
Ball::Ball()
{
    ball_=gluNewQuadric();
}

/** Destroy the quadric
 * @brief Ball::~Ball
 */
Ball::~Ball()
{
    gluDeleteQuadric(ball_);
}

/** Display the ball at x_,y_,z_ coords.
 * @brief Ball::display
 */
void Ball::display()
{
    float color[] = {1.0,0.0,0.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
    glPushMatrix();
    glTranslatef(x_,y_,z_+1);
    gluSphere(ball_,r_,20,20);
    glPopMatrix();
}

/** Move the ball of Vx_ and Vy_
 * @brief Ball::move
 */
void Ball::move(){
    x_ += Vx_;
    y_ += Vy_;
}

/** Initialize the ball parameters
 * @brief Ball::init
 */
void Ball::init(){
    x_ = 0;
    y_ = -5;
    Vx_ = (float)(rand()%1000 - 500)/(float)10000;
    Vy_ = 0.08;
}
