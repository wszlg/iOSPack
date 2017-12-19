#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();



    w.setWindowTitle("iOS-打包各种类型的ipa包______神经病院长");
    return a.exec();
}
