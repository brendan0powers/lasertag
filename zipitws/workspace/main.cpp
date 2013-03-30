#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QWSServer>
#include <QWidget>
#include <QBrush>
#include <QLinearGradient>
#include <stdlib.h>
#include "mainpanel.h"
#include "mouseemulator.h"

int main(int argc, char *argv[])
{
#ifdef __ZIPIT__
    //Make sure that when the app starts on a zipit that it has the right keymap and screen orientation
    setenv("QWS_DISPLAY","transformed:Rot90");
    setenv("QWS_KEYBOARD","tty:keymap=/opt/zipitws/share/z2.qmap");
#endif

    QApplication app(argc,argv, QApplication::GuiServer);

    QLinearGradient gradient(QPoint(0,0),QPoint(0,240));
    gradient.setColorAt(0,QColor(44,113,165));
    gradient.setColorAt(1,QColor(30,73,115));
    QBrush brush(gradient);
    QWSServer::setBackground(brush);

    QWSServer::setMaxWindowRect(QRect(0,16,320,240));
    qDebug() << "Screen Side:" << QDesktopWidget().availableGeometry();

    MouseEmulator mouse;
    QWSServer::addKeyboardFilter(&mouse);
    QWSServer::setMouseHandler(&mouse);

    MainPanel panel(NULL);
    panel.show();
    panel.setGeometry(QRect(0,0,320,16));

    app.exec();
    return(0);
}
