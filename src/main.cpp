#include <QApplication>
#include "commonpanel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CommonPanel panel;
    panel.show();

    return a.exec();
}
