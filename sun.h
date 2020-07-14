#ifndef SUN_H
#define SUN_H

#include <QGraphicsPixmapItem>

class Sun : public QGraphicsPixmapItem
{
    bool CollectReady;//是否被收集
    bool deleteReady;//是否应该删除（动画结束）
    int existTime;//阳光存在时间
    QPointF initPos;//动画开始位置
    QPointF finalPos;//动画停止位置
    int duration;//动画持续时间为50ms*duration

public:
    Sun(int durationValue=200);
    void advance(int phase);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool getCollectReady() const;//获得收集状态
    bool getDeleteReady() const;//获得收集状态

    int getExistTime() const;//获得存在时间，超时消失
    void setExistTime(int value);//改变存在时间
    void setInitPos(QPointF pos);//初位置
    void setFinalPos(QPointF pos);//末位置 以实现动画效果
    QPointF getInitPos()const;
    QPointF getFinalPos()const;
};

#endif // SUN_H
