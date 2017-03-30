#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <QFontDatabase>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //将字体文件名传给addApplicationFont,得到字体的Id
    int fontId = QFontDatabase::addApplicationFont(":/font/loli.ttf");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    QString msyh = QFontDatabase::applicationFontFamilies ( fontId ).at(0);
    QFont font(msyh,10);
    //将此字体设为QApplication的默认字体
    QApplication::setFont(font);
    MainWindow w;
    w.show();
    return a.exec();
}
