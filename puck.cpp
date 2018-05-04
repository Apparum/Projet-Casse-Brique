#include "puck.h"
#include <QGLWidget>

Puck::Puck()
{
}

Puck::~Puck()
{
}

void Puck::display() const
{
    glBegin(GL_QUADS);

    float color[] = {1.0,0.5,0.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);

    glNormal3f(1.0,0.0,0.0);
    glVertex3f(x_,y_,z_);
    glVertex3f(x_,y_+dy_,z_);
    glVertex3f(x_,y_+dy_,z_+dz_);
    glVertex3f(x_,y_,z_+dz_);

    glNormal3f(1.0,0.0,0.0);
    glVertex3f(x_+dx_,y_,z_);
    glVertex3f(x_+dx_,y_+dy_,z_);
    glVertex3f(x_+dx_,y_+dy_,z_+dz_);
    glVertex3f(x_+dx_,y_,z_+dz_);

    glNormal3f(0.0,1.0,0.0);
    glVertex3f(x_,y_,z_);
    glVertex3f(x_+dx_,y_,z_);
    glVertex3f(x_+dx_,y_,z_+dz_);
    glVertex3f(x_,y_,z_+dz_);

    glNormal3f(0.0,1.0,0.0);
    glVertex3f(x_,y_+dy_,z_);
    glVertex3f(x_+dx_,y_+dy_,z_);
    glVertex3f(x_+dx_,y_+dy_,z_+dz_);
    glVertex3f(x_,y_+dy_,z_+dz_);

    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x_,y_,z_);
    glVertex3f(x_,y_+dy_,z_);
    glVertex3f(x_+dx_,y_+dy_,z_);
    glVertex3f(x_+dx_,y_,z_);

    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x_,y_,z_+dz_);
    glVertex3f(x_,y_+dy_,z_+dz_);
    glVertex3f(x_+dx_,y_+dy_,z_+dz_);
    glVertex3f(x_+dx_,y_,z_+dz_);

    glEnd();
}

void Puck::move(float x){
    if ((x < 0 && x_+x > -13) || (x > 0 && x_ + x + dx_ < 14)) x_ += x;
}

void Puck::init(){
    dx_ = 6, dy_ = 1, dz_ = 1;
    x_ = -dx_/2, y_ = -10, z_ = 0;
    Vx_ = 0;
}

void Puck::setdX(float dx){
    if (dx >= 3 && dx <= 20) dx_ = dx;
}
