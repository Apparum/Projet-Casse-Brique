#ifndef BRICK_H
#define BRICK_H

#include <GL/glu.h>

using namespace std;

// Classe dediee pour la gestion d'une brique
class Brick
{
public:
    // Constructeur avec parametres
    Brick(float x, float y, int R, int G, int B);

    // Destructeur
    virtual ~Brick();

    // Methode d'affichage
    void display() const;

    //Accesseurs
    float getX(){return x_;}
    float getY(){return y_;}
    float getdX(){return dx_;}
    float getdY(){return dy_;}

private:
    // Paramètres
    float x_,y_,z_ = 0;
    float dx_ = 2, dy_ = 1, dz_ = 2;
    int R_,G_,B_;
};


#endif // BRICK_H
