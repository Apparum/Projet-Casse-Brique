#include "ball.h"

Ball::Ball()
{
    // Creation de la quadrique
    ball_=gluNewQuadric();
}


Ball::~Ball()
{
    // Destruction de la quadrique
    gluDeleteQuadric(ball_);
}


void Ball::display()
{
    float color[] = {1.0,0.0,0.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
    glPushMatrix();
    glTranslatef(x_,y_,z_+1);
    gluSphere(ball_,r_,20,20);
    glPopMatrix();
}

void Ball::move(){
    x_ += Vx_;
    y_ += Vy_;
}

void Ball::init(){
    x_ = 0;
    y_ = -5;
    Vx_ = (float)(rand()%1000 - 500)/(float)10000;
    Vy_ = 0.08;
}
