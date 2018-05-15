#include "puck.h"
#include <QGLWidget>

/** Create a puck instance
 * @brief Puck::Puck
 */
Puck::Puck()
{
}

/** Destroy the puck instance
 * @brief Puck::~Puck
 */
Puck::~Puck()
{
}

/** Display the puck
 * @brief Puck::display
 */
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

/** Move the puck of x
 * @brief Puck::move
 * @param x : the horizontal variation of the puck
 */
void Puck::move(float x){
    if ((x < 0 && x_+x > -13) || (x > 0 && x_ + x + dx_ < 14)) x_ += x;
}

/** Initialize the puck
 * @brief Puck::init
 */
void Puck::init(){
    x_ = -dx_/2, y_ = -10, z_ = 0;
    Vx_ = 0;
}

/** Set the size of the puck
 * @brief Puck::setdX
 * @param dx : the size of the puck
 */
void Puck::setdX(float dx){
    if (dx >= 3 && dx <= 20){
        x_ = -dx/2;
        dx_ = dx;
    }
}
