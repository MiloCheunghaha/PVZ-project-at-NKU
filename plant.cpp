#include "plant.h"
#include <QMediaPlaylist>
#include <QMediaPlayer>
Plant::Plant() : alive(true),collide(false),isAttacking(false)
{
}

Plant::Plant(int type,int line,int column):alive(true),collide(false),isAttacking(false)
{
    if_plant_pause=false;
    setType(type);
    setPosition(line,column);
    attackTimer=0;
    size=120;
}

void Plant::updatePlant()
{
    //如果是Wallnut，根据生命值变换图像
    if(type_Plant==Wallnut)
    {
        if(life<=200)
        {
            GifPlant->stop();
            GifPlant->setFileName(":/new/prefix1_plants/image/plant/WallNut/Wallnut_cracked2.gif");
            GifPlant->start();
        }
        else if(life<=400)
        {
            GifPlant->stop();
            GifPlant->setFileName(":/new/prefix1_plants/image/plant/WallNut/Wallnut_cracked1.gif");
            GifPlant->start();
        }
    }
    setPixmap(GifPlant->currentPixmap().scaled(size,size));//更新图像
}

void Plant::advance(int phase)
{
    if(if_plant_pause) return;
    if(!phase) return;
    //如果死亡，移出游戏界面，死亡与受攻击部分待完成
    if(!alive)
    {
        this->setPos(10000,this->pos().y());
        return;
    }
    updatePlant();
}

void Plant::setType(int value)
{
    type_Plant = value;
    switch (type_Plant)
    {
    case PeaShooter:
        setLife(100);
        setAttack(25);
        setAttackRate(3000);
        GifPlant = new QMovie(":/new/prefix1_plants/image/plant/Peashooter/Peashooter.gif");
        GifPlant->start();
        updatePlant();
        break;
    case CherryBomb:
        setLife(1);
        setAttack(999);
        setAttackRate(100);
        GifPlant = new QMovie(":/new/prefix1_plants/image/plant/CherryBomb/CherryBomb.gif");
        GifPlant->start();
        updatePlant();
        CherryBoomTimer=startTimer(650);
        CherryBooming = false;
        break;
    case SnowPea:
        setLife(100);
        setAttack(25);
        setAttackRate(3000);
        GifPlant = new QMovie(":/new/prefix1_plants/image/plant/SnowPea/SnowPea.gif");
        GifPlant->start();
        updatePlant();
        break;
    case PotatoMine:
        setLife(100);
        setAttack(999);
        setAttackRate(50);
        GifPlant = new QMovie(":/new/prefix1_plants/image/plant/PotatoMine/PotatoMineNotReady.gif");
        GifPlant->start();
        potatomine_ready_music = new QMediaPlayer;
        potatomine_ready_music->setMedia(QUrl("qrc:/new/prefix1/music/plant/wakeup.mp3"));
        potatomine_ready_music->setVolume(50);
        updatePlant();
        PotatoMineReadyTimer=startTimer(2000);
        PotatoMineReady=false;
        PotatoMineBoomed = false;
        break;
    case Wallnut:
        setLife(600);
        setAttack(0);
        setAttackRate(0);
        GifPlant = new QMovie(":/new/prefix1_plants/image/plant/WallNut/WallNut.gif");
        GifPlant->start();
        updatePlant();
        break;
    case SunFlower:
        setLife(100);
        setAttack(0);
        setAttackRate(5000);//指生成阳光的速率
        setIsAttacking(true);//向日葵一直产生太阳
        GifPlant = new QMovie(":/new/prefix1_plants/image/plant/SunFlower/SunFlower.gif");
        GifPlant->start();
        updatePlant();
        break;
    default:
        break;
    }
}

void Plant::setPosition(int line, int column)
{
    Line=line;
    Column=column;
    switch (Line) {
    case 1:
        setPos(0,125);
        break;
    case 2:
        setPos(0,265);
        break;
    case 3:
        setPos(0,410);
        break;
    case 4:
        setPos(0,560);
        break;
    case 5:
        setPos(0,705);
        break;
    default:
        break;
    }
    switch (Column) {
    case 1:
        setPos(100,this->y());
        break;
    case 2:
        setPos(220,this->y());
        break;
    case 3:
        setPos(345,this->y());
        break;
    case 4:
        setPos(470,this->y());
        break;
    case 5:
        setPos(585,this->y());
        break;
    case 6:
        setPos(710,this->y());
        break;
    case 7:
        setPos(830,this->y());
        break;
    case 8:
        setPos(945,this->y());
        break;
    case 9:
        setPos(1080,this->y());
        break;
    default:
        break;
    }
}

void Plant::timerEvent(QTimerEvent* event)
{
    //樱桃炸弹更换爆炸gif
    if (event->timerId() == CherryBoomTimer)
    {
        killTimer(CherryBoomTimer);
        GifPlant->stop();
        GifPlant->setFileName(":/new/prefix1_plants/image/plant/CherryBomb/Boom.gif");
        GifPlant->start();
        size=180;
        this->setCherryBooming(true);
        int msec1 = 50;
        QEventLoop loop1;
        QTimer::singleShot(msec1, &loop1, SLOT(quit()));
        loop1.exec();
        this->setCherryBooming(false);
        int msec2 = 800;
        QEventLoop loop2;
        QTimer::singleShot(msec2, &loop2, SLOT(quit()));
        loop2.exec();
        setAlive(false);
        hide();
    }
    //土豆类更换准备好gif
    if (event->timerId() == PotatoMineReadyTimer)
    {
        if(!PotatoMineReady)
        {
            delete potatomine_ready_music;
            potatomine_ready_music = new QMediaPlayer;
            potatomine_ready_music->setMedia(QUrl("qrc:/new/prefix1/music/plant/wakeup.mp3"));
            potatomine_ready_music->setVolume(50);
            potatomine_ready_music->play();
            PotatoMineReady = true;
            killTimer(PotatoMineReadyTimer);
            PotatoMineReadyTimer = startTimer(50);
            GifPlant->stop();
            GifPlant->setFileName(":/new/prefix1_plants/image/plant/PotatoMine/PotatoMine.gif");
            GifPlant->start();
            if_pb_music=true;
        }
        else if (PotatoMineBoomed)
        {
            GifPlant->stop();
            GifPlant->setFileName(":/new/prefix1_plants/image/plant/PotatoMine/PotatoMine_mashed.gif");
            GifPlant->start();
            int msec = 500;
            QEventLoop loop;
            QTimer::singleShot(msec, &loop, SLOT(quit()));
            loop.exec();
            setAlive(false);
        }
    }
}

vector<Zombie *> Plant::getCollideZombies() const
{
    return collideZombies;
}
void Plant::addCollideZombie(Zombie *zombie)
{
    collideZombies.push_back(zombie);
}

void Plant::setAttack(int value)
{
    attack=value;
}
void Plant::setAttackTimer(int value)
{
    attackTimer=value;
}

void Plant::setAlive(bool value)
{
    alive = value;
}
void Plant::setCollideZombies(const vector<Zombie *> &value)
{
    collideZombies = value;
}
void Plant::setCollide(bool value)
{
    collide = value;
}
void Plant::setIsAttacking(bool value)
{
    isAttacking=value;
    attackTimer=0;
    //    qDebug()<<"isAttacking:"<<isAttacking;
}

void Plant::setCost(int value)
{
    cost = value;
}

void Plant::setLife(double value)
{
    life = value;
}
void Plant::setCD(int value)
{
    CDtime = value;
}
void Plant::setSun(int value)
{
    sun = value;
}
void Plant::setAttackRate(int value)
{
    attackRate = value;
}
void Plant::setPotatoMineBoomed(bool value)
{
    PotatoMineBoomed = value;
}
void Plant::setCherryBooming(bool value)
{
    CherryBooming = value;
}

int Plant::getAttack() const
{
    return attack;
}
int Plant::getAttackTimer() const
{
    return attackTimer;
}

int Plant::getAttackRate() const
{
    return attackRate;
}
bool Plant::getAlive() const
{
    return alive;
}
bool Plant::getCollide() const
{
    return collide;
}
bool Plant::getPotatoMineReady() const
{
    return PotatoMineReady;
}
bool Plant::getIsAttacking()const
{
    return isAttacking;
}
bool Plant::getCherryBooming() const
{
    return CherryBooming;
}

int Plant::getType() const
{
    return type_Plant;
}
int Plant::getCost() const
{
    return cost;
}
int Plant::getLine()const
{
    return Line;
}
int Plant::getColumn()const
{
    return Column;
}

double Plant::getLife()
{
    return life;
}
double Plant::getCD() const
{
    return CDtime;
}
int Plant::getSun() const
{
    return sun;
}
void Plant::Changing_plant_state()
{
    if(if_plant_pause)
    {
        if_plant_pause=false;
    }
    else
    {
        if_plant_pause=true;
    }
}
bool Plant::get_if_pb_music() const
{
    return if_pb_music;
}

