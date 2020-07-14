# PVZ-project-at-NKU
Created by 陈俊升,程子果,董继煜,张嘉诚 from July 1 to July 10, 2020

#### 实验报告

### 摘要:

在这次高级语言编程实训中，我们自7月1日确定项目为游戏"植物大战僵尸"后，尽管有前端实训的任务，仍全身心地投入到Qt的学习与游戏的制作中，并于7月11日基本圆满完成项目。

在项目前期，我们一方面认真学习Qt的基本知识，一方面寻找查阅相关项目的信息，既收集了大量游戏的素材以供后期使用，又对将要制作的游戏有了大体的框架。在基本了解Qt后，我们开始了制作，决定用实践促进学习。我们首先将任务分为界面与逻辑，组员各司其职，确定了以QGraphics类为基础的游戏逻辑。

筑实了游戏的基本盘后，我们通过比对原版游戏，讨论制作了我们在功能、界面、音乐、创新方面可以执行的任务，并给出优先度，组员清楚有条理的领取并独立完成任务，对项目执行的效率与清晰度均有了很大的提升。小组成员之间分工虽然明确，但交流氛围极为活跃，每个人都对整体代码十分了解，互帮互助在提高积极性的同时增强了每人的能力。

经过十天的艰苦奋斗，最终我们成功得到一个功能较为复杂， 界面交互友好的，可以正常游玩且相对原版有所创新的"植物大战僵尸"游戏。报告将以此介绍分工情况，成果展示，源文件分析与心得体会，最后是反思、总结和核心代码附录。





### 分工情况

陈俊升

1. 土豆雷和樱桃炸弹的实现,以及其对炸死僵尸的动画变换
2. 阶段的逻辑与动画实现,僵尸随机生成与僵尸吃植物与死亡动画的实现
3. 植物根据是否有僵尸决定是否发射子弹的实现
4. 负责课堂演示PPT的制作

程子果

1. 实现两个欢迎界面和战斗窗口分离，并可以返回菜单。参与游戏暂停、继续功能的实现。
2. 实现用户可调整游戏参数（如僵尸移动速度）以获得多样的游戏体验
3. 完成游戏胜利与失败界面的部分,完善用户放回、重选植物的操作
4. 负责报告中源文件分析部分

董继煜

1. 寻找并处理游戏图片、音乐素材,并独立完成所有音乐音效部分
2. 完成选取、种植植物，鼠标点击,植物跟随,植物冲突等相关功能,
3. 参与暂停继续功能的实现,多次聚合代码，处理bug
4. 负责课堂演示的讲述

张嘉诚:

1. 整合、优化、精简类的分工，基本确定整体结构，完成发射物类、阳光类、小车类，确定用vector储存各类元素的思路
2. 将游戏背景改造为QGraphicsScene类，将QLabel子类改造成QGraphicsItem子类，以实现游戏中的碰撞逻辑
3. 学习并使用advance槽以实现各运动类的动画效果
4. 负责实验报告的部分撰写与排版





##### 成果展示:(文字与图片)

跨平台编译成功，具有良好的移植性

功能:

1. 正常游戏的进行：各类僵尸的实现（旗帜僵尸、普通僵尸、路障僵尸、铁桶僵尸等），各类植物的实现（豌豆射手，寒冰射手，土豆雷，樱桃炸弹，向日葵，坚果），以及僵尸与植物的互相攻击，自然阳光的随机生成，僵尸随阶段不同的随机生成，胜负的检测
2. 小车对僵尸的检测与击杀
3. 暂停、继续游戏与返回菜单的功能实现
4. 选择植物时的卡片跟随与放回操作
5. 玩家可调整僵尸速度对应不同难度

界面：

1. 欢迎界面与游戏界面的分离与互连
2. 游戏右下角的阶段提示动画，一大波、最终波僵尸的视觉提醒
3. 胜利与失败的界面，可返回菜单重新开始
4. 阳光落下、产生与收集的流畅动画
5. 僵尸吃植物的动画，正常死亡与炸死动画的切换

音乐：

1. 游戏欢迎界面与游戏界面的背景音乐
2. 种植植物、收集阳光、车子移动的音效
3. 发射子弹与子弹攻击的音效，樱桃、土豆雷的爆炸音效
4. 僵尸吃植物的音效，阶段变换的音效等



##### 源文件分析:

本次我们植物大战僵尸游戏的实现包含10个源文件，分别为main主函数、Welcome（欢迎界面）、Plant（植物类）、Projectile（植物发射物类）、Zombie（僵尸类）、Sun（阳光类）、Car（小车类）、BackgroundLabel（战斗界面背景类）、Level（游戏阶段与难度类）、TheWar（战斗界面）。

- main：主函数

  将Welcome实例化，show后进入事件循环。

- Welcome：欢迎界面类。

1. 使用paintevent进行背景的填充
2. 内有“冒险模式”按钮，并用槽函数机制来实现点击“冒险模式”按钮后才初始化TheWar界面
3. 使用QSpinBox、QSlider实现调整僵尸移动速度的微调框和滑条
4. 自动播放背景音乐

- Plant：继承QGraphicsPixmapItem的植物类

1. 类中定义植物的生命值，攻击力，攻击速度，价格，是否碰撞等

2. setType函数实现每种植物各类属性的初始化。setPosition函数实现植物的位置选择

3. 使用一系列set和get函数来设置或获得植物属性。

4. 通过重载advance函数，并在TheWar里通过计时器不断调用来显示出动画效果（以下继承自QGraphicsPixmapItem均同此）

- Projectile：继承QGraphicsPixmapItem的植物发射物类

1. 类中定义该发射物的攻击力以及是否会提供减速效果。

2. 使用setAttack初始化攻击力。使用setSlow实现改变是否有对僵尸的减速效果。

- Zombie：继承QGraphicsPixmapItem的僵尸类

1. 类中定义僵尸的生命值，攻击力，攻击速度，是否碰撞等。

2. setType函数实现每种僵尸各类属性的初始化。setPosition函数实现僵尸出现的位置选择。

3. 使用一系列set和get函数来设置或获得僵尸属性。

- Sun：继承QGraphicsPixmapItem的阳光类

1. 通过mousePressEvent改变阳光是否被点击的状态。

2. 通过mousePressEvent函数来判断阳光是否被点击。如果被点击，则通过在TheWar里反复调用advance函数以及当前坐标、最终坐标和原始坐标实现阳光到达最终位置后消失的动画效果。

- Car：继承QGraphicsPixmapItem的小车类

  通过在TheWar里对僵尸与小车碰撞的判断，改变小车ready状态，如果ready，则小车发动，通过碰撞检测“撞死“沿途僵尸。

-  BackgroundLabel：战斗界面背景类。

1. 使用mouseMoveEvent和mousePressEvent两个事件函数实现对鼠标移动位置和鼠标点击位置的追踪，以实现选中植物后，卡片的跟随提醒，提高交互性

2. 使用judge_lawn函数来将上两个事件函数返回的像素点值转换为行列值，方便植物的种植。

- Level：游戏阶段与难度类

1. 用QLabel实现右下角以及大波僵尸来临的阶段提示，并在timeEvent中通过计时器实现阶段提示的动画效果。

2. 内含int型的level变量，来表示目前游戏阶段：1是普通阶段，2是尸潮阶段（僵尸出现频率更快），并在timeEvent函数中实现阶段的切换。

3. 在产生完毕所有僵尸后，发送信号给TheWar，表明僵尸已经产生完毕，可以进行游戏胜利判断。

- TheWar：实现植物与僵尸战斗的界面类，游戏的主进程

1. 用Qlabel设置植物选择框，阳光数量显示框及游戏结束界面。

2. 用QPushButton设置植物选择按钮，实现植物的选择。

3. 用BackgroundLabel类定义战斗界面的背景，使其可以获取鼠标位置，并用createPlant函数实现“种下”植物。

4. generateZombie函数与level类配合，共同实现僵尸的随机产生。

5. PlantChecker函数实现与植物直接相关的检查器，如植物与僵尸的碰撞、植物生成的阳光。

6. PlantsGenerate函数实现植物产生物，包括发射物及阳光等。

7. 产生植物、植物生成物和僵尸都使用vector来储存的。僵尸与植物，僵尸与小车，僵尸与植物发射物的碰撞主要是依靠collidesWithItem函数来检测的。

8. GameOverChecker函数实现检验游戏是否失败，checkZombie函数检验游戏是否胜利。并根据游戏结果的不同显示不同的游戏结束界面。

9. Pause、goto函数实现游戏的暂停和继续。



##### 心得体会:

陈俊升:

1. 应该要建立好的代码分享方式，不然在对接各自实现的功能时会造成很多的麻烦。
2. 应该在编写代码前清晰地制作出程序的预期功能和要实现这些功能而编写的代码，这样写代码的时候就会有较强的目的性，而不是想到什么就写什么，那样会造成编写的混乱。
3. C++的封装性能让程序分成分工严明的几个模块，让源码显得简洁好读，在编写程序的时候应该好好利用C++语言的这一特性。
4. 在合作项目中，不应该严格指明任务后，事不关己则漠不关心，主动承担任务提供帮助，不仅能促进团队的协作，还能起到锻炼自己编程能力的作用。

程子果:

1. 信息检索能力的重要性。虽然现在网络发达，但是网络上的信息错综复杂，想要快速找到符合自己要求的信息极其不容易。就比如这次大作业中对qt的使用，就因为检索能力的限制，在寻找信息的过程中花费了大量本不必要的时间。所以我认为以后上网检索信息时首先要明确自己的目的，清楚的表达自己的需求，这样才能找到真正有用的东西。
2. 分配任务的优越性。前几次团队合作写的大作业我感觉并没有真正发挥团队协作的价值，可能是多了前几次的经验，这次团队协作的效率很高。我们各司其职，各自领得各自的任务，大家的任务明确，团队成员也不会迷茫。
3. C++类封装的便利性。以前大作业基本把整个文件写在一个cpp里面，但在这次项目中，我感受到C++的这种特性对代码简明性和复用性的巨大好处，比如游戏中的zombie和plant类。

董继煜:

1. 作为一个团队合作项目，应当在开始对项目有一个比较具体的构想，便于后续分工以及提高每个团队成员的工作效率；并且注释应当合适且易于理解，利于成员间对接代码；且多进行组内讨论交流有利于项目工作的开展
2. 这是第二次使用Qt写项目，经过这次练习，更加加深了我对Qt的理解，尤其是Qt最重要的机制：信号和槽；同时也加深了我对c++类的理解，本次项目中，我们重写了大量的Qt自身的基类来实现我们需要的功能
3. 由于Qt的特殊性，这样的项目debug存在一定的难度，但是通过这样的项目练习，debug的效率和质量也逐步提升

张嘉诚:

1. Qt方面：学习并使用Qt的过程中，感受到C++面向对象的优越，在阅读其说明详细的文档时，既学习到如何使用，又学习到如何实现，封装精巧的Qt巧妙地使用了诸多在C++课程中提到的但不知道实用性在哪的特性，使我们对C++的理解又更深一步，还有Qt对跨平台开发的兼容性与qrc资源文件管理素材的方便性，
2. 项目方面：感受到在项目之处便确定大体框架的重要性。在实现图形碰撞时，需要将已实现的部分代码改造成另一个基类，增加了一些本不必要的工作量
3. 合作方面：感受到一个气氛活跃积极的小组对个人及小组项目的促进作用。原本我认为将项目分割明确才能提高效率，但活跃的组内交流虽然增加了一定的交流成本，效率却反而更高，个人收获也更多

##### 反思与总结:

不可避免地，我们在项目构建中也遇到了种种问题，第一是代码聚合时的效率较低，消耗了大量本可以用于项目的时间，学习git可能对于版本控制和团队协作效率将有明显提升；第二是游戏逻辑的整体架构应先行于项目开始，不致于开始项目后因逻辑的不吻合而耗费较多时间在对现有代码的大改造上；第三是之前很少考虑的性能问题，在测试中，较多的僵尸将明显降低游戏的帧率，此次较大的图形项目的经验让我们感受到了对代码性能考量的重要性。

值得说明的是，我们之所以没有为游戏加入更多关卡、植物与僵尸，是因为我们已基本完成了游戏结构的搭建，且代码具有良好的扩展性与可移植性，内容的再增加更多的将只是一些重复性工作，对实训能力的提升并不大。

虽然遇到了种种小问题，但总的来说，此次团队项目，我们前期准备充足，执行过程中分工明确且交流频繁有效。熟练地使用了Qt与C++的特性，实践与学习相互促进，在保持积极性的同时取得了良好的效果。最终得到了功能较为复杂，界面交互友好的“”植物大战僵尸“游戏



##### 核心代码附录:

startGame()函数,重点在用计时器的信号与各槽对应,实现对页面的刷新与逻辑的检查

```c++
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

```

PlantsChecker()函数,实现僵尸对植物的攻击,与土豆类对僵尸的伤害

```c++
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
    }
}
```

PlantsGenerate()函数,实现各植物的产生物逻辑(子弹、炸弹、阳光等)，与暂停功能的部分逻辑

```c++
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
```

