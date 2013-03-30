#ifndef MOUSEEMULATOR_H
#define MOUSEEMULATOR_H

#include <QWSServer>
#include <QTime>
#include <QObject>
#include <QWSMouseHandler>
#include <QTimer>

class MouseEmulator : public QObject,  public QWSServer::KeyboardFilter, public QWSMouseHandler
{
    Q_OBJECT
public:
    MouseEmulator();
    ~MouseEmulator();

    virtual bool filter(int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat);
    virtual void suspend();
    virtual void resume();

protected slots:
    void moveTick();

private:
    bool _enabled;
    QTime *_buttontime;
    int _state;
    QTimer *_movetimer;
    bool _up;
    bool _down;
    bool _left;
    bool _right;
};

#endif // MOUSEEMULATOR_H
