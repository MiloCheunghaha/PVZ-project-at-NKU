#ifndef LEVEL_H
#define LEVEL_H

#include <QLabel>
#include <QTimer>

#include "zombie.h"
#include <QMediaPlaylist>
#include <QMediaPlayer>

class level :public QLabel
{
     Q_OBJECT
public:
    level();
    void startLevel();//游戏运行
    void resetLevel(int value);//改变游戏运行阶段
    void resetpausing(bool value);//改变游戏暂停状态
    int getLevel() const;//返回level
    bool getWaveLeader() const;
    void Levelmove();
    void timerEvent(QTimerEvent* event);
    void stopmusic();

private:
    int Level;//游戏运行阶段（1表示普通阶段，2表示尸潮）
    int levelTimer;//阶段计时器
    bool pausing;//游戏暂停状态
    bool WaveLeader;//用于制造旗帜僵尸

    //用于展示阶段
    QLabel LevelShower1;
    QLabel LevelShower2;
    QLabel LevelFlag1;
    QLabel LevelFlag2;
    QLabel zhongwen;

    QLabel LevelMoving;//游戏进行时
    QLabel zombiewave;
    QTimer *generateTimer;//僵尸生成计时器
    QMediaPlayer* hugewave_music;
    QMediaPlayer* finalwave_music;
signals:
    void end();
};

#endif // LEVEL_H
