#include "mouseemulator.h"
#include <QDebug>
#include <QWSServer>

#ifdef __ZIPIT__
    #define MOUSEBUTTON Qt::Key_Eject
    #define MOUSEBUTTON_LEFT Qt::Key_Back
    #define MOUSEBUTTON_LEFT1 Qt::Key_Enter
    #define MOUSEBUTTON_RIGHT Qt::Key_Next
#else
    #define MOUSEBUTTON Qt::Key_Escape
    #define MOUSEBUTTON_LEFT Qt::Key_F1
    #define MOUSEBUTTON_LEFT1 Qt::Key_F3
    #define MOUSEBUTTON_RIGHT Qt::Key_F2
#endif

MouseEmulator::MouseEmulator()
    : QObject(NULL), QWSServer::KeyboardFilter(), QWSMouseHandler("emulated","emulated")
{
    _enabled = false;
    _buttontime = new QTime();
    _state = 0;
    _up = false;
    _down = false;
    _left = false;
    _right = false;
    _movetimer = new QTimer(this);
    connect(_movetimer,SIGNAL(timeout()),SLOT(moveTick()));

    QWSServer::setCursorVisible(false);
}

MouseEmulator::~MouseEmulator()
{
    delete _buttontime;
}

bool MouseEmulator::filter(int, int keycode, int, bool isPress, bool autoRepeat)
{
    //Event Filter
    if((keycode == Qt::Key_Up) || (keycode == Qt::Key_Down) || (keycode == Qt::Key_Left) || (keycode == Qt::Key_Right) ||
        (keycode == MOUSEBUTTON) || (keycode == MOUSEBUTTON_LEFT) || (keycode == MOUSEBUTTON_LEFT1) ||
        (keycode == MOUSEBUTTON_RIGHT))
    {
        if(autoRepeat) return(true); //Ignore auto repeate events
    }
    else
    {
        return(false); //Forward other key events
    }


    //Mouse Button Pressed
    if((keycode == MOUSEBUTTON) && isPress)
    {
        _buttontime->start();
        if(_enabled)
        {
            QWSServer::setCursorVisible(false);
            _enabled = false;
            qDebug() << "Mouse Emulation Disabled";
            _movetimer->stop();
        }
        else
        {
            qDebug() << "Mouse Emulation Enabled";
            QWSServer::setCursorVisible(true);
            _enabled = true;
            _movetimer->start(33);
        }
    }

    //Mouse Button Released
    if((keycode == MOUSEBUTTON) && !isPress)
    {
        if(_buttontime->elapsed() > 500)
        {
            qDebug() << "Mouse Emulation Disabled";
            QWSServer::setCursorVisible(false);
            _enabled = false;
            _movetimer->stop();
        }
    }

    if(keycode == Qt::Key_Up) _up = isPress;
    else if(keycode == Qt::Key_Down) _down = isPress;
    else if(keycode == Qt::Key_Left) _left = isPress;
    else if(keycode == Qt::Key_Right) _right = isPress;

    //Mouse emulation is enabled, do stuff
    if(_enabled)
    {
        //Button state change event
        if((keycode == MOUSEBUTTON_LEFT) || (keycode == MOUSEBUTTON_LEFT1) || (keycode == MOUSEBUTTON_RIGHT))
        {
            if(isPress)
            {
                if((keycode == MOUSEBUTTON_LEFT) || (keycode == MOUSEBUTTON_LEFT1)) _state |= Qt::LeftButton;
                if(keycode == MOUSEBUTTON_RIGHT) _state |= Qt::RightButton;
            }
            else
            {
                if((keycode == MOUSEBUTTON_LEFT) || (keycode == MOUSEBUTTON_LEFT1)) _state &= !Qt::LeftButton;
                if(keycode == MOUSEBUTTON_RIGHT) _state &= !Qt::RightButton;
            }

            mouseChanged(pos(),_state);
        }

        return(true);
    }

    return(false);
}

void MouseEmulator::moveTick()
{
    QPoint p = pos();
    if(_up) p.setY(p.y() - 3);
    if(_down) p.setY(p.y() + 3);
    if(_left) p.setX(p.x() - 3);
    if(_right) p.setX(p.x() + 3);

    limitToScreen(p);

    //qDebug() << "Move: " << p << _up << _down << _left << _right;
    mouseChanged(p,_state);

}

void MouseEmulator::suspend()
{
    //Does Nothing

}

void MouseEmulator::resume()
{
    //Does Nothing
}
