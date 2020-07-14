#ifndef THEWAR_H
#define THEWAR_H

#include<QLabel>
#include <QWidget>
#include<QPushButton>
#include<QMovie>
#include<QGraphicsScene>
#include <QGraphicsView>
#include<QGraphicsPixmapItem>
#include<QTimer>
#include<QTime>
#include"zombie.h"
#include"plant.h"
#include"car.h"
#include"projectile.h"
#include"sun.h"
#include"BackgroundLabel.h"
#include"level.h"
#include <QMediaPlaylist>
#include <QMediaPlayer>
const int cd_wallnut = 4000;//ms
const int cd_sunflower = 3000;
const int cd_sonwpea = 3000;
const int cd_peashooter = 3000;
const int cd_cherrybomb = 3000;
const int cd_potatomine = 3000;


class TheWar : public QWidget
{
    Q_OBJECT

private:
      QGraphicsScene* scene;//注意，这里主背景不再是QLabel，而是QGraphicsScene，它储存QGraphicsPixmapItem对象，通过QGraphicsView显示
      BackgroundLabel* pBackGround;//背景指针
      QLabel chooseBackground;//植物选择框框
      QLabel sunBackground;//显示阳光的背景
      QLabel sunShow;//显示阳光数量
      QLabel ending_interface;//游戏结束界面
      BackgroundLabel ch_action[8];//选择动画
      QPushButton ch_bar[8];//选择植物
      level *thisGame;//本游戏level

      QMediaPlayer*b_g_music;
      QMediaPlaylist*b_g_music_list;
      QMediaPlayer *button_music;
      QMediaPlayer*planting_plant_music;//种植植物音效
      QMediaPlayer*peashooter_music;//发射豌豆音效
      QMediaPlayer*snowpea_music;//冰豌豆音效
      QMediaPlayer *potatomine_bomb_music;
      QMediaPlayer *cherrybomb_music;
      QMediaPlayer *pea;//吐 豌豆
      QMediaPlayer *snowpea;//吐 豌豆
      QMediaPlayer *pea_z;//子弹音效
      QMediaPlayer *snowpea_z;//子弹音效
      QMediaPlayer *win;//胜利音效
      bool if_play_win;
      QMediaPlayer *lose;//lose音效
      bool if_play_lose;
      QMediaPlayer *sun_get_music;

      int m_x;//position_x
      int m_y;//position_y
      int L;//line
      int R;//row
      int plant_kind=-1;//种什么植物
      int totalSun;//阳光总数


      int updateSunTimer;//更新阳光数量显示的计时
      int dropSunsTimer;//自然阳光的记时
      int dropSunsDuration;//每次生成新阳光的时间在一个区间
      int wllnut_cd_timer;
      int sunflower_cd_timer;
      int snowpea_cd_timer;
      int peashooter_cd_timer;
      int cherrybomb_cd_timer;
      int potatomine_cd_timer;
      QPushButton* b3 ;//暂停继续按钮
      QPushButton* b2;//返回按钮
      bool pausing;
      QTime plants_generate;//记录最近一次产生植物产生的时间
      QTime pause_time;//记录按下暂停的时间
      int gap;//记录上两个变量的差值
      QTime last_wn_cd;//最近一次wallnut开始cd的时间
      QTime last_sf_cd;//sunflower
      QTime last_sp_cd;//snowpea
      QTime last_pea_cd;//pea
      QTime last_pm_cd;//potatomine
      QTime last_cb_cd;//cherrybomb
      int wn_gap;//记录暂停时cd的timer走过的时间
      int sf_gap;
      int sp_gap;
      int pea_gap;
      int pm_gap;
      int cb_gap;
      bool if_wn_cd;//用于判断植物是否在冷却
      bool if_sf_cd;
      bool if_sp_cd;
      bool if_pea_cd;
      bool if_pm_cd;
      bool if_cb_cd;

      bool after_pause;//用于判断是否暂停后第一次执行植物产物产生
      int zombieSpeed;
      bool ifZombieEnd;//僵尸是否全部生成完毕

      QTimer * sceneTimer;//更新画面计时器
      QTimer * checkerTimer;//检查逻辑计时器
      QTimer * plantsGenerateTimer;//植物产生物的计时器

      int if_lawn_plant[5][9];//记录草坪上是否有植物;1有0无



public:
    explicit TheWar(QWidget* parent = nullptr);
    void initGame();
    void startGame();
    void sendSlot();//发送信号给第一窗口（欢迎界面）
    void initialChooseBar();
    void timerEvent(QTimerEvent* event);
    void GameOver(bool Win);//游戏结束（赢或输）



    vector<Plant*> existingPlants;//储存存在的植物
    vector<Zombie*> existingZombies;//储存存在的僵尸
    vector<Car*> existingCars;//储存存在的小车
    vector<Projectile*> existingProjectiles;//储存存在的发射物
    vector<Sun*> existingPlantSuns;//储存植物产生的太阳
    vector<Sun*> existingDropSuns;//储存落下来的太阳

    void creat_plant_label(int n);//选择植物动画
    void createPlant(int n);//创造植物
    void cd();
    void cherrybooming(int line, int column);//樱桃炸弹爆炸
    void potatominebooming(int line, int column);//土豆雷炸弹爆炸
    void adjust(int ZombieSpeed);//游戏开始前调整参数


signals:
    void SignalReturnMenu();//返回菜单信号
public slots:

    void if_start();
    void PlantsChecker();//与植物直接相关的检查器，如植物与僵尸的碰撞、植物生成的阳光
    void PlantsGenerate();//植物产生物，包括发射物及阳光等
    void DropSunsChecker();//自然阳光的生成
    void ProjectileChecker();//与发射物直接相关的检查器，如发射物与僵尸的碰撞、发射物超出边界，还未实现
    void GameOverChecker();//检查游戏是否结束，包含小车的实现

    void plant_slot1();//选择框按钮信号对应的槽
    void plant_slot2();//选择框按钮信号对应的槽
    void plant_slot3();//选择框按钮信号对应的槽
    void plant_slot4();//选择框按钮信号对应的槽
    void plant_slot5();//选择框按钮信号对应的槽
    void plant_slot6();//选择框按钮信号对应的槽
    void createSlot();//种植槽
    void reset_button();//设置按钮样式
    void generateZombie();//产生僵尸
    void pause();//游戏暂停
    void go_on();//游戏继续
    void checkZombie();//检查是否僵尸全部死亡
};

#endif // THEWAR_H
