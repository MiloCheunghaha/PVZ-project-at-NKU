#include <QLabel>

#include "level.h"
#include "TheWar.h"
#include <QMediaPlaylist>
#include <QMediaPlayer>
level::level() :Level(1)
{
    finalwave_music = NULL;
    hugewave_music = NULL;
}

void level::startLevel()
{
    pausing = false;
    WaveLeader = false;
    TheWar* ptr = (TheWar*)parentWidget();
    //以下为初始化阶段页面
    LevelShower1.setParent(ptr);
    LevelShower1.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/FlagMeterEmpty.png"));
    LevelShower1.move(1000,800);
    LevelShower2.setParent(ptr);
    LevelShower2.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/FlagMeterFull.png"));
    LevelShower2.move(1000,800);
    LevelFlag1.setParent(ptr);
    LevelFlag1.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/FlagMeterParts2.png"));
    LevelFlag1.move(1000,795);
    LevelFlag2.setParent(ptr);
    LevelFlag2.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/FlagMeterParts2.png"));
    LevelFlag2.move(1070,795);
    LevelMoving.setParent(ptr);
    LevelMoving.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/FlagMeterParts1.png"));
    LevelMoving.move(1140,795);
    zhongwen.setParent(ptr);
    zhongwen.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/FlagMeterLevelProgress.png"));
    zhongwen.move(1035,815);
    zombiewave.setParent(ptr);
    zombiewave.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/boom.png"));
    zombiewave.hide();
    generateTimer = new QTimer(this);
    connect(generateTimer, SIGNAL(timeout()), ptr , SLOT(generateZombie()));
    generateTimer->start(15000);
    levelTimer = startTimer(50);
}

void level::resetLevel(int value)
{
    Level = value;
    if(Level == 1)
    {
        generateTimer->stop();
        generateTimer->start(15000);
    }
    else if (Level == 2)
    {
        generateTimer->stop();
        generateTimer->start(3000);
    }
}
void level::resetpausing(bool value)
{
    pausing = value;
}
int level::getLevel() const
{
    return Level;
}
bool level::getWaveLeader() const
{
    return WaveLeader;
}

void level::Levelmove()
{
    LevelMoving.move(LevelMoving.x() - 1,LevelMoving.y());
}

void level::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == levelTimer)
    {
        if(!pausing)
        {
            this->Levelmove();
            if(LevelMoving.x() == 1070)
            {
                zombiewave.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/LargeWave.gif"));//提示一大波僵尸
                //音效
                delete hugewave_music;
                hugewave_music = new QMediaPlayer;
                hugewave_music->setMedia(QUrl("qrc:/new/prefix1/music/war/hugewave.mp3"));
                hugewave_music->setVolume(35);
                hugewave_music->play();

                zombiewave.move(500,400);
                zombiewave.show();
                LevelFlag2.move(LevelFlag2.x(),LevelFlag2.y() - 10);
                int msec1 = 2000;
                QEventLoop loop1;
                QTimer::singleShot(msec1, &loop1, SLOT(quit()));
                loop1.exec();
                zombiewave.hide();
                WaveLeader = true;
                generateTimer->stop();
                generateTimer->start(1);
                int msec2 = 1;
                QEventLoop loop2;
                QTimer::singleShot(msec2, &loop2, SLOT(quit()));
                loop2.exec();
                WaveLeader = false;
                this->resetLevel(2);
                killTimer(levelTimer);
                levelTimer = startTimer(30000);
            }
            else if(LevelMoving.x() == 1069)
            {
                this->resetLevel(1);
                killTimer(levelTimer);
                levelTimer = startTimer(50);
            }
            else if(LevelMoving.x() < 1000)
            {
                zombiewave.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/FinalWave.gif"));//最终波僵尸
                //音效
                delete finalwave_music;
                finalwave_music = new QMediaPlayer;
                finalwave_music->setMedia(QUrl("qrc:/new/prefix1/music/war/finalwave.mp3"));
                finalwave_music->setVolume(35);
                finalwave_music->play();

                zombiewave.show();
                LevelFlag1.move(LevelFlag1.x(),LevelFlag1.y() - 10);
                int msec1 = 2000;
                QEventLoop loop1;
                QTimer::singleShot(msec1, &loop1, SLOT(quit()));
                loop1.exec();
                zombiewave.hide();
                this->resetLevel(2);
                generateTimer->stop();
                generateTimer->start(1);
                int msec2 = 1;
                QEventLoop loop2;
                QTimer::singleShot(msec2, &loop2, SLOT(quit()));
                loop2.exec();
                WaveLeader = false;
                this->resetLevel(2);
                killTimer(levelTimer);
                int msec3 = 30000;
                QEventLoop loop3;
                QTimer::singleShot(msec3, &loop3, SLOT(quit()));
                loop3.exec();
                generateTimer->stop();
                TheWar* ptr = (TheWar*)parentWidget();
                connect(this, &level::end, ptr, &TheWar::checkZombie);
                emit end();
            }
        }
    }
}

void level::stopmusic()
{
    delete hugewave_music;
    delete finalwave_music;

}
