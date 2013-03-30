#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QWidget>

namespace Ui {
    class MainPanel;
}

class MainPanel : public QWidget {
    Q_OBJECT
public:
    MainPanel(QWidget *parent = 0);
    ~MainPanel();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void updatePanel();
    void checkBatteryStatus();
    void checkWifiStatus();

private:
    Ui::MainPanel *ui;
    bool _charging;
    int _batpercent;
    int _wifipercent;
    bool _batwarn;
};

#endif // MAINPANEL_H
