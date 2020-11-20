#include "paint.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(static_cast<unsigned int>(time(nullptr)));
    Paint w;
    w.show();

    return a.exec();
}
