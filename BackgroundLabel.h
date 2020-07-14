#ifndef BACKGROUNDLABEL_H
#define BACKGROUNDLABEL_H


#include<QLabel>
#include<QMouseEvent>

extern int x_in_Label, y_in_Label, x_in_Ui, y_in_Ui;

class BackgroundLabel :public QLabel
{
    Q_OBJECT
private:
    int x_in_Label;
    int y_in_Label;
    int x_in_Ui;
    int y_in_Ui;
    bool move_flag;
    BackgroundLabel* plant;
    int R;
    int L;
    int plant_judge;
    bool creat_flag;
    bool reset_button;

public:
    BackgroundLabel(QWidget* parent = 0);
    ~BackgroundLabel();
    void mousePressEvent(QMouseEvent* e);//鼠标按下事件
    void mouseMoveEvent(QMouseEvent* e);//鼠标移动事件,以实现选择植物图片跟随鼠标
    void judge_lawn(int,int);//判断点击处位置
    int getpos_x();
    int getpos_y();
    void choose_move(BackgroundLabel*);
    int get_R();
    int get_L();

signals:
    void send_sg1();
    void send_sg2();
};

#endif // BACKGROUNDLABEL_H
