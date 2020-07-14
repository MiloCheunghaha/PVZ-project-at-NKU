#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <QGraphicsPixmapItem>
#include<QMovie>
#include<QDebug>
#include<QTimer>
#include <QMediaPlaylist>
#include <QMediaPlayer>

class Zombie :public QGraphicsPixmapItem
{
private:
    int type_Zombie;//僵尸种类
    int line;//位置
    int life;//生命值
    int attack;//攻击力
    int attackTimer;//碰撞状态持续时间，用于与攻击速度取余决定是否攻击
    double attackRate;//攻击速度
    double speed;//移动速度
    bool collide; //碰撞状态
    bool alive;//存活状态
    bool slowed;//减速状态
    bool boomed;//僵尸灰烬
    int CountForDead;//用于播放完僵尸死亡的动画
    QMovie* GifZombie;//加载僵尸的Gif图像
    bool if_zombie_pause;
    QMediaPlaylist *eat_list;
    QMediaPlayer *eat;//僵尸eat music
public:
    enum {Normal,Conehead,Buckethead,Flag};

    Zombie();
    Zombie(int type,int line);
    void advance(int phase) override; //重载advance，在TheWar中将有计时器循环调用所有Item的advance，动画实现
    void updateZombie();//更新画面
    void Changing_zombie_state();//改变僵尸暂停继续状态
    void Zombieboomed();//改变僵尸

    //设置参数
    void setType(int value);
    void setLine(int value);
    void setLife(int value);
    void setAttack(int value);
    void setAttackTimer(int value);
    void setAttackRate(int value);
    void setSpeed(double value);
    void setCollide(bool value);
    void setAlive(bool value);
    void setSlowed(bool value);
    //获得参数
    int getType() const;
    int getLine() const;
    int getLife() const;
    int getAttack() const;
    int getAttackTimer()const;
    int getAttackRate() const;
    double getSpeed() const;
    bool getCollide() const;
    bool getAlive() const;
    bool getSlowed() const;
};

#endif // ZOMBIE_H
