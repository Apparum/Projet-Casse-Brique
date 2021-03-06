#ifndef BRICK_H
#define BRICK_H

#include <GL/glu.h>

using namespace std;

class Brick
{
public:
    // Constructor of the brick, having the coords and the color of the brick
    Brick(float x, float y, int R, int G, int B);

    // Destructor of the brick instance
    virtual ~Brick();

    // Display the brick at the corresponding coords
    void display() const;

    // Getters & Setters
    float getX(){return x_;}
    float getY(){return y_;}
    float getdX(){return dx_;}
    float getdY(){return dy_;}

private:
    float x_,y_,z_ = 0; // Position of the brick
    float dx_ = 2, dy_ = 1, dz_ = 2; // Size of the brick
    int R_,G_,B_; // Color of the brick
};


#endif // BRICK_H
