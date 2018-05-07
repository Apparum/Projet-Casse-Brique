#define _USE_MATH_DEFINES

#include "myglwidget.h"
#include "brick.h"
#include <QApplication>
#include <QDesktopWidget>
#include <cmath>
#include <random>
#include <QDebug>

// Declarations des constantes
const unsigned int WIN_WIDTH  = 640;
const unsigned int WIN_HEIGHT = 480;
const float MAX_DIMENSION     = 15.0f;

// Constructeur
MyGLWidget::MyGLWidget(QWidget * parent) : QGLWidget(parent)
{
    // Update the graphic timer
    connect(&animationTimer,  &QTimer::timeout, [&] {
        updateGL();
    });
    animationTimer.setInterval(10);
    animationTimer.start();

    // Update the timer of the game
    connect(&gameTimer,  &QTimer::timeout, [&] {
        updateGame();
    });
    gameTimer.setInterval(10);
    gameTimer.start();

    initGame();
}

void MyGLWidget::initGame(){
    // Initialisations des éléments du jeu
    generateLevel(level);
    ball_.init();
    puck_.init();
    ball_.setVx(ball_.getVx()*(level+10)/10);
    ball_.setVy(ball_.getVy()*(level+10)/10);
    averageX = 0;
    averageY = 0;
}
// Fonction d'initialisation
void MyGLWidget::initializeGL()
{
    // Reglage de la couleur de fond
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Activation du zbuffer
    glEnable(GL_DEPTH_TEST);
    // Reglage de la lampe
    glEnable(GL_LIGHT0);
    GLfloat color_ambiant[] = {1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,color_ambiant);
    GLfloat pos[] = {0.0f,0.0f,5.0f,1.0f};
    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    // Activation de la lumiere
    glEnable(GL_LIGHTING); // Allume la 1ere lumière
}

// Fonction de redimensionnement
void MyGLWidget::resizeGL(int width, int height)
{
    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, width, height);
    // Definition de la matrice de projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(width != 0)
        glOrtho(-MAX_DIMENSION, MAX_DIMENSION, -MAX_DIMENSION * height / static_cast<float>(width), MAX_DIMENSION * height / static_cast<float>(width), -MAX_DIMENSION * 2.0f, MAX_DIMENSION * 2.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Display function
void MyGLWidget::paintGL()
{
    // Buffer reset
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // Camera setup
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.5f, 1.0f, 10.0f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    // Display & Bounce
    QFont font("Monotype Corsiva", 16);
    if (gameState == 0){ // Playing
        for(unsigned int i=0;i<bricks.size();i++)
        {
            bricks[i].display();
            // If the ball_ is at the brick level in Y
            if((ball_.getY() + ball_.getR()) > (bricks[i].getY()) && (ball_.getY() - ball_.getR()) < (bricks[i].getY() + bricks[i].getdY()))
            {
                // If the ball_ is at the brick level in X
                if ((ball_.getX() + ball_.getR()) > bricks[i].getX() && (ball_.getX() - ball_.getR()) < (bricks[i].getX() + bricks[i].getdX()))
                {
                    float ymin = 0, ymax = 0, xmin = 0, xmax = 0;
                    (bricks[i].getY() > ball_.getY()) ? ymin = bricks[i].getY() : ymin = ball_.getY();
                    (bricks[i].getY()+bricks[i].getdY() <ball_.getY()+ball_.getR()) ? ymax = bricks[i].getY()+bricks[i].getdY() : ymax = ball_.getY()+ball_.getR();
                    (bricks[i].getX() > ball_.getX()) ? xmin = bricks[i].getX() : xmin = ball_.getX();
                    (bricks[i].getX()+bricks[i].getdX() < ball_.getX()+ball_.getR()) ? xmax = bricks[i].getX()+bricks[i].getdX() : xmax = ball_.getX()+ball_.getR();
                    float ysize = ymax - ymin;
                    float xsize = xmax - xmin;
                    int dir;
                    (xsize > ysize) ? ((ball_.getY() > bricks[i].getY()) ? dir = 3 : dir = 1) : ((ball_.getX() < bricks[i].getX()) ? dir = 0 : dir = 2);
                    bounceBrick(dir);
                    bricks.erase(bricks.begin() + i);
                    score ++;
                }
            }
        }
        ball_.display();
        puck_.display();

        // Create the walls and background
        displayWall(-14,1,-12.5,23.5,0.8,0.8,0.8, 2); // Left wall
        displayWall(14,1,-12.5,23.5,0.8,0.8,0.8, 2); // Right wall
        displayWall(-14,29,10,1,0.8,0.8,0.8, 2); // Top wall
        displayWall(-14,29,-13,1,0.8,0.8,0.8, 2); // Bottom wall
        displayWall(-13,27,-12,22,0.2,0.2,0.2,0); // Background

        // Put the life left indicator
        int x=0;
        for (int i=0; i<livesLeft-1; i++){
            GLUquadric* tmpQuad=gluNewQuadric();
            float color[] = {1.0,0.0,0.0};
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
            glPushMatrix();
            glTranslatef(x-6.3,12.5,4);
            gluSphere(tmpQuad,0.5,20,20);
            glPopMatrix();
            gluDeleteQuadric(tmpQuad);
            x += 1.1;
        }

        // Print text
        QString str("Lives Left :  ");
        renderText(-13, 10.8, -4, str, font);
        str = "Level : " + QString::number(level);
        renderText(-2, 10.8, -4, str, font);
        str = "Score : " + QString::number(score);
        renderText(7, 10.8, -4, str, font);
    }
    else if (gameState == 3){ // perdu
        QString str("You lose !");
        renderText(-5,0,-6,str,font);
        level = 1;
        score = 0;
        livesLeft = 3;
        gameTimer.stop();
        animationTimer.stop();
        qDebug() << "Défaite";
        gameState = 5;
    }
    else if (gameState == 4){ // gagné
        QString str("You Win !");
        renderText(-5,0,-6,str,font);
        level++;
        gameTimer.stop();
        animationTimer.stop();
        qDebug() << "Gagné";
        gameState = 5;
    }
    else { qDebug() << gameState;}
}

void MyGLWidget::updateGame(){
    // Rebond puck_
    if (animationTimer.isActive()){
        if((ball_.getY() - ball_.getR()) < (puck_.getY()+puck_.getdY()) && (ball_.getX() + ball_.getR()) > puck_.getX() && (ball_.getX() - ball_.getR()) < (puck_.getX() + puck_.getdX())) bouncePuck();
        puck_.move(averageX/50);
        if(ball_.getX()-ball_.getR()< -13 || ball_.getX()+ball_.getR() > 14) ball_.setVx(-ball_.getVx()); //Rebond sur mur gauche droite
        if(ball_.getY()+ball_.getR() > 10.5) ball_.setVy(-ball_.getVy()); //Rebond sur mur haut
        if(ball_.getY() < puck_.getY()){
            //ball_.setVy(-ball_.getVy());
            ball_.init();
            puck_.init();
            ball_.display();
            puck_.display();
            animationTimer.stop();
            livesLeft --;
        }
        ball_.move();
        if (vectX.size() > 60){
            averageX = accumulate( vectX.begin(), vectX.end(), 0.0)/vectX.size();
            vectX.erase(vectX.begin(), vectX.begin() + 40);
        }
        else {
            vectX.push_back(vect_.x);
        }
        if (vectY.size() > 60){
            averageY = accumulate( vectY.begin(), vectY.end(), 0.0)/vectY.size();
            vectY.erase(vectY.begin(), vectY.begin() + 40);
        }
        else {
            vectY.push_back(vect_.y);
        }
        // Inutile, dégrade la qualité du jeu mais présent dans le cahier des charges, "if" à commenter pour améliorer le jeu.
        if (abs(averageY) > 3*abs(averageX)) {
            qDebug() << averageY << "    " << averageX;
            averageX = 0;
            averageY = 0;
        }
        if (livesLeft == 0) gameState = 1;
        if (bricks.size() == 0) gameState = 2;
    }
}



void MyGLWidget::bouncePuck(){
    float alpha = atan(ball_.getX()-(puck_.getX()+puck_.getdX()/2));
    ball_.setVy((cos(alpha)/8)*(level+10)/10);
    ball_.setVx((sin(alpha)/8)*(level+10)/10);
}

void MyGLWidget::bounceBrick(int dir) {
    // dir 0: Left, 1: Top, 2: Right, 3: Bottom
    int mulx=1,muly=1;
    if (ball_.getVx() >= 0) {
        // ball_ is moving in the positive x direction
        if (ball_.getVy() > 0) {
            // ball_ is moving in the positive y direction
            (dir == 0 || dir == 3) ? mulx = -1 : muly = -1;
        } else if (ball_.getVy() < 0) {
            // ball_ is moving in the negative y direction
            (dir == 0 || dir == 1) ? mulx = -1 : muly = -1;
        }
    } else if (ball_.getVx() < 0) {
        // ball_ is moving in the negative x direction
        if (ball_.getVy() > 0) {
            // ball_ is moving in the positive y direction
            (dir == 2 || dir == 3) ? mulx = -1 : muly = -1;
        } else if (ball_.getVy() < 0) {
            // ball_ is moving in the negative y direction
            (dir == 1 || dir == 2) ? mulx = -1 : muly = -1;
        }
    }
    // Set the new direction of the ball_, by multiplying the old direction
    // with the determined direction factors
    ball_.setVx(ball_.getVx()*mulx);
    ball_.setVy(ball_.getVy()*muly);
}

void MyGLWidget::displayWall(float x1, float dx, float y1, float dy, float color1, float color2, float color3, float dz){
    glBegin(GL_QUADS);
    float color[] = {color1,color2,color3};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);

    glNormal3f(1.0,0.0,0.0);
    glVertex3f(x1,y1,0);
    glVertex3f(x1,y1+dy,0);
    glVertex3f(x1,y1+dy,0+dz);
    glVertex3f(x1,y1,0+dz);

    glNormal3f(1.0,0.0,0.0);
    glVertex3f(x1+dx,y1,0);
    glVertex3f(x1+dx,y1+dy,0);
    glVertex3f(x1+dx,y1+dy,0+dz);
    glVertex3f(x1+dx,y1,0+dz);

    glNormal3f(0.0,1.0,0.0);
    glVertex3f(x1,y1,0);
    glVertex3f(x1+dx,y1,0);
    glVertex3f(x1+dx,y1,0+dz);
    glVertex3f(x1,y1,0+dz);

    glNormal3f(0.0,1.0,0.0);
    glVertex3f(x1,y1+dy,0);
    glVertex3f(x1+dx,y1+dy,0);
    glVertex3f(x1+dx,y1+dy,0+dz);
    glVertex3f(x1,y1+dy,0+dz);

    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x1,y1,0);
    glVertex3f(x1,y1+dy,0);
    glVertex3f(x1+dx,y1+dy,0);
    glVertex3f(x1+dx,y1,0);

    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x1,y1,0+dz);
    glVertex3f(x1,y1+dy,0+dz);
    glVertex3f(x1+dx,y1+dy,0+dz);
    glVertex3f(x1+dx,y1,0+dz);

    glEnd();
}

void MyGLWidget::generateLevel(int levelNb){
    if (levelNb < 1) levelNb = 1;
    level = levelNb;
    if (levelNb > 2) levelNb = levelNb%2;
    bricks.clear();
    for (float x = -12.5; x < 12; x+=2.2){
        for (float y = 2.0; y < 10; y+=1.7){
            // Generate a random R,G,B triplet
            int R,G,B;
            int randNum = rand()%(2) + 1;
            (randNum == 1) ? R = 255 : R = 0;
            randNum = rand()%(2) + 1;
            (randNum == 1) ? G = 255 : G = 0;
            randNum = rand()%(2) + 1;
            (randNum == 1 || (G==0 && R==0)) ? ((G!=255 || R!=255) ? B = 255 : B = 127) : B = 0;
            // Add the current brick to the list
            bricks.push_back(Brick(x,y,R,G,B));
        }
    }
    if (levelNb == 1){
        bricks.erase(bricks.begin()+2);
        bricks.erase(bricks.begin()+2);
        bricks.erase(bricks.begin()+2);
        bricks.erase(bricks.begin()+4);
        bricks.erase(bricks.begin()+4);
        bricks.erase(bricks.begin()+4);
        bricks.erase(bricks.begin()+6);
        bricks.erase(bricks.begin()+6);
        bricks.erase(bricks.begin()+6);
        bricks.erase(bricks.begin()+8);
        bricks.erase(bricks.begin()+8);
        bricks.erase(bricks.begin()+8);

        bricks.erase(bricks.begin()+8);
        bricks.erase(bricks.begin()+8);
        bricks.erase(bricks.begin()+11);
        bricks.erase(bricks.begin()+11);
        bricks.erase(bricks.begin()+14);
        bricks.erase(bricks.begin()+14);
        bricks.erase(bricks.begin()+17);
        bricks.erase(bricks.begin()+17);

        bricks.erase(bricks.begin()+22);
        bricks.erase(bricks.begin()+22);
        bricks.erase(bricks.begin()+22);
        bricks.erase(bricks.begin()+24);
        bricks.erase(bricks.begin()+24);
        bricks.erase(bricks.begin()+24);
        bricks.erase(bricks.begin()+26);
        bricks.erase(bricks.begin()+26);
        bricks.erase(bricks.begin()+26);
        bricks.erase(bricks.begin()+28);
        bricks.erase(bricks.begin()+28);
        bricks.erase(bricks.begin()+28);
    }
    else if(levelNb == 2){
        bricks.erase(bricks.begin()+0);
        bricks.erase(bricks.begin()+1);
        bricks.erase(bricks.begin()+2);
        bricks.erase(bricks.begin()+3);
        bricks.erase(bricks.begin()+4);
        bricks.erase(bricks.begin()+5);
        bricks.erase(bricks.begin()+6);
        bricks.erase(bricks.begin()+7);
        bricks.erase(bricks.begin()+8);
        bricks.erase(bricks.begin()+9);
        bricks.erase(bricks.begin()+10);
        bricks.erase(bricks.begin()+11);
        bricks.erase(bricks.begin()+12);
        bricks.erase(bricks.begin()+13);
        bricks.erase(bricks.begin()+14);
        bricks.erase(bricks.begin()+15);
        bricks.erase(bricks.begin()+16);
        bricks.erase(bricks.begin()+17);
        bricks.erase(bricks.begin()+18);
        bricks.erase(bricks.begin()+19);
        bricks.erase(bricks.begin()+20);
        bricks.erase(bricks.begin()+21);
        bricks.erase(bricks.begin()+22);
        bricks.erase(bricks.begin()+23);
        bricks.erase(bricks.begin()+24);
        bricks.erase(bricks.begin()+25);
        bricks.erase(bricks.begin()+26);
        bricks.erase(bricks.begin()+27);
        bricks.erase(bricks.begin()+28);
        bricks.erase(bricks.begin()+29);
    }
}
