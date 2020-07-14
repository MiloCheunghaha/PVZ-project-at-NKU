#include "BackgroundLabel.h"
#include"TheWar.h"


BackgroundLabel::BackgroundLabel(QWidget* parent) :QLabel(parent)
{
    move_flag = false;
    creat_flag = false;
    this->setMouseTracking(true);
    R = L = 0;
}
BackgroundLabel::~BackgroundLabel()
{
}

void BackgroundLabel::mousePressEvent(QMouseEvent* e)
{
    x_in_Ui = geometry().x() + x_in_Label;
    y_in_Ui = geometry().y() + y_in_Label;
    if (e->button() == Qt::LeftButton)
    {
        TheWar* ptr = (TheWar*)parentWidget();
        connect(this, &BackgroundLabel::send_sg1, ptr, &TheWar::createSlot);
        judge_lawn(x_in_Ui, y_in_Ui);
        creat_flag = true;
        if (reset_button)
        {
            connect(this, &BackgroundLabel::send_sg2, ptr, &TheWar::reset_button);
            L = R = 0;
            emit send_sg2();
        }
        else
        {
            emit send_sg1();
            L = R = 0;
        }
    }
}

void BackgroundLabel::mouseMoveEvent(QMouseEvent* e)
{
    x_in_Label = e->x();
    y_in_Label = e->y();
    x_in_Ui = geometry().x() + x_in_Label;
    y_in_Ui = geometry().y() + y_in_Label;
    if (move_flag)
    {
        plant->setScaledContents(true);
        plant->setGeometry(x_in_Label - 37, y_in_Label -50, 74, 53);
    }
}

int BackgroundLabel::getpos_x()
{
    return x_in_Ui;
}

int BackgroundLabel::getpos_y()
{
    return y_in_Ui;
}

int BackgroundLabel::get_R()
{
    return R;
}

int BackgroundLabel::get_L()
{
    return L;
}

void BackgroundLabel::choose_move(BackgroundLabel* Q)
{
    move_flag = true;
    plant = Q;
}

void BackgroundLabel::judge_lawn(int x,int y)
{
    if (x > 100 && x < 1200 && y>125 && y < 890)
    {
        if (x > 100 && x < 220 && y> 125 && y < 245)
        {
            R = 1;
            L = 1;
        }
        if (x > 100 && x < 210 && y>270 && y < 390)
        {
            R = 1;
            L = 2;
        }
        if (x > 100 && x < 210 && y>410 && y < 550)
        {
            R = 1;
            L = 3;
        }
        if (x > 100 && x < 210 && y>565 && y < 690)
        {
            R = 1;
            L = 4;
        }
        if (x > 100 && x < 210 && y>710 && y < 855)
        {
            R = 1;
            L = 5;
        }

        if (x > 225 && x < 330 && y>125 && y < 245)
        {
            R = 2;
            L = 1;
        }
        if (x > 225 && x < 330 && y>270 && y < 390)
        {
            R = 2;
            L = 2;
        }
        if (x > 225 && x < 330 && y>410 && y < 550)
        {
            R = 2;
            L = 3;
        }
        if (x > 225 && x < 330 && y>565 && y < 690)
        {
            R = 2;
            L = 4;
        }
        if (x > 225 && x < 330 && y>710 && y < 855)
        {
            R = 2;
            L = 5;
        }

        if (x > 340 && x < 450 && y>125 && y < 245)
        {
            R = 3;
            L = 1;
        }
        if (x > 340 && x < 450 && y>270 && y < 390)
        {
            R = 3;
            L = 2;
        }
        if (x > 340 && x < 450 && y>410 && y < 550)
        {
            R = 3;
            L = 3;
        }
        if (x > 340 && x < 450 && y>565 && y < 690)
        {
            R = 3;
            L = 4;
        }
        if (x > 340 && x < 450 && y>710 && y < 855)
        {
            R = 3;
            L = 5;
        }

        if (x > 470 && x < 575 && y>125 && y < 245)
        {
            R = 4;
            L = 1;
        }
        if (x > 470 && x < 575 && y>270 && y < 390)
        {
            R = 4;
            L = 2;
        }
        if (x > 470 && x < 575 && y>410 && y < 550)
        {
            R = 4;
            L = 3;
        }
        if (x > 470 && x < 575 && y>565 && y < 690)
        {
            R = 4;
            L = 4;
        }
        if (x > 470 && x < 575 && y>710 && y < 855)
        {
            R = 4;
            L = 5;
        }

        if (x > 590 && x < 695 && y>125 && y < 245)
        {
            R = 5;
            L = 1;
        }
        if (x > 590 && x < 695 && y>270 && y < 390)
        {
            R = 5;
            L = 2;
        }
        if (x > 590 && x < 695 && y>410 && y < 550)
        {
            R = 5;
            L = 3;
        }
        if (x > 590 && x < 695 && y>565 && y < 690)
        {
            R = 5;
            L = 4;
        }
        if (x > 590 && x < 695 && y>710 && y < 855)
        {
            R = 5;
            L = 5;
        }

        if (x > 705 && x < 820 && y>125 && y < 245)
        {
            R = 6;
            L = 1;
        }
        if (x > 705 && x < 820 && y>270 && y < 390)
        {
            R = 6;
            L = 2;
        }
        if (x > 705 && x < 820 && y>410 && y < 550)
        {
            R = 6;
            L = 3;
        }
        if (x > 705 && x < 820 && y>565 && y < 690)
        {
            R = 6;
            L = 4;
        }
        if (x > 705 && x < 820 && y>710 && y < 855)
        {
            R = 6;
            L = 5;
        }

        if (x > 835 && x < 940 && y>125 && y < 245)
        {
            R = 7;
            L = 1;
        }
        if (x > 835 && x < 940 && y>270 && y < 390)
        {
            R = 7;
            L = 2;
        }
        if (x > 835 && x < 940 && y>410 && y < 550)
        {
            R = 7;
            L = 3;
        }
        if (x > 835 && x < 940 && y>565 && y < 690)
        {
            R = 7;
            L = 4;
        }
        if (x > 835 && x < 940 && y>710 && y < 855)
        {
            R = 7;
            L = 5;
        }

        if (x > 950 && x < 1065 && y>125 && y < 245)
        {
            R = 8;
            L = 1;
        }
        if (x > 950 && x < 1065 && y>270 && y < 390)
        {
            R = 8;
            L = 2;
        }
        if (x > 950 && x < 1065 && y>410 && y < 550)
        {
            R = 8;
            L = 3;
        }
        if (x > 950 && x < 1065 && y>565 && y < 690)
        {
            R = 8;
            L = 4;
        }
        if (x > 950 && x < 1065 && y>710 && y < 855)
        {
            R = 8;
            L = 5;
        }

        if (x > 1075 && x < 1200 && y>125 && y < 245)
        {
            R = 9;
            L = 1;
        }
        if (x > 1075 && x < 1200 && y>270 && y < 390)
        {
            R = 9;
            L = 2;
        }
        if (x > 1075 && x < 1200 && y>410 && y < 550)
        {
            R = 9;
            L = 3;
        }
        if (x > 1075 && x < 1200 && y>565 && y < 690)
        {
            R = 9;
            L = 4;
        }
        if (x > 1075 && x < 1200 && y>710 && y < 855)
        {
            R = 9;
            L = 5;
        }
        reset_button = false;
    }
    else
    {
        reset_button = true;
    }
    // qDebug() <<"L="<< L << " " <<"R="<< R;
}
