#include "breakout.h"
#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    // Prepare the random seed
    srand(time(NULL));
    QApplication a(argc, argv);
    Breakout w;
    w.show();

    return a.exec();
}
