#include "car.h"


Car::Car() : speed(10), ready(false)
{
}

Car::Car(int line) : speed(10), ready(false)
{
    Line=line;
    setPixmap(QPixmap(":/new/prefix1_car/image/car/car1.png"));
    setPos(22, 150 + 143 * Line);
    if_car_music=true;
    car_music=NULL;
}

void Car::advance(int phase)
{
    if(!phase) return;
    if(!ready) return;
    this->setPos(this->pos().x() + speed, this->pos().y());
}

bool Car::getReady() const
{
    return ready;
}

void Car::setReady(bool value)
{
    ready = value;
    //播放音效
    if(if_car_music==true)
    {
        delete car_music;
        car_music = new QMediaPlayer;
        car_music->setMedia(QUrl("qrc:/new/prefix1/music/car/lawnmower.mp3"));
        car_music->setVolume(50);
        car_music->play();
        if_car_music=false;
    }
}

int Car::getLine() const
{
    return Line;
}
