#ifndef PUCK_H
#define PUCK_H

#include <GL/glu.h>

class Puck
{
public:
    // Constructor of a puck instance
    Puck();

    // Destructor of a puck instance
    virtual ~Puck();

    // Display the puck at its coords
    void display() const;

    // Getters & Setters
    float getX(){return x_;}
    float getY(){return y_;}
    float getdX(){return dx_;}
    float getdY(){return dy_;}
    void setdX(float dx);

    // Methods
    // Move the puck of a given amount x
    void move(float x);
    // Initialize the puck at a specific place
    void init();

private:
    float dx_ = 6, dy_ = 1, dz_ = 1; // Size of the puck
    float x_ = -dx_/2, y_ = -10, z_ = 0; // Position of the puck
    float Vx_ = 0; // Horizontal speed of the puck
};

#endif // PUCK_H
