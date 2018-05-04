#include "brick.h"

using namespace std;

Brick::Brick(float x, float y, int R, int G, int B)
{
    // Initialisation des paramètres
    x_ = x;
    y_ = y;
    R_ = R;
    G_ = G;
    B_ = B;
}


Brick::~Brick()
{
}


void Brick::display() const
{
    glBegin(GL_QUADS);

    float color[] = {R_/255,G_/255,B_/255};
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
