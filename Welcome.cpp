#include "Welcome.h"
#include "ui_Welcome.h"
#include<QPushButton>
#include<QPaintEvent>
#include<QPainter>
#include <QMediaPlaylist>
#include <QMediaPlayer>

Welcome::Welcome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Welcome)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("植物大战僵尸"));
    resize(900, 600);

    welcome_music_list = new QMediaPlaylist;
    welcome_music_list->addMedia(QUrl("qrc:/new/prefix1/music/b_g/welcome.mp3"));
    welcome_music_list->setCurrentIndex(1);
    welcome_music_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    welcome_music = new QMediaPlayer;
    welcome_music->setPlaylist(welcome_music_list);
    welcome_music->setVolume(50);
    welcome_music->play();

    war_conts=0;
    buttonStartGame();
    //处理子窗口的信号
    connect(&thewar[war_conts], &TheWar::SignalReturnMenu, this, &Welcome::returnMenu);
    connect(this, &Welcome::SignalStartGame, &thewar[war_conts], &TheWar::if_start);
    adjustment();
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/new/prefix1_background/image/b_g/MainMenu.png"), QRect());
}

void Welcome::buttonStartGame()
{
    QPushButton* button = new QPushButton(this);
    button->setParent(this);
    button->move(520, 170);
    button->resize(200, 100);
    button->setStyleSheet("QPushButton{border-image:url(:/new/prefix1_buttons/image/buttons/Adventure_0.png)}");
    connect(button, &QPushButton::clicked, this, &Welcome::startGame);
}

void Welcome::startGame()//开始游戏
{
    button_music = new QMediaPlayer;
    button_music->setMedia(QUrl("qrc:/new/prefix1/music/button/buttonclick.mp3"));
    button_music->setVolume(50);
    button_music->play();
    welcome_music->stop();
    delete welcome_music;

    emit SignalStartGame();
    thewar[war_conts].adjust(pSlider->value());
    hide();
    thewar[war_conts].show();
}

void Welcome::returnMenu()//返回到菜单
{
    thewar[war_conts].close();

    welcome_music = new QMediaPlayer;
    welcome_music->setPlaylist(welcome_music_list);
    welcome_music->setVolume(50);
    welcome_music->play();

    show();
    war_conts++;
    connect(&thewar[war_conts], &TheWar::SignalReturnMenu, this, &Welcome::returnMenu);
    connect(this, &Welcome::SignalStartGame, &thewar[war_conts], &TheWar::if_start);
    disconnect(&thewar[war_conts-1], &TheWar::SignalReturnMenu, this, &Welcome::returnMenu);
    disconnect(this, &Welcome::SignalStartGame, &thewar[war_conts-1], &TheWar::if_start);
}
void Welcome::adjustment()
{

    int nMin = 1;
    int nMax = 5;
    int nSingleStep = 1;

    // 字体
    QLabel *text=new QLabel(this);
    text->setText("僵尸移动速度");
    text->resize(200,100);
    QFont ft;
    ft.setPointSize(15);
    text->setFont(ft);
    // 微调框
    QSpinBox *pSpinBox = new QSpinBox(this);
    pSpinBox->setMinimum(nMin);  // 最小值
    pSpinBox->setMaximum(nMax);  // 最大值
    pSpinBox->setSingleStep(nSingleStep);  // 步长
    pSpinBox->move(170,40);

    //滑条框
    pSlider = new QSlider(this);
    pSlider->setOrientation(Qt::Horizontal);  // 水平方向
    pSlider->setMinimum(nMin);  // 最小值
    pSlider->setMaximum(nMax);  // 最大值
    pSlider->setSingleStep(nSingleStep);  // 步长
    pSlider->move(210,40);
    connect(pSpinBox, SIGNAL(valueChanged(int)), pSlider, SLOT(setValue(int)));
    connect(pSlider, SIGNAL(valueChanged(int)), pSpinBox, SLOT(setValue(int)));
    pSlider->setValue(3);


}

