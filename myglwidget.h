#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "brick.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "ball.h"
#include "puck.h"

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

// Classe dediee a l'affichage d'une scene OpenGL
class MyGLWidget : public QGLWidget{

    Q_OBJECT

public:
    // Constructeur
    MyGLWidget(QWidget * parent = nullptr);

    // Attributs
    QTimer animationTimer;
    QTimer gameTimer;

    // Méthodes
    void displayWall(float x1, float dx, float y1, float dy, float color1, float color2, float color3, float dz);
    void initGame();
    void bouncePuck();
    void bounceBrick(int dir);
    void generateLevel(int level);

    //Accesseurs
    void setVect(Point vect){vect_ = vect;}
    void setGameState(int state) {gameState = state;}
    int getGameState() {return gameState;}
    Puck getPuck() {return puck_;}
    void setPuck(Puck puck) {puck_ = puck;}

protected:
    // Fonction d'initialisation
    void initializeGL();

    // Fonction de redimensionnement
    void resizeGL(int width, int height);

    // Fonction d'affichage
    void paintGL();

private slots:
    void updateGame();

private:
    Ball ball_;
    Puck puck_;
    Point vect_;
    std::vector<float> vectX, vectY;
    std::vector<Brick> bricks;
    int gameState = 0; // 0 : Playing, 1 : Lose, 2 : Win, 3 : Lose Transition, 4 : Win Transition, 5 : Ready for Restart
    int score = 0;
    int level = 1;
    int livesLeft;
    float averageX = 0, averageY = 0;
};

#endif // MYGLWIDGET_H
