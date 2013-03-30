#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv, QApplication::GuiServer);

    app.exec();
    return(0);
}
