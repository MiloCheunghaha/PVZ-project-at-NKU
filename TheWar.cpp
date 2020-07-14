#include<cstdlib>
#include<ctime>
#include<QTime>
#include "TheWar.h"
#include <QMediaPlaylist>
#include <QMediaPlayer>
TheWar::TheWar(QWidget* parent) : QWidget(parent)
{
    setWindowTitle(QStringLiteral("植物大战僵尸"));
    resize(1290, 896);
    this->setMouseTracking(true);
}

void TheWar::if_start()
{
    initGame();
}

void TheWar::initGame()
{
    //初始化地图数组
    for(int i=0;i<5;i++)
        for(int j=0;j<9;j++)
            if_lawn_plant[i][j]=0;
    //初始化背景
    scene = new QGraphicsScene(this);
    QGraphicsView* view = new QGraphicsView(scene, this);
    QGraphicsPixmapItem* map1 = new QGraphicsPixmapItem(QPixmap(":/new/prefix1_background/image/b_g/map1.jpg"));
    scene->setSceneRect(0,0,1288,894); //注意这里必须宽度与高度-2，不然会有边界（硬解码问题）
    pBackGround=new BackgroundLabel;
    pBackGround->setParent(this);
    pBackGround->resize(1290,896);
    this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    scene->addItem(map1);//scene通过addItem将QGraphicsPixmapItem对象收入
    view->resize(1290,896);
    view->show();

    totalSun=10000;//初始化阳光
    dropSunsTimer=0;
    dropSunsDuration=100;//首次生成自然阳光时间100*50ms
    //初始化小车
    for(int i=0;i<5;i++)
    {
        Car* aCar=new Car(i);
        scene->addItem(aCar);
        existingCars.push_back(aCar);
    }
    initialChooseBar();//初始化选择菜单
    //设置返回按钮
    QPushButton* b2 = new QPushButton(this);
    b2->setParent(this);
    b2->move(1148, 0);
    b2->resize(142, 53);
    b2->setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_buttons/image/buttons/menu.png);"
                "border-radius:10px;"
                "}"
                );
    connect(b2, &QPushButton::pressed, this, &TheWar::sendSlot);
    //设置暂停按钮
    b3= new QPushButton(this);
    b3->setParent(this);
    b3->move(1006, 2);
    b3->resize(142, 51);
    b3->setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_buttons/image/buttons/pause.png);"
                "border-radius:10px;"
                "}"
                );
    connect(b3,&QPushButton::pressed, this, &TheWar::pause);

    pausing=false;
    ifZombieEnd=false;
    updateSunTimer=startTimer(200);//阳光数量更新的timer

    if_play_win=true;
    if_play_lose=true;

    //初始化音乐音效
    b_g_music_list = new QMediaPlaylist;
    b_g_music_list->addMedia(QUrl("qrc:/new/prefix1/music/war/Watery Graves.mp3"));
    b_g_music_list->setCurrentIndex(1);
    b_g_music_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    b_g_music = new QMediaPlayer;
    b_g_music->setPlaylist(b_g_music_list);
    b_g_music->setVolume(40);
    b_g_music->play();

    pea = NULL;
    snowpea = NULL;
    planting_plant_music = NULL;
    pea_z = NULL;
    snowpea_z=NULL;
    win = NULL;
    lose = NULL;
    cherrybomb_music = NULL;
    potatomine_bomb_music = NULL;
    sun_get_music=NULL;
    //开始游戏
    startGame();
}

void TheWar::startGame()
{
    srand(time(0));//随机数种子
    thisGame=new level;
    thisGame->setParent(this);
    thisGame->startLevel();//开始出现僵尸
    //下面根据计时器，更新画面，检查逻辑（如碰撞）
    sceneTimer = new QTimer(this);
    connect(sceneTimer, SIGNAL(timeout()), scene, SLOT(advance()));
    sceneTimer->start(10);
    checkerTimer=new QTimer(this);
    connect(checkerTimer, SIGNAL(timeout()), this, SLOT(PlantsChecker()));
    connect(checkerTimer, SIGNAL(timeout()), this, SLOT(ProjectileChecker()));
    connect(checkerTimer, SIGNAL(timeout()), this, SLOT(GameOverChecker()));
    connect(checkerTimer, SIGNAL(timeout()), this, SLOT(DropSunsChecker()));
    checkerTimer->start(50);
    after_pause=true;
    //下面根据计时器，生成植物产生物
    plantsGenerateTimer=new QTimer(this);
    connect(plantsGenerateTimer, SIGNAL(timeout()), this, SLOT(PlantsGenerate()));
    plantsGenerateTimer->start(50);
    gap=50;
}

void TheWar::PlantsChecker()
{
    if(!pausing)
    {
        //僵尸与植物的碰撞
        for(int i = 0; i < int(existingZombies.size()); i++)
        {
            if(existingZombies.at(i)->getAlive())
            {
                for(int j = 0;j < int(existingPlants.size()); j++)
                {
                    if(existingPlants.at(j)->getType()!=Plant::CherryBomb&&existingPlants.at(j)->getAlive())
                    {
                        if(existingZombies.at(i)->getLine()==existingPlants.at(j)->getLine()
                                &&existingZombies.at(i)->collidesWithItem(existingPlants.at(j)))
                        {
                            if(!existingZombies.at(i)->getCollide())
                            {
                                //                            qDebug()<<"existingZombies:"<<i;
                                if(!existingPlants.at(j)->getCollide())
                                    existingPlants.at(j)->setCollide(true);
                                existingPlants.at(j)->addCollideZombie(existingZombies.at(i));
                                //                            qDebug()<<"existingPlants add CollideZombie!";
                                existingZombies.at(i)->setCollide(true);
                            }
                            //!使碰撞中僵尸的计时器增加
                            existingZombies.at(i)->setAttackTimer(existingZombies.at(i)->getAttackTimer()+50);
                        }
                    }
                }
            }
        }
        //僵尸对植物的攻击
        for(int i=0;i < int(existingPlants.size()); i++)
        {
            if(existingPlants.at(i)->getAlive()&&existingPlants.at(i)->getCollide())
            {
                if(existingPlants.at(i)->getType() == Plant::PotatoMine && existingPlants.at(i)->getPotatoMineReady())//土豆雷与僵尸碰撞并已经准别就绪
                {
                    if(existingPlants.at(i)->get_if_pb_music())
                    {
                        delete potatomine_bomb_music;
                        potatomine_bomb_music = new QMediaPlayer;
                        potatomine_bomb_music->setMedia(QUrl("qrc:/new/prefix1/music/plant/explosion.mp3"));
                        potatomine_bomb_music->setVolume(50);
                        potatomine_bomb_music->play();
                    }
                    potatominebooming(existingPlants.at(i)->getLine(),existingPlants.at(i)->getColumn());

                    existingPlants.at(i)->setPotatoMineBoomed(true);
                    //死亡后设置与它碰撞僵尸的状态
                    if (existingPlants.at(i)->getCollide())
                    {
                        vector <Zombie *> itsCollidingZombies = existingPlants.at(i)->getCollideZombies();
                        for(int j=0; j <int(itsCollidingZombies.size()); j++)
                        {
                            if(itsCollidingZombies.at(j)->getAlive())
                                itsCollidingZombies.at(j)->setCollide(false);
                        }
                    }
                    if_lawn_plant[existingPlants.at(i)->getLine()-1][existingPlants.at(i)->getColumn()-1]=0;
                }
                else
                {
                    vector<Zombie*> itsCollidingZombies=existingPlants.at(i)->getCollideZombies();
                    for(int j=0;j<int(itsCollidingZombies.size());j++)
                    {
                        if(itsCollidingZombies.at(j)->getAlive()&&
                                itsCollidingZombies.at(j)->getAttackTimer()%itsCollidingZombies.at(j)->getAttackRate()==0&&
                                itsCollidingZombies.at(j)->getAttackTimer()!=0)
                        {
                            existingPlants.at(i)->setLife( existingPlants.at(i)->getLife()-itsCollidingZombies.at(j)->getAttack());
                            // qDebug()<<"Attacked";
                        }
                    }
                }
            }
            //植物死亡时
            if (existingPlants.at(i)->getAlive()&&existingPlants.at(i)->getLife()<=0)
            {
                existingPlants.at(i)->hide();
                existingPlants.at(i)->setAlive(false);
                if_lawn_plant[existingPlants.at(i)->getLine()-1][existingPlants.at(i)->getColumn()-1]=0;
                //死亡后设置与它碰撞僵尸的状态
                if (existingPlants.at(i)->getCollide())
                {
                    vector <Zombie *> itsCollidingZombies = existingPlants.at(i)->getCollideZombies();
                    for(int j=0; j <int(itsCollidingZombies.size()); j++)
                    {
                        if(itsCollidingZombies.at(j)->getAlive())
                            itsCollidingZombies.at(j)->setCollide(false);
                    }
                }
            }
        }
        //植物生成的阳光相关
        for(int i=0; i<int(existingPlantSuns.size()); i++)
        {
            //超时消失
            if(existingPlantSuns.at(i)->getExistTime() >= 7500)
            {
                delete existingPlantSuns.at(i);
                existingPlantSuns.erase(existingPlantSuns.begin()+i);
            }
            //如果点击，加阳光，消失
            else if(existingPlantSuns.at(i)->getDeleteReady())
            {
                totalSun += 25;
                delete existingPlantSuns.at(i);
                existingPlantSuns.erase(existingPlantSuns.begin()+i);
                delete sun_get_music;
                sun_get_music = new QMediaPlayer;
                sun_get_music->setMedia(QUrl("qrc:/new/prefix1/music/plant/chime.mp3"));
                sun_get_music->setVolume(30);
                sun_get_music->play();
            }
            //如果没点，每次调用此函数加存在时间，相当于计时器
            else
                existingPlantSuns.at(i)->setExistTime(existingPlantSuns.at(i)->getExistTime()+50);
        }
    }
}

void TheWar::ProjectileChecker()
{

    //检查子弹对僵尸的伤害
    for (int i=0; i<int(existingZombies.size()); i++)
    {
        if(existingZombies.at(i)->getAlive())
        {
            for(int j=0; j<int(existingProjectiles.size()); j++)
            {
                if(existingZombies.at(i)->collidesWithItem(existingProjectiles.at(j)))
                {
                    existingZombies.at(i)->setLife(existingZombies.at(i)->getLife() - 25);//因为现有子弹攻击力均为25，以后可更改
                    if (existingZombies.at(i)->getLife() <= 0)
                    {
                        existingZombies.at(i)->hide();
                        existingZombies.at(i)->setAlive(false);
                    }
                    else
                    {
                        if(!existingZombies.at(i)->getSlowed()&&existingProjectiles.at(j)->getSlow())
                        {
                            existingZombies.at(i)->setSlowed(true);
                            existingZombies.at(i)->setSpeed(0.5*existingZombies.at(i)->getSpeed());

                            delete snowpea_z;
                            snowpea_z = new QMediaPlayer;
                            snowpea_z->setMedia(QUrl("qrc:/new/prefix1/music/war/snow_pea_sparkles.mp3"));
                            snowpea_z->setVolume(100);
                            snowpea_z->play();
                        }
                    }
                    delete existingProjectiles.at(j);
                    existingProjectiles.erase(existingProjectiles.begin()+j);
                }
            }
        }
    }
    //检查子弹超过边界
    for(int i=0;i<int(existingProjectiles.size());i++)
    {
        if(existingProjectiles.at(i)->pos().x()>=1290)
        {
            delete existingProjectiles.at(i);
            existingProjectiles.erase(existingProjectiles.begin()+i);
            //            qDebug()<<"Projectile Reach The Range";
        }
    }
}

void TheWar::GameOverChecker()
{
    //检查小车是否应该启动
    for(int i=0;i<int(existingZombies.size());i++)
    {
        if(existingZombies.at(i)->getAlive())
        {
            if(existingZombies.at(i)->collidesWithItem(existingCars.at(existingZombies.at(i)->getLine()-1)))
                existingCars.at(existingZombies.at(i)->getLine()-1)->setReady(true);
        }
    }
    //小车杀僵尸
    for(int i=0;i<int(existingCars.size());i++)
    {
        if(existingCars.at(i)->getReady())
        {
            if(existingCars.at(i)->pos().x()>=1290)
            {
                existingCars.at(i)->hide();
                existingCars.at(i)->setReady(false);
                //                qDebug()<<"Cars reach the range!";
                continue;
            }
            for(int j=0;j<int(existingZombies.size());j++)
            {
                if(existingZombies.at(j)->getAlive())
                {
                    if(existingCars.at(i)->collidesWithItem(existingZombies.at(j))
                            &&existingCars.at(i)->getLine()+1==existingZombies.at(j)->getLine())
                    {
                        existingZombies.at(j)->hide();
                        existingZombies.at(j)->setAlive(false);
                    }
                }
            }
        }
    }
    for(int i=0;i<int(existingZombies.size());i++)
    {
        if(existingZombies.at(i)->getAlive())
        {
            if(existingZombies.at(i)->pos().x()<-240)
            {
                GameOver(false);
                //                qDebug()<<existingZombies.at(i)->pos();
            }
        }
    }
}


void TheWar::PlantsGenerate()
{
    if(!pausing)//如果正在暂停则不执行以下行为
    {
        for(int i=0;i<int(existingPlants.size());i++)
        {
            if(existingPlants.at(i)->getAlive())
            {
                switch (existingPlants.at(i)->getType())
                {
                case Plant::PeaShooter:
                {
                    bool shoot = false;
                    for(int j=0; j<int(existingZombies.size()); j++)//判断是否射击
                    {
                        if(existingPlants.at(i)->getLine() == existingZombies.at(j)->getLine() && existingZombies.at(j)->getAlive())//植物和僵尸在同一行
                        {
                            shoot = true;
                            break;
                        }
                    }
                    if(existingPlants.at(i)->getIsAttacking()!=shoot)
                        existingPlants.at(i)->setIsAttacking(shoot);
                    if(existingPlants.at(i)->getIsAttacking())
                    {
                        if(existingPlants.at(i)->getAttackTimer()%existingPlants.at(i)->getAttackRate()==0)
                        {
                            Projectile* aProjectile=new Projectile;
                            aProjectile->setPixmap(QPixmap(":/new/prefix1_plants/image/plant/Peashooter/PB001.png").scaled(60,60));
                            aProjectile->setPos(existingPlants.at(i)->pos().x()+70,existingPlants.at(i)->pos().y());
                            aProjectile->setAttack(existingPlants.at(i)->getAttack());
                            scene->addItem(aProjectile);
                            existingProjectiles.push_back(aProjectile);
                            delete pea;
                            pea = new QMediaPlayer;
                            pea->setMedia(QUrl("qrc:/new/prefix1/music/plant/throw.mp3"));
                            pea->setVolume(150);
                            pea->play();
                        }
                        existingPlants.at(i)->setAttackTimer(existingPlants.at(i)->getAttackTimer()+50);
                    }
                    break;
                }
                case Plant::SnowPea:
                {
                    bool shoot = false;
                    for(int j=0; j<int(existingZombies.size()); j++)//判断是否射击
                    {
                        if(existingPlants.at(i)->getLine() == existingZombies.at(j)->getLine() && existingZombies.at(j)->getAlive())//植物和僵尸在同一行
                        {
                            shoot = true;
                            break;
                        }
                    }
                    if(existingPlants.at(i)->getIsAttacking()!=shoot)
                        existingPlants.at(i)->setIsAttacking(shoot);
                    if(existingPlants.at(i)->getIsAttacking())
                    {
                        if(existingPlants.at(i)->getAttackTimer()%existingPlants.at(i)->getAttackRate()==0)
                        {
                            Projectile* aProjectile=new Projectile;
                            aProjectile->setPixmap(QPixmap(":/new/prefix1_plants/image/plant/SnowPea/PB-10.png").scaled(60,60));
                            aProjectile->setPos(existingPlants.at(i)->pos().x()+70,existingPlants.at(i)->pos().y());
                            aProjectile->setAttack(existingPlants.at(i)->getAttack());
                            aProjectile->setSlow(true);
                            scene->addItem(aProjectile);
                            existingProjectiles.push_back(aProjectile);
                            delete snowpea;
                            snowpea = new QMediaPlayer;
                            snowpea->setMedia(QUrl("qrc:/new/prefix1/music/plant/throw.mp3"));
                            snowpea->setVolume(150);
                            snowpea->play();
                        }
                        existingPlants.at(i)->setAttackTimer(existingPlants.at(i)->getAttackTimer()+50);
                    }
                    break;
                }
                case Plant::SunFlower:
                {
                    if(existingPlants.at(i)->getAttackTimer()%existingPlants.at(i)->getAttackRate()==0&&
                            existingPlants.at(i)->getAttackTimer()!=0)
                    {
                        Sun* aSun=new Sun(20);
                        aSun->setPixmap(QPixmap(":/new/prefix1_plants/image/plant/SunFlower/Sun.gif"));
                        aSun->setInitPos(QPointF(existingPlants.at(i)->pos().x()+20,existingPlants.at(i)->pos().y()+30));
                        aSun->setFinalPos(QPointF(aSun->pos().x()+(rand()%40-20),aSun->pos().y()-(rand()%30+40)));
                        scene->addItem(aSun);
                        existingPlantSuns.push_back(aSun);
                        existingPlants.at(i)->setAttackTimer(0);
                    }
                    existingPlants.at(i)->setAttackTimer(existingPlants.at(i)->getAttackTimer()+50);
                    break;
                }
                case Plant::CherryBomb:
                {
                    if(existingPlants.at(i)->getCherryBooming())
                    {
                        if_lawn_plant[existingPlants.at(i)->getLine()-1][existingPlants.at(i)->getColumn()-1]=0;
                        cherrybooming(existingPlants.at(i)->getLine(),existingPlants.at(i)->getColumn());//时间到，樱桃炸弹爆炸
                    }
                }
                default:
                    break;
                }
            }
        }
        plants_generate=QTime::currentTime();
        if(after_pause)
        {
            plantsGenerateTimer->stop();
            plantsGenerateTimer->start(50);
            after_pause=false;
        }
    }
}

void TheWar::DropSunsChecker()
{
    if(!pausing)
    {
        if(dropSunsTimer%(dropSunsDuration*50)==0&&dropSunsTimer!=0)
        {
            Sun* aSun=new Sun;
            aSun->setPixmap(QPixmap(":/new/prefix1_plants/image/plant/SunFlower/Sun.gif"));
            aSun->setFinalPos(QPointF(rand()%900+100,rand()%400+125));
            aSun->setInitPos(QPointF(aSun->getFinalPos().x(),-20));
            scene->addItem(aSun);
            existingDropSuns.push_back(aSun);
            dropSunsTimer=0;
            dropSunsDuration=rand()%41+80;//4秒-6秒，可调整
        }
        else
            dropSunsTimer+=50;

        for(int i=0;i<int(existingDropSuns.size());i++)
        {
            //超时消失
            if(existingDropSuns.at(i)->getExistTime() >= 7500)
            {
                delete existingDropSuns.at(i);
                existingDropSuns.erase(existingDropSuns.begin()+i);
            }
            //如果点击，加阳光，消失
            else if(existingDropSuns.at(i)->getDeleteReady())
            {
                totalSun += 25;
                delete sun_get_music;
                sun_get_music = new QMediaPlayer;
                sun_get_music->setMedia(QUrl("qrc:/new/prefix1/music/plant/chime.mp3"));
                sun_get_music->setVolume(30);
                sun_get_music->play();
                delete existingDropSuns.at(i);
                existingDropSuns.erase(existingDropSuns.begin()+i);
            }
            //如果没点，每次调用此函数加存在时间，相当于计时器
            else
                existingDropSuns.at(i)->setExistTime(existingDropSuns.at(i)->getExistTime()+50);
        }
    }
}


void TheWar::GameOver(bool Win)
{

    //将两按钮信号屏蔽
    disconnect(b3,&QPushButton::pressed, this, &TheWar::go_on);

    //结束界面
    ending_interface.setParent(this);
    ending_interface.move(400,230);
    ending_interface.resize(500,300);
    if(Win)
    {
        for(int i = 0; i < int(existingZombies.size()); i++)
        {
            existingZombies.at(i)->setSpeed(0);
        }
        ending_interface.setPixmap(QPixmap(":/new/prefix1_buttons/image/end/GameVictory.png"));
        if(if_play_win)
        {
            b_g_music->stop();
            delete win;
            win = new QMediaPlayer;
            win->setMedia(QUrl("qrc:/new/prefix1/music/war/winmusic.mp3"));
            win->setVolume(50);
            win->play();
            if_play_win=false;
        }
    }
    else
    {
        for(int i = 0; i < int(existingZombies.size()); i++)
        {
            existingZombies.at(i)->setSpeed(0);
        }

        ending_interface.setPixmap(QPixmap(":/new/prefix1_buttons/image/end/GameLose.png"));
        if(if_play_lose)
        {
            b_g_music->stop();
            delete lose;
            lose = new QMediaPlayer;
            lose->setMedia(QUrl("qrc:/new/prefix1/music/war/losemusic.mp3"));
            lose->setVolume(50);
            lose->play();
            if_play_lose=false;
        }
    }
    ending_interface.setScaledContents(true);
    ending_interface.show();
    //开始新游戏按钮
    QPushButton *b=new QPushButton;
    b->setParent(&ending_interface);
    b->setGeometry(180, 200, 154, 37);
    b->setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_buttons/image/end/StartButton.png);"
                "border-radius:10px;"
                "}"
                );
    connect(b,&QPushButton::pressed, this, &TheWar::sendSlot);
    b->show();
}

void TheWar::initialChooseBar()
{
    chooseBackground.setMouseTracking(true);
    chooseBackground.setParent(this);
    chooseBackground.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/ChooserBackground1.png"));
    chooseBackground.setGeometry(0, 0, 647, 107);
    sunBackground.setMouseTracking(true);
    sunBackground.setParent(this);
    sunBackground.setPixmap(QPixmap(":/new/prefix1_background/image/b_g/Sunbar.png"));
    sunBackground.setGeometry(3, 48, 95,80 );
    sunShow.setMouseTracking(true);
    sunShow.setParent(this);;
    sunShow.setGeometry(3, 75, 95, 106);
    sunShow.setStyleSheet("background:transparent");
    QFont ft;
    ft.setPointSize(15);
    sunShow.setFont(ft);
    sunShow.setStyleSheet("color:black");
    sunShow.setAlignment(Qt::AlignHCenter);
    sunShow.setText(QString::number(totalSun));

    ch_bar[0].setParent(this);
    ch_bar[0].setGeometry(95, 8, 64, 89);
    ch_bar[0].setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_plant_cards/image/plant_card/card_potatomine/card_potatomine.png);"
                "border-radius:10px;"
                "}"
                );
    connect(&ch_bar[0], &QPushButton::clicked, this, &TheWar::plant_slot1);
    ch_bar[1].setParent(this);
    ch_bar[1].setGeometry(95+68, 8, 64, 89);
    ch_bar[1].setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_plant_cards/image/plant_card/card_cherrybomb/card_cherrybomb.png);"
                "border-radius:10px;"
                "}"
                );
    connect(&ch_bar[1], &QPushButton::clicked, this, &TheWar::plant_slot2);
    ch_bar[2].setParent(this);
    ch_bar[2].setGeometry(95+ 68*2, 8, 64, 89);
    ch_bar[2].setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_plant_cards/image/plant_card/card_peashooter/card_peashooter.png);"
                "border-radius:10px;"
                "}"
                );
    connect(&ch_bar[2], &QPushButton::clicked, this, &TheWar::plant_slot3);
    ch_bar[3].setParent(this);
    ch_bar[3].setGeometry(95+ 68*3, 8, 64, 89);
    ch_bar[3].setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_plant_cards/image/plant_card/card_snowpea/card_snowpea.png);"
                "border-radius:10px;"
                "}"
                );
    connect(&ch_bar[3], &QPushButton::clicked, this, &TheWar::plant_slot4);
    ch_bar[4].setParent(this);
    ch_bar[4].setGeometry(95+ 68*4, 8, 64, 89);
    ch_bar[4].setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_plant_cards/image/plant_card/card_sunflower/card_sunflower.png);"
                "border-radius:10px;"
                "}"
                );
    connect(&ch_bar[4], &QPushButton::clicked, this, &TheWar::plant_slot5);
    ch_bar[5].setParent(this);
    ch_bar[5].setGeometry(95+ 68*5, 8, 64, 89);
    ch_bar[5].setStyleSheet(
                "QPushButton{"
                "background: url(:/new/prefix1_plant_cards/image/plant_card/card_wallnut/card_wallnut.png);"
                "border-radius:10px;"
                "}"
                );
    connect(&ch_bar[5], &QPushButton::clicked, this, &TheWar::plant_slot6);
}

void TheWar::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == updateSunTimer)
    {
        sunShow.setText(QString::number(totalSun));
    }
    if (event->timerId() == wllnut_cd_timer)
    {
        connect(&ch_bar[5], &QPushButton::clicked, this, &TheWar::plant_slot6);
        ch_bar[5].setStyleSheet(
                    "QPushButton{"
                    "background: url(:/new/prefix1_plant_cards/image/plant_card/card_wallnut/card_wallnut.png);"
                    "border-radius:10px;"
                    "}"
                    );
        killTimer(wllnut_cd_timer);
        if_wn_cd=false;
    }
    if (event->timerId() == sunflower_cd_timer)
    {
        connect(&ch_bar[4], &QPushButton::clicked, this, &TheWar::plant_slot5);
        ch_bar[4].setStyleSheet(
                    "QPushButton{"
                    "background: url(:/new/prefix1_plant_cards/image/plant_card/card_sunflower/card_sunflower.png);"
                    "border-radius:10px;"
                    "}"
                    );
        killTimer(sunflower_cd_timer);
    }
    if (event->timerId() == snowpea_cd_timer)
    {
        connect(&ch_bar[3], &QPushButton::clicked, this, &TheWar::plant_slot4);
        ch_bar[3].setStyleSheet(
                    "QPushButton{"
                    "background: url(:/new/prefix1_plant_cards/image/plant_card/card_snowpea/card_snowpea.png);"
                    "border-radius:10px;"
                    "}"
                    );
        killTimer(snowpea_cd_timer);
        if_sp_cd=false;
    }
    if (event->timerId() == peashooter_cd_timer)
    {
        connect(&ch_bar[2], &QPushButton::clicked, this, &TheWar::plant_slot3);
        ch_bar[2].setStyleSheet(
                    "QPushButton{"
                    "background: url(:/new/prefix1_plant_cards/image/plant_card/card_peashooter/card_peashooter.png);"
                    "border-radius:10px;"
                    "}"
                    );
        killTimer(peashooter_cd_timer);
        if_pea_cd=false;
    }
    if (event->timerId() == cherrybomb_cd_timer)
    {
        connect(&ch_bar[1], &QPushButton::clicked, this, &TheWar::plant_slot2);
        ch_bar[1].setStyleSheet(
                    "QPushButton{"
                    "background: url(:/new/prefix1_plant_cards/image/plant_card/card_cherrybomb/card_cherrybomb.png);"
                    "border-radius:10px;"
                    "}"
                    );
        killTimer(cherrybomb_cd_timer);
        if_cb_cd=false;
    }
    if (event->timerId() == potatomine_cd_timer)
    {
        connect(&ch_bar[0], &QPushButton::clicked, this, &TheWar::plant_slot1);
        ch_bar[0].setStyleSheet(
                    "QPushButton{"
                    "background: url(:/new/prefix1_plant_cards/image/plant_card/card_potatomine/card_potatomine.png);"
                    "border-radius:10px;"
                    "}"
                    );
        killTimer(potatomine_cd_timer);
        if_pm_cd=false;
    }
    if(ifZombieEnd)
    {
        int i=0;
        for(;i<int(existingZombies.size());i++)
        {
            if(existingZombies.at(i)->getAlive())
                break;
        }
        if(i==int(existingZombies.size()))
        {
            //qDebug()<<i;
            //qDebug()<<existingZombies.size();
            //qDebug()<<"win";
            GameOver(true);
        }
    }

}


void TheWar::creat_plant_label(int n)
{
    // qDebug()<<"creat_plant_label() called!"<<n;
    switch (n)
    {
    case Plant::PotatoMine:
        ch_action[0].setParent(this);//qDebug()<<1;
        m_x = pBackGround->getpos_x();//qDebug()<<2;
        m_y = pBackGround->getpos_y();//qDebug()<<3;
        ch_action[0].setGeometry(m_x - 37, m_y - 50, 74, 53);//qDebug()<<4;
        ch_action[0].setPixmap(QPixmap(":/new/prefix1_plant_cards/image/plant_card/card_potatomine/potatomine0.gif"));//qDebug()<<5;
        ch_action[0].raise();//qDebug()<<6;
        ch_action[0].show();//qDebug()<<7;
        ch_action[0].setMouseTracking(true);
        pBackGround->choose_move(&ch_action[0]);//qDebug()<<8;
        plant_kind = 0;//qDebug()<<9;
        break;
    case Plant::CherryBomb:
        ch_action[1].setParent(this);
        m_x = pBackGround->getpos_x();
        m_y = pBackGround->getpos_y();
        ch_action[1].setScaledContents(true);
        ch_action[1].setGeometry(m_x - 37, m_y - 50, 74, 53);
        ch_action[1].setPixmap(QPixmap(":/new/prefix1_plant_cards/image/plant_card/card_cherrybomb/cherrybomb0.gif"));
        ch_action[1].raise();
        ch_action[1].show();
        pBackGround->choose_move(&ch_action[1]);
        plant_kind = 1;
        break;
    case Plant::PeaShooter:
        ch_action[2].setParent(this);
        m_x = pBackGround->getpos_x();
        m_y = pBackGround->getpos_y();
        ch_action[2].setScaledContents(true);
        ch_action[2].setGeometry(m_x - 37, m_y - 50, 74, 53);
        ch_action[2].setPixmap(QPixmap(":/new/prefix1_plant_cards/image/plant_card/card_peashooter/peashoot0.gif"));
        ch_action[2].raise();
        ch_action[2].show();
        pBackGround->choose_move(&ch_action[2]);
        plant_kind = 2;
        break;
    case Plant::SnowPea:
        ch_action[3].setParent(this);
        m_x = pBackGround->getpos_x();
        m_y = pBackGround->getpos_y();
        ch_action[3].setScaledContents(true);
        ch_action[3].setGeometry(m_x - 37, m_y - 50, 74, 53);
        ch_action[3].setPixmap(QPixmap(":/new/prefix1_plant_cards/image/plant_card/card_snowpea/snowpea0.gif"));
        ch_action[3].raise();
        ch_action[3].show();
        pBackGround->choose_move(&ch_action[3]);
        plant_kind = 3;
        break;
    case Plant::SunFlower:
        ch_action[4].setParent(this);
        m_x = pBackGround->getpos_x();
        m_y = pBackGround->getpos_y();
        ch_action[4].setScaledContents(true);
        ch_action[4].setGeometry(m_x - 37, m_y - 50, 74, 53);
        ch_action[4].setPixmap(QPixmap(":/new/prefix1_plant_cards/image/plant_card/card_sunflower/sunflower0.gif"));
        ch_action[4].raise();
        ch_action[4].show();
        pBackGround->choose_move(&ch_action[4]);
        plant_kind = 4;
        break;
    case Plant::Wallnut:
        ch_action[5].setParent(this);
        m_x = pBackGround->getpos_x();
        m_y = pBackGround->getpos_y();
        ch_action[5].setScaledContents(true);
        ch_action[5].setGeometry(m_x - 37, m_y - 50, 74, 53);
        ch_action[5].setPixmap(QPixmap(":/new/prefix1_plant_cards/image/plant_card/card_wallnut/wallnut0.gif"));
        ch_action[5].raise();
        ch_action[5].show();
        pBackGround->choose_move(&ch_action[5]);
        plant_kind = 5;
        break;
    default:
        break;
    }
}

void TheWar::createPlant(int n)
{
    L= ch_action[n].get_L();
    R=ch_action[n].get_R();
    //    qDebug()<<"n="<<n;

    if (L != 0 && R != 0)
    {
        if(if_lawn_plant[L-1][R-1]==0)
        {
            if_lawn_plant[L-1][R-1]=1;
            //        qDebug()<<"switch";
            delete planting_plant_music;
            planting_plant_music = new QMediaPlayer;
            planting_plant_music->setMedia(QUrl("qrc:/new/prefix1/music/plant/plant1.mp3"));
            planting_plant_music->setVolume(25);
            planting_plant_music->play();
            switch (n)
            {

            case Plant::PotatoMine:
            {
                Plant *nowPlant=new Plant(Plant::PotatoMine,L,R);
                existingPlants.push_back(nowPlant);
                scene->addItem(nowPlant);
                ch_action[0].hide();
                totalSun -= 25;
                cd();
                plant_kind = -1;
                break;
            }
            case Plant::CherryBomb:
            {
                Plant *nowPlant=new Plant(Plant::CherryBomb,L,R);
                existingPlants.push_back(nowPlant);
                scene->addItem(nowPlant);
                ch_action[1].hide();
                totalSun -= 150;
                cd();
                plant_kind = -1;
                break;
            }
            case Plant::PeaShooter:
            {
                Plant *nowPlant=new Plant(Plant::PeaShooter,L,R);
                existingPlants.push_back(nowPlant);
                scene->addItem(nowPlant);
                ch_action[2].hide();
                totalSun -= 100;
                cd();
                plant_kind = -1;
                break;
            }
            case Plant::SnowPea:
            {
                Plant *nowPlant=new Plant(Plant::SnowPea,L,R);
                existingPlants.push_back(nowPlant);
                scene->addItem(nowPlant);
                ch_action[3].hide();
                totalSun -= 175;
                cd();
                plant_kind = -1;
                break;
            }
            case Plant::SunFlower:
            {
                Plant *nowPlant=new Plant(Plant::SunFlower,L,R);
                existingPlants.push_back(nowPlant);
                scene->addItem(nowPlant);
                ch_action[4].hide();
                totalSun -= 50;
                cd();
                plant_kind = -1;
                break;
            }
            case Plant::Wallnut:
            {
                Plant *nowPlant=new Plant(Plant::Wallnut,L,R);
                existingPlants.push_back(nowPlant);
                scene->addItem(nowPlant);
                ch_action[5].hide();
                totalSun -= 50;
                cd();
                plant_kind = -1;
                break;
            }
            default:
                break;
            }
        }
        else
        {
            reset_button();
        }
    }

    else
    {
        //        qDebug()<<"reset";
        reset_button();
    }


}

void TheWar::cd()
{
    switch (plant_kind)
    {
    case Plant::PotatoMine:
        potatomine_cd_timer = startTimer(cd_potatomine);
        last_pm_cd=QTime::currentTime();
        disconnect(&ch_bar[0], &QPushButton::clicked, this, &TheWar::plant_slot1);
        if_pm_cd=true;
        break;
    case Plant::CherryBomb:
        cherrybomb_cd_timer = startTimer(cd_cherrybomb);
        last_cb_cd=QTime::currentTime();
        disconnect(&ch_bar[1], &QPushButton::clicked, this, &TheWar::plant_slot2);
        if_cb_cd=true;
        break;
    case Plant::PeaShooter:
        peashooter_cd_timer = startTimer(cd_peashooter);
        last_pea_cd=QTime::currentTime();
        disconnect(&ch_bar[2], &QPushButton::clicked, this, &TheWar::plant_slot3);
        if_pea_cd=true;
        break;
    case Plant::SnowPea:
        snowpea_cd_timer = startTimer(cd_sonwpea);
        last_sp_cd=QTime::currentTime();
        disconnect(&ch_bar[3], &QPushButton::clicked, this, &TheWar::plant_slot4);
        if_sp_cd=true;
        break;
    case Plant::SunFlower:
        sunflower_cd_timer = startTimer(cd_sunflower);
        last_sf_cd=QTime::currentTime();
        disconnect(&ch_bar[4], &QPushButton::clicked, this, &TheWar::plant_slot5);
        if_sf_cd=true;
        break;
    case Plant::Wallnut:
        wllnut_cd_timer = startTimer(cd_wallnut);
        last_wn_cd=QTime::currentTime();
        disconnect(&ch_bar[5], &QPushButton::clicked, this, &TheWar::plant_slot6);
        if_wn_cd=true;
        break;
    default:
        break;
    }

}

void TheWar::createSlot()
{
    if(!pausing)
    {
        switch (plant_kind)
        {
        case 0:
            ch_bar[0].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_potatomine/card_potatomine2.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
            break;
        case 1:
            ch_bar[1].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_cherrybomb/card_cherrybomb2.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
            break;
        case 2:
            ch_bar[2].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_peashooter/card_peashooter2.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
            break;
        case 3:
            ch_bar[3].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_snowpea/card_snowpea2.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
            break;
        case 4:
            ch_bar[4].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_sunflower/card_sunflower2.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
            break;
        case 5:
            ch_bar[5].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_wallnut/card_wallnut2.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
            break;
        default:
            break;
        }
        createPlant(plant_kind);
        this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }
}

void TheWar::reset_button()
{
    if(!pausing)
    {
        switch (plant_kind)
        {
        case 0:
            ch_action[0].hide();
            ch_bar[0].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_potatomine/card_potatomine.png);"
                        "border-radius:10px;"
                        "}"
                        );
            plant_kind = -1;
            break;
        case 1:
            ch_action[1].hide();
            ch_bar[1].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_cherrybomb/card_cherrybomb.png);"
                        "border-radius:10px;"
                        "}"
                        );
            plant_kind = -1;
            break;
        case 2:
            ch_action[2].hide();
            ch_bar[2].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_peashooter/card_peashooter.png);"
                        "border-radius:10px;"
                        "}"
                        );
            plant_kind = -1;;
            break;
        case 3:
            ch_action[3].hide();
            ch_bar[3].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_snowpea/card_snowpea.png);"
                        "border-radius:10px;"
                        "}"
                        );
            plant_kind = -1;
            break;
        case 4:
            ch_action[4].hide();
            ch_bar[4].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_sunflower/card_sunflower.png);"
                        "border-radius:10px;"
                        "}"
                        );
            plant_kind = -1;
            break;
        case 5:
            ch_action[5].hide();
            ch_bar[5].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_wallnut/card_wallnut.png);"
                        "border-radius:10px;"
                        "}"
                        );
            plant_kind = -1;
            break;
        default:
            break;
        }
    }
}

void TheWar::plant_slot1()
{
    if(!pausing)
    {
        int i=plant_kind;//把plant-kind保存下来
        if(plant_kind!=-1)
        {
            ch_action[plant_kind].close();
            reset_button();
        }
        this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        if (totalSun >= 25&&i!=0)
        {
            creat_plant_label(0);
            ch_bar[0].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_potatomine/card_potatomine1.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
        }

    }
}
void TheWar::plant_slot2()
{
    if(!pausing)
    {
        int i=plant_kind;
        if(plant_kind!=-1)
        {
            ch_action[plant_kind].close();
            reset_button();
        }
        this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        if (totalSun >= 150&&i!=1)
        {
            creat_plant_label(1);
            ch_bar[1].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_cherrybomb/card_cherrybomb1.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
        }
    }
}
void TheWar::plant_slot3()
{
    if(!pausing)
    {
        int i=plant_kind;//把plant-kind保存下来，因为reset button会重置plant-kind；
        if(plant_kind!=-1)
        {
            ch_action[plant_kind].close();
            reset_button();
        }
        this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        if (totalSun >= 100&&i!=2)
        {
            creat_plant_label(2);
            ch_bar[2].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_peashooter/card_peashooter1.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
        }
    }
}
void TheWar::plant_slot4()
{
    if(!pausing)
    {
        int i=plant_kind;//把plant-kind保存下来，因为reset button会重置plant-kind；
        if(plant_kind!=-1)
        {
            ch_action[plant_kind].close();
            reset_button();
        }
        this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        if (totalSun >= 175&&i!=3)
        {
            creat_plant_label(3);
            ch_bar[3].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_snowpea/card_snowpea1.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
        }
    }
}
void TheWar::plant_slot5()
{
    if(!pausing)
    {
        int i=plant_kind;//把plant-kind保存下来，因为reset button会重置plant-kind；
        if(plant_kind!=-1)
        {
            ch_action[plant_kind].close();
            reset_button();
        }
        this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        if (totalSun >= 50&&i!=4)
        {
            creat_plant_label(4);
            ch_bar[4].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_sunflower/card_sunflower1.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
        }
    }
}
void TheWar::plant_slot6()
{
    if(!pausing)
    {
        int i=plant_kind;//把plant-kind保存下来，因为reset button会重置plant-kind；
        if(plant_kind!=-1)
        {
            ch_action[plant_kind].close();
            reset_button();
        }
        this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        if (totalSun >= 50&&i!=5)
        {
            creat_plant_label(5);
            ch_bar[5].setStyleSheet(
                        "QPushButton{"
                        "background: url(:/new/prefix1_plant_cards/image/plant_card/card_wallnut/card_wallnut1.jpg);"
                        "border-radius:10px;"
                        "}"
                        );
        }
    }
}
void TheWar::sendSlot()
{
    button_music = new QMediaPlayer;
    button_music->setMedia(QUrl("qrc:/new/prefix1/music/button/buttonclick.mp3"));
    button_music->setVolume(50);
    button_music->play();
    b_g_music->stop();
    delete b_g_music;
    plantsGenerateTimer->stop();
    checkerTimer->stop();
    sceneTimer->stop();
    thisGame->stopmusic();
    //  delete thisGame;

    for(int i = 0; i < int(existingZombies.size()); i++)
    {
        if(existingZombies.at(i)->getAlive())
            existingZombies.at(i)->Zombieboomed();
    }

    emit SignalReturnMenu();
}

void TheWar::generateZombie()
{
    int ZombieType;
    if(thisGame->getWaveLeader())
    {
        ZombieType = 3;
    }
    else if(rand()%10+1  <= 7)
    {
        ZombieType = 0;
    }
    else if(rand()%10+1  <= 9)
    {
        ZombieType = 1;
    }
    else
    {
        ZombieType = 2;
    }
    if(!pausing)
    {
        Zombie *aZombie=new Zombie(ZombieType,rand()%5 + 1);
        existingZombies.push_back(aZombie);
        aZombie->setSpeed(zombieSpeed);
        scene->addItem(aZombie);
    }
}

void TheWar::pause()
{
    button_music = new QMediaPlayer;
    button_music->setMedia(QUrl("qrc:/new/prefix1/music/button/pause.mp3"));
    button_music->setVolume(50);
    button_music->play();
    b_g_music->pause();

    pausing=true;
    pause_time=QTime::currentTime();
    wn_gap=pause_time.msecsTo(last_wn_cd)+cd_wallnut;
    sf_gap=pause_time.msecsTo(last_sf_cd)+cd_sunflower;
    sp_gap=pause_time.msecsTo(last_sp_cd)+cd_sonwpea;
    pea_gap=pause_time.msecsTo(last_pea_cd)+cd_peashooter;
    pm_gap=pause_time.msecsTo(last_pm_cd)+cd_potatomine;
    cb_gap=pause_time.msecsTo(last_cb_cd)+cd_cherrybomb;
    thisGame->resetpausing(pausing);
    // after_pause=false;
    this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    for(int i=0;i<int(existingZombies.size());i++)
    {
        existingZombies.at(i)->Changing_zombie_state();
    }
    for(int i=0;i<int(existingPlants.size());i++)
    {
        existingPlants.at(i)->Changing_plant_state();
    }
    for(int i=0;i<int(existingProjectiles.size());i++)
    {
        existingProjectiles.at(i)->Changing_projectile_state();
    }

    gap=pause_time.msecsTo(plants_generate)+5000;
    disconnect(b3,&QPushButton::pressed, this, &TheWar::pause);
    plantsGenerateTimer->stop();
    if(if_wn_cd)
        killTimer(wllnut_cd_timer);
    if(if_sf_cd)
        killTimer(sunflower_cd_timer);
    if(if_sp_cd)
        killTimer(snowpea_cd_timer);
    if(if_pea_cd)
        killTimer(peashooter_cd_timer);
    if(if_cb_cd)
        killTimer(cherrybomb_cd_timer);
    if(if_pm_cd)
        killTimer(potatomine_cd_timer);
    connect(b3,&QPushButton::pressed, this, &TheWar::go_on);

}
void TheWar::go_on()
{
    b_g_music->play();
    pausing=false;
    this->pBackGround->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    thisGame->resetpausing(pausing);
    plantsGenerateTimer->start(gap);
    if(if_wn_cd)
    {
        if(wn_gap>=0)
            wllnut_cd_timer=startTimer(wn_gap);
        else
            wllnut_cd_timer=startTimer(cd_wallnut);
    }
    if(if_sf_cd)
    {
        if(sf_gap>=0)
            sunflower_cd_timer=startTimer(sf_gap);
        else
            sunflower_cd_timer=startTimer(cd_sunflower);
    }
    if(if_sp_cd)
    {
        if(sp_gap>=0)
            snowpea_cd_timer=startTimer(sp_gap);
        else
            snowpea_cd_timer=startTimer(cd_sonwpea);
    }
    if(if_pea_cd)
    {
        if(pea_gap>=0)
            peashooter_cd_timer=startTimer(pea_gap);
        else
            peashooter_cd_timer=startTimer(cd_peashooter);
    }
    if(if_cb_cd)
    {
        if(pm_gap>=0)
            cherrybomb_cd_timer=startTimer(pm_gap);
        else
            cherrybomb_cd_timer=startTimer(cd_cherrybomb);
    }
    if(if_pm_cd)
    {
        if(cb_gap>=0)
            potatomine_cd_timer=startTimer(cb_gap);
        else
            potatomine_cd_timer=startTimer(cd_potatomine);
    }

    after_pause=true;
    for(int i=0;i<int(existingZombies.size());i++)
    {
        existingZombies.at(i)->Changing_zombie_state();
    }
    for(int i=0;i<int(existingPlants.size());i++)
    {
        existingPlants.at(i)->Changing_plant_state();
    }
    for(int i=0;i<int(existingProjectiles.size());i++)
    {
        existingProjectiles.at(i)->Changing_projectile_state();
    }
    disconnect(b3,&QPushButton::pressed, this, &TheWar::go_on);
    connect(b3,&QPushButton::pressed, this, &TheWar::pause);
}
void TheWar::cherrybooming(int line, int column)
{
    delete cherrybomb_music;
    cherrybomb_music = new QMediaPlayer;
    cherrybomb_music->setMedia(QUrl("qrc:/new/prefix1/music/plant/explosion.mp3"));
    cherrybomb_music->setVolume(50);
    cherrybomb_music->play();
    int left,right,up,down;
    switch (line)
    {
    case 1:
        up = 30;
        down = 175;
        break;
    case 2:
        up = 30;
        down = 330;
        break;
    case 3:
        up = 175;
        down = 465;
        break;
    case 4:
        up = 320;
        down = 610;
        break;
    case 5:
        up = 465;
        down = 610;
        break;
    default:
        break;
    }
    switch (column)
    {
    case 1:
        left = -130;
        right = 240;
        break;
    case 2:
        left = -130;
        right = 370;
        break;
    case 3:
        left = 0;
        right = 500;
        break;
    case 4:
        left = 130;
        right = 640;
        break;
    case 5:
        left = 270;
        right = 770;
        break;
    case 6:
        left = 400;
        right = 900;
        break;
    case 7:
        left = 540;
        right = 1030;
        break;
    case 8:
        left = 670;
        right = 1170;
        break;
    case 9:
        left = 800;
        right = 1300;
        break;
    default:
        break;
    }
    int die_zombie[30];
    int count_die = 0;
    for(int i=0; i<int(existingZombies.size()); i++)
    {
        //        qDebug() << existingZombies.at(i)->x();
        if(existingZombies.at(i)->x() >= left && existingZombies.at(i)->x() <= right
                && existingZombies.at(i)->y() <= down && existingZombies.at(i)->y() >= up)
        {
            die_zombie[count_die++] = i;
        }
    }
    for(;count_die > 0;)
    {
        existingZombies.at(die_zombie[--count_die])->Zombieboomed();
    }
}

void TheWar::potatominebooming(int line, int column)
{

    int left,right,up,down;
    switch (line)
    {
    case 1:
        up = 30;
        down = 30;
        break;
    case 2:
        up = 175;
        down = 175;
        break;
    case 3:
        up = 320;
        down = 320;
        break;
    case 4:
        up = 465;
        down = 465;
        break;
    case 5:
        up = 610;
        down = 610;
        break;
    default:
        break;
    }
    switch (column)
    {
    case 1:
        left = 0;
        right = 130;
        break;
    case 2:
        left = 120;
        right = 250;
        break;
    case 3:
        left = 240;
        right = 380;
        break;
    case 4:
        left = 370;
        right = 500;
        break;
    case 5:
        left = 490;
        right = 610;
        break;
    case 6:
        left = 600;
        right = 740;
        break;
    case 7:
        left = 730;
        right = 860;
        break;
    case 8:
        left = 850;
        right = 980;
        break;
    case 9:
        left = 930;
        right = 1110;
        break;
    default:
        break;
    }
    int die_zombie[30];
    int count_die = 0;
    for(int i=0; i<int(existingZombies.size()); i++)
    {
        //        qDebug() << existingZombies.at(i)->x();
        if(existingZombies.at(i)->x() >= left && existingZombies.at(i)->x() <= right
                && existingZombies.at(i)->y() <= down && existingZombies.at(i)->y() >= up)
        {
            die_zombie[count_die++] = i;
        }
    }
    for(;count_die > 0;)
    {
        existingZombies.at(die_zombie[--count_die])->Zombieboomed();
    }
}
void TheWar::adjust(int ZombieSpeed)
{
    zombieSpeed=5+2*(ZombieSpeed-3);
}
void TheWar::checkZombie()
{
    ifZombieEnd=true;
}
//一种插入图片方式
//    QGraphicsScene* scene = new QGraphicsScene(this);
//    QImage map1(":/new/prefix1_background/image/b_g/map1.jpg");
//    QGraphicsPixmapItem item( QPixmap::fromImage(map1));
//    scene->addItem(&item);
//    QGraphicsView view(scene);
