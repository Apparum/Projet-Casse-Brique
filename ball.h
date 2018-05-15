#ifndef BALL_H
#define BALL_H

#include <GL/glu.h>

class Ball
{
public:
    // Constructor of an instance of Ball
    Ball();

    // Destructor of an instance of Ball
    virtual ~Ball();

    // Display the ball at x_,y_,z_ coords.
    void display();

    // Move the ball of Vx_ and Vy_
    void move();

    // Getters & Setters
    void setVx(float Vx){Vx_=Vx;}
    void setVy(float Vy){Vy_=Vy;}
    float getX(){return x_;}
    float getY(){return y_;}
    float getVx(){return Vx_;}
    float getVy(){return Vy_;}
    float getR(){return r_;}

    // Initialize the ball
    void init();

private:
    GLUquadric * ball_; // Ball Quadric
    float x_ = 0, y_ = -5, z_ = 0; // Initial position
    float r_ = 0.5; // Ball radius
    float Vx_ = (float)(rand()%1000 - 500)/(float)10000; // Horizontal speed (initialized with a random speed)
    float Vy_ = 0.08; // Vertical speed
};

#endif // BALL_H
