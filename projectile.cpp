#include "projectile.h"

bool Projectile::getSlow() const
{
    return slow;
}

void Projectile::setSlow(bool value)
{
    slow = value;
}
int Projectile::getAttack() const
{
    return attack;
}
void Projectile::setAttack(int value)
{
    attack = value;
}

Projectile::Projectile() : speed(6), slow(false)
{
    if_projectile_pause=false;
}

void Projectile::advance(int phase)
{
    if(if_projectile_pause) return;
    if(!phase) return;
    this->setPos(this->pos().x() + speed, this->pos().y());
}

void Projectile::Changing_projectile_state()
{
    if_projectile_pause=!if_projectile_pause;
}
