#include "mainpanel.h"
#include "ui_mainpanel.h"
#include <QPalette>
#include <QLinearGradient>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QPixmap>

MainPanel::MainPanel(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint),
    ui(new Ui::MainPanel)
{
    ui->setupUi(this);

    QPalette pal = palette();
    QColor color = pal.color(QPalette::Window);
    QLinearGradient gradient(QPoint(0,0), QPoint(0,16));
    gradient.setColorAt(0,color);
    color.setRed(color.red()-50);
    color.setGreen(color.green()-50);
    color.setBlue(color.blue()-50);
    gradient.setColorAt(1,color);
    pal.setBrush(QPalette::Window, QBrush(gradient));
    setPalette(pal);

    QTimer *timer = new QTimer(this);
    this->connect(timer,SIGNAL(timeout()),SLOT(updatePanel()));
    timer->start(1000);
    updatePanel();
    _batwarn = true;
}

MainPanel::~MainPanel()
{
    delete ui;
}

void MainPanel::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainPanel::updatePanel()
{
    //Clock
    ui->ClockLabel->setText(QTime::currentTime().toString("h:mm AP"));

    //Check Status of battery and wifi
    bool oldcharging = _charging;
    int oldbatpercent = _batpercent;
    int oldwifipercent = _wifipercent;
    checkBatteryStatus();
    checkWifiStatus();

    qDebug() << _charging << _batpercent << _wifipercent;

    //BATTERY
    if((oldcharging != _charging) || (oldbatpercent != _batpercent))
    {
        qDebug() << "Updating Battery";

        //break the battery percent into segments
        int percent = 100;
        if(_batpercent <= 80) percent = 80;
        if(_batpercent <= 60) percent = 60;
        if(_batpercent <= 40) percent = 40;
        if(_batpercent <= 20) percent = 20;
        if(_batpercent <= 5) percent = 0;

        QString filename;

        //if the battery isn't charging, and the power is very low, flash a warning
        if(!_charging && (percent == 0))
        {
            if(_batwarn)
            {
                filename = "/opt/zipitws/share/icons/bat-warn-0.png";
            }
            else
            {
                filename = "/opt/zipitws/share/icons/bat-warn-1.png";
            }
        }
        else
        {
            filename = QString("/opt/zipitws/share/icons/bat-%1%2.png").arg(percent).arg(_charging ? "-charging" : "");
        }

        ui->BatteryLabel->setPixmap(QPixmap(filename));
    }

    //WIFI
    if(oldwifipercent != _wifipercent)
    {
        qDebug() << "Updating Wifi";

        int percent = 100;
        if(_wifipercent <= 80) percent = 80;
        if(_wifipercent <= 60) percent = 60;
        if(_wifipercent <= 40) percent = 40;
        if(_wifipercent <= 20) percent = 20;
        if(_wifipercent <= 5) percent = 0;

        QString filename;

        if(percent == 0)
        {
            if(_batwarn)
            {
                filename = "/opt/zipitws/share/icons/wifi-nosignal-0.png";
            }
            else
            {
                filename = "/opt/zipitws/share/icons/wifi-nosignal-1.png";
            }
        }
        else
        {
           filename = QString("/opt/zipitws/share/icons/wifi-%1.png").arg(percent);
        }

        ui->WifiLabel->setPixmap(QPixmap(filename));
    }

    _batwarn = !_batwarn;
}

void MainPanel::checkBatteryStatus()
{
    QFile file("/home/brendan/zipitws/bin/apm");
    if(!file.open(QFile::ReadOnly))
    {
        _charging = true;
        _batpercent = 0;
        return;
    }

    QString line = file.readLine();
    line = line.replace("%","");
    QStringList parts = line.split(" ",QString::SkipEmptyParts);

    if(parts.size() < 7)
    {
        _charging = true;
        _batpercent = 0;
        return;
    }

    if(parts[3] == "0x00") _charging = false;
    else _charging=true;

    bool ok;
    _batpercent = parts[6].toInt(&ok);
    if(!ok)
    {
        _charging = true;
        _batpercent = 0;
    }
}

void MainPanel::checkWifiStatus()
{
    QFile file("/home/brendan/zipitws/bin/wireless");
    if(!file.open(QFile::ReadOnly))
    {
        _wifipercent = 0;
        return;
    }

    //skip 2 lines
    file.readLine();
    file.readLine();
    QString line = file.readLine();
    line = line.replace(".","");
    QStringList parts = line.split(" ",QString::SkipEmptyParts);

    if(parts.size() < 3)
    {
        _wifipercent = 0;
        return;
    }

    bool ok;
    _wifipercent = parts[2].toInt(&ok);
    if(!ok)
    {
        _wifipercent = 0;
    }
}
