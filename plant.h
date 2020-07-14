#ifndef PLANT_H
#define PLANT_H

#include "zombie.h"
#include <QGraphicsPixmapItem>
#include <vector>
#include<QTimer>
#include<QObject>
#include <QMediaPlaylist>
#include <QMediaPlayer>
using std::vector;

class Plant:public QObject ,public QGraphicsPixmapItem
{
private:
    int type_Plant; //植物种类
    double life;//生命值
    int cost;//价格
    int attack;//攻击力
    int attackTimer;//碰撞状态持续时间，用于与攻击速度取余决定是否攻击
    int attackRate;//攻速
    double CDtime;//冷却时间
    int sun;//向日葵专用
    bool alive;//存活状态
    bool collide;//碰撞状态
    bool isAttacking;//攻击状态
    int Line;//行
    int Column;//列
    int size;//图片显示大小,为了让樱桃炸弹变大而改
    int CherryBoomTimer;//樱桃炸弹爆炸倒计时
    int PotatoMineReadyTimer;//土豆雷准备倒计时
    bool PotatoMineReady;//土豆类准备状态
    bool CherryBooming;//樱桃炸弹爆炸状态
    bool PotatoMineBoomed;//土豆雷已经爆炸
    QMediaPlayer *potatomine_ready_music;
    bool if_pb_music;

    bool if_plant_pause;
    vector <Zombie *> collideZombies; // 储存与植物碰撞的僵尸，暂未实现
    QMovie* GifPlant;

    virtual void timerEvent(QTimerEvent* event) override;

//    int slow;
//    int bomb;
//    int shootingTimeCounter;

public:
    enum{PotatoMine,CherryBomb,PeaShooter,SnowPea,SunFlower,Wallnut};

    Plant();
    Plant(int type,int line,int column);
    void advance(int phase) override;  //重载advance，在TheWar中将有计时器循环调用所有Item的advance，动画实现
    void updatePlant();//更新画面
    //设置参数
    void setPosition(int line,int column);
    void setType(int value);
    void setAttack(int value);
    void setAttackTimer(int value);
    void setAttackRate(int value);
    void setCost(int value);
    void setLife(double value);
    void setCD(int value);
    void setSun(int value);
    void setAlive(bool value);
    void setCollide(bool value);
    void setIsAttacking(bool value);
    void setPotatoMineBoomed(bool value);
    void setCherryBooming(bool value);
    //获得参数
    int getAttack() const;
    int getAttackTimer() const;
    int getAttackRate() const;
    int getType() const;
    int getCost() const;
    int getLine()const;
    int getColumn()const;
    int getSun() const;
    double getLife();
    double getCD() const;
    bool getAlive() const;
    bool getCollide() const;
    bool getPotatoMineReady() const;
    bool getIsAttacking()const;
    bool getCherryBooming() const;
    bool get_if_pb_music()const;
    void Changing_plant_state();//改变plant暂停继续状态
    //与碰撞僵尸有关，还未实现
    void setCollideZombies(const vector<Zombie *> &value);
    void addCollideZombie(Zombie * zombie);
    vector<Zombie *> getCollideZombies() const;

};

#endif // PLANT_H
