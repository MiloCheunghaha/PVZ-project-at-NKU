#ifndef WELCOME_H
#define WELCOME_H

#include "TheWar.h"
#include <QWidget>
#include<QSlider>
#include<QSpinBox>
#include <QMediaPlaylist>
#include <QMediaPlayer>
QT_BEGIN_NAMESPACE
namespace Ui { class Welcome; }
QT_END_NAMESPACE

class Welcome : public QWidget
{
    Q_OBJECT

public:
    Welcome(QWidget *parent = nullptr);
    ~Welcome();

    void buttonStartGame();//初始化按钮
    void startGame();//槽函数
    void returnMenu();//返回函数
    void paintEvent(QPaintEvent *event);
    void adjustment();

signals:
    void SignalStartGame();//开始游戏信号
private:
    Ui::Welcome *ui;
    TheWar thewar[10];
    int war_conts;
    QSlider *pSlider;
    QMediaPlayer *welcome_music;
    QMediaPlaylist *welcome_music_list;
    QMediaPlayer *button_music;
};
#endif // WELCOME_H
