#include "zombie.h"
#include <QDebug>
#include <QTimer>
#include <QMediaPlaylist>
#include <QMediaPlayer>

Zombie::Zombie() : collide(false), alive(true), slowed(false), boomed(false), CountForDead(0)
{}

Zombie::Zombie(int type,int line):collide(false), alive(true), slowed(false), boomed(false), CountForDead(0)
{
    if_zombie_pause=false;
    setType(type);
    setLine(line);
    attackTimer=0;
}

//该基类不能直接加载gif，通过取帧达到动画效果
void Zombie::updateZombie()
{
    setPixmap(GifZombie->currentPixmap().scaled(225,225));
}


void Zombie::advance(int phase)
{
    if(if_zombie_pause) return;
    if(!phase) return;//不用管这一行
    //如果死亡，移出游戏界面
    if(!alive)
    {
        if(collide==true)
        {
            eat->stop();
        }
        if(CountForDead == 0 && !boomed)
        {
            speed = 0;
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/Zombie/ZombieDie.gif");
            GifZombie->start();
            CountForDead ++;
        }
        else if(CountForDead < 300)
        {
            CountForDead++;
        }
        else
        {
            this->setPos(10000,this->pos().y());
            return;
        }
    }
    updateZombie();//更新画面
    if(collide) return;//如果碰撞，则返回，不执行下一行的移动
    this->setPos(this->pos().x()-.1*speed,this->pos().y());
}

void Zombie::setType(int value)
{
    type_Zombie = value;
    switch (type_Zombie)
    {
    case Normal:
        setLife(100);
        setAttack(25);
        setAttackRate(1000);
        setSpeed(5);
        //声明QMovie 加载gif图像
        GifZombie = new QMovie(":/new/prefix1_zombies/image/zombie/Zombie/Zombie.gif");
        GifZombie->start();
        setPixmap(GifZombie->currentPixmap().scaled(225,225));
        break;
    case Conehead:
        setLife(200);
        setAttack(25);
        setAttackRate(1000);
        setSpeed(100);
        //声明QMovie 加载gif图像
        GifZombie = new QMovie(":/new/prefix1_zombies/image/zombie/ConeheadZombie/ConeheadZombie.gif");
        GifZombie->start();
        break;
    case Buckethead:
        setLife(300);
        setAttack(25);
        setAttackRate(1000);
        setSpeed(100);
        //声明QMovie 加载gif图像
        GifZombie = new QMovie(":/new/prefix1_zombies/image/zombie/BucketheadZombie/BucketheadZombie.gif");
        GifZombie->start();
        break;
    case Flag:
        setLife(100);
        setAttack(25);
        setAttackRate(1000);
        setSpeed(100);
        //声明QMovie 加载gif图像
        GifZombie = new QMovie(":/new/prefix1_zombies/image/zombie/FlagZombie/FlagZombie.gif");
        GifZombie->start();
        break;
    default:
        break;
    }
}

void Zombie::setLine(int value)
{
    line=value;
    switch (line) {
    case 1:
        setPos(1200,30);
        break;
    case 2:
        setPos(1200,175);
        break;
    case 3:
        setPos(1200,330);
        break;
    case 4:
        setPos(1200,465);
        break;
    case 5:
        setPos(1200,610);
        break;
    default:
        break;
    }
}

void Zombie::setLife(int value)
{
    life = value;
}
void Zombie::setAttack(int value)
{
    attack = value;
}

void Zombie::setAttackTimer(int value)
{
    attackTimer = value;
}


void Zombie::setAttackRate(int value)
{
    attackRate = value;
}
void Zombie::setSpeed(double value)
{
    speed = value;
}
void Zombie::setCollide(bool value)
{
    collide = value;
    attackTimer=0;//每次进入碰撞状态开始记时
    if(collide==true)
    {
        switch (type_Zombie)
        {
        case Normal:
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/Zombie/ZombieAttack.gif");
            GifZombie->start();
            break;
        case Conehead:
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/ConeheadZombie/ConeheadZombieAttack.gif");
            GifZombie->start();
            break;
        case Buckethead:
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/BucketheadZombie/BucketheadZombieAttack.gif");
            GifZombie->start();
            break;
        case Flag:
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/FlagZombie/FlagZombieAttack.gif");
            GifZombie->start();
            break;
        default:
            break;
        }
        eat_list = new QMediaPlaylist;
        eat_list->addMedia(QUrl("qrc:/new/prefix1/music/zombie/chompsoft.mp3"));
        eat_list->setCurrentIndex(1);
        eat_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        eat = new QMediaPlayer;
        eat->setPlaylist(eat_list);
        eat->setVolume(40);
        eat->play();
    }
    else
    {
        switch (type_Zombie)
        {
        case Normal:
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/Zombie/Zombie.gif");
            GifZombie->start();
            break;
        case Conehead:
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/ConeheadZombie/ConeheadZombie.gif");
            GifZombie->start();
            break;
        case Buckethead:
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/BucketheadZombie/BucketheadZombie.gif");
            GifZombie->start();
            break;
        case Flag:
            GifZombie->stop();
            GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/FlagZombie/FlagZombie.gif");
            GifZombie->start();
            break;
        default:
            break;
        }
        eat->stop();
    }

}
void Zombie::setAlive(bool value)
{
    alive = value;
}
void Zombie::setSlowed(bool value)
{
    slowed = value;
}

int Zombie::getLine() const
{
    return line;
}
int Zombie::getLife() const
{
    return life;
}
int Zombie::getAttack() const
{
    return attack;
}
int Zombie::getAttackTimer() const
{
    return attackTimer;
}

int Zombie::getAttackRate() const
{
    return attackRate;
}
double Zombie::getSpeed() const
{
    return speed;
}
bool Zombie::getCollide() const
{
    return collide;
}

bool Zombie::getAlive() const
{
    return alive;
}


bool Zombie::getSlowed() const
{
    return slowed;
}

int Zombie::getType() const
{
    return type_Zombie;
}

void Zombie::Changing_zombie_state()
{
    if(if_zombie_pause)
    {
        if_zombie_pause=false;

    }
    else
    {
        if_zombie_pause=true;
        if(collide==true)
        {
            eat->stop();
        }
    }
}

void Zombie::Zombieboomed()
{
    if(alive)
    {
        GifZombie->stop();
        GifZombie->setFileName(":/new/prefix1_zombies/image/zombie/Zombie/BoomDie.gif");
        GifZombie->start();
        boomed = true;
        alive = false;
        speed = 0;
        if(collide==true)
        {
            eat->stop();
        }
    }
}
//下面为曾经尝试过的重载paint与boundingRect函数以获得动画，但碰撞会受影响
//paint 调用
//void Zombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    if (GifZombie && GifZombie->state() == QMovie::Running)
//    {
////        qDebug()<<boundingRect();
//        painter->drawImage(boundingRect(), GifZombie->currentImage());
//    }
//}

//QRectF Zombie::boundingRect()const
//{
//    return QRectF(0,0,225,225);
//}

