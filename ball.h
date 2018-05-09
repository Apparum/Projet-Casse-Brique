#ifndef BALL_H
#define BALL_H

#include <GL/glu.h>

class Ball
{
public:
    // Constructeur avec parametres
    Ball();

    // Destructeur
    virtual ~Ball();

    // Methode d'affichage
    void display();

    // Movement
    void move();

    // Getters & Setters
    void setVx(float Vx){Vx_=Vx;}
    void setVy(float Vy){Vy_=Vy;}
    float getX(){return x_;}
    float getY(){return y_;}
    float getVx(){return Vx_;}
    float getVy(){return Vy_;}
    float getR(){return r_;}

    void init();

private:
    // Paramètres
    GLUquadric * ball_;
    float x_ = 0, y_ = -5, z_ = 0;
    float r_ = 0.5;
    float Vx_ = (float)(rand()%1000 - 500)/(float)10000;
    float Vy_ = 0.08;
};

#endif // BALL_H
