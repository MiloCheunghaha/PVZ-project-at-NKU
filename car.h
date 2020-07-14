#ifndef CAR_H
#define CAR_H
#include <QGraphicsPixmapItem>
#include <QMediaPlaylist>
#include <QMediaPlayer>
class Car: public QGraphicsPixmapItem
{
private:
    double speed;//行动速度
    bool ready;//准备状态
    int Line;//位置
    QMediaPlayer *car_music;//车子开动的音效
    bool if_car_music;
public:
    Car();
    Car(int line);
    void advance(int phase);//重载advance，在TheWar中将有计时器循环调用所有Item的advance，动画实现
    void setReady(bool value);//改变准备状态
    bool getReady() const;//获得准备状态
    int getLine() const;
};

#endif // CAR_H
