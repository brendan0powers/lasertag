#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    qDebug() << "Screen Side:" << QDesktopWidget().availableGeometry();

    app.exec();
    return(0);
}
