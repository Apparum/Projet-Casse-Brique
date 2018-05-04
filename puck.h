#ifndef PUCK_H
#define PUCK_H

#include <GL/glu.h>

class Puck
{
public:
    // Constructeur avec parametres
    Puck();

    // Destructeur
    virtual ~Puck();

    // Methode d'affichage
    void display() const;

    //Accesseurs
    float getX(){return x_;}
    float getY(){return y_;}
    float getdX(){return dx_;}
    float getdY(){return dy_;}
    void setdX(float dx);

    // Méthodes
    void move(float x);
    void init();
private:
    // Paramètres
    float dx_ = 6, dy_ = 1, dz_ = 1;
    float x_ = -dx_/2, y_ = -10, z_ = 0;
    float Vx_ = 0;
};

#endif // PUCK_H
