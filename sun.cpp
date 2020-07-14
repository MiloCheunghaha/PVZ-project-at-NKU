#include "sun.h"

Sun::Sun(int durationvalue):CollectReady(false),deleteReady(false),existTime(0)
{
    duration=durationvalue;//如果没有指定动画时间，默认为200*50ms
}

void Sun::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    CollectReady = true;
    setInitPos(pos());
    duration=25;//调整收集阳光动画速度
    setFinalPos(QPointF(90,90));
}

void Sun::advance(int phase)
{
    if(!phase) return;
    if(pos()!=finalPos)
    {
        setPos(pos().x()+(finalPos.x()-initPos.x())/duration,pos().y()+(finalPos.y()-initPos.y())/duration);
    }
    else if(CollectReady)
        deleteReady=true;//到达最终位置才清除
}


bool Sun::getCollectReady() const
{
    return CollectReady;
}
bool Sun::getDeleteReady() const
{
    return deleteReady;
}

int Sun::getExistTime() const
{
    return existTime;
}
void Sun::setExistTime(int value)
{
    existTime=value;
}
void Sun::setInitPos(QPointF pos)
{
    initPos=pos;
    setPos(initPos);
}
void Sun::setFinalPos(QPointF pos)
{
    finalPos=pos;
}
QPointF Sun::getInitPos()const
{
    return initPos;
}
QPointF Sun::getFinalPos()const
{
    return finalPos;
}
