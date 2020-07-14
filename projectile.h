#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsPixmapItem>

class Projectile: public QGraphicsPixmapItem
{
private:
    int speed;
    int attack;//攻击力
    bool slow;//供寒冰射手，是否有减速效果
    bool if_projectile_pause;//以实现暂停功能
public:
    Projectile();
    void advance(int phase);//重载advance，在TheWar中将有计时器循环调用所有Item的advance，动画实现

    int getAttack() const;
    bool getSlow() const;
    void setSlow(bool value);
    void setAttack(int value);
    void setSpeed(int value);
    void Changing_projectile_state();
};

#endif // PROJECTILE_H
