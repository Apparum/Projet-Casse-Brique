#include "brick.h"

using namespace std;

/** Create a brick instance
 * @brief Brick::Brick
 * @param x : horizontal position of the brick
 * @param y : vertical position of the brick
 * @param R : red value of the brick
 * @param G : green value of the brick
 * @param B : blue value of the brick
 */
Brick::Brick(float x, float y, int R, int G, int B)
{
    x_ = x;
    y_ = y;
    R_ = R;
    G_ = G;
    B_ = B;
}

/** Destructor of the brick instance
 * @brief Brick::~Brick
 */
Brick::~Brick()
{
}

/** Display a brick quad at x_,y_,z_ coords with the right color
 * @brief Brick::display
 */
void Brick::display() const
{
    glBegin(GL_QUADS);

    float color[] = {(float)R_/255,(float)G_/255,(float)B_/255};
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
