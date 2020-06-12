#include "psbrokerwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PSBrokerWidget w;
    w.show();

    return a.exec();
}
