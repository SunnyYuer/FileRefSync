#include "filediff.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileDiff w;
    w.show();

    return a.exec();
}
