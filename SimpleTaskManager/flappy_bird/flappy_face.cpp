#include "flappy_face.h"
#include "ui_flappy_face.h"
#define window_x 572
#define window_y 572
double gravity=0.6;
int frame=16;
int speed=8;
Flappy_face::Flappy_face(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Flappy_face)
{
    setWindowIcon(QPixmap(":/smile.png"));
    cpu_percent=0;
    score=0;
    missle_magzine=0;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    setFixedSize(window_x,window_y);
    ui->setupUi(this);
    status=false;
    timer=new QTimer(this);
    block_creator_timer=new QTimer(this);
    auto_shoot=new QTimer(this);
    face=new Object(":/smile.png",30,150,gravity,0,72,72,46,46,13,13);
    background=new Object(":/background.png",0,0,0,0,window_x,window_y,0,0,0,0);
    ground=new Object(":/ground.png",0,window_y-80,0,speed,800,80,0,0,0,0);
    numbers=QPixmap(":/numbers.png");
    shop=QPixmap(":/shop.png");
    with_gun=false;
    with_umbrella=false;
    have_syringe=false;
    player=new QSound(":/bgm.wav",this);
    player->play();
}
void Flappy_face::med_starts()
{
    if(missle_magzine<=0)
        return;
    --missle_magzine;
    face->size_x=36;
    face->size_y=36;
    face->collision_x=23;
    face->collision_y=23;
    face->bias_x=7;
    face->bias_y=7;
    QTimer::singleShot(5000,this,SLOT(med_ends()));
}
void Flappy_face::med_ends()
{
    face->size_x=72;
    face->size_y=72;
    face->collision_x=46;
    face->collision_y=46;
    face->bias_x=13;
    face->bias_y=13;
}
void Flappy_face::syringe_starts()
{
    if(have_syringe)
        return;
    if(missle_magzine<=0)
        return;
    --missle_magzine;
    have_syringe=true;
    face->image=QPixmap(":/sunglasses.png");
    speed=4;
    ground->v=4;
    block_creator_timer->stop();
    block_creator_timer->start(1000);
    for(int i=0;i<blocks.size();++i)
    {
        blocks[i]->v=4;
    }
    QTimer::singleShot(5000,this,SLOT(syringe_ends()));
}
void Flappy_face::syringe_ends()
{
    have_syringe=false;
    face->image=QPixmap(":/smile.png");
    speed=8;
    ground->v=8;
    block_creator_timer->stop();
    block_creator_timer->start(500);
    for(int i=0;i<blocks.size();++i)
        blocks[i]->v=8;
}
void Flappy_face::gun_starts()
{
    if(with_gun)
        return;
    if(missle_magzine<=0)
        return;
    --missle_magzine;
    with_gun=true;
    face->image=QPixmap(":/smile_with_gun.png");
    auto_shoot->start(50);
    QTimer::singleShot(5000,this,SLOT(gun_ends()));
}
void Flappy_face::gun_shoot()
{
    Object* new_missle=new Object(":/bullet.png",face->x+60,face->y+40,0,speed*2,32,32,32,32,0,0);
    missles.push_back(new_missle);
}
void Flappy_face::gun_ends()
{
    face->image=QPixmap(":/smile.png");
    auto_shoot->stop();
    with_gun=false;
}
Flappy_face::~Flappy_face()
{
    delete ui;
}
void Flappy_face::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(background->x,background->y,background->size_x,background->size_y,background->image);
    painter.drawPixmap(ground->x,ground->y,ground->size_x,ground->size_y,ground->image);
    for(int i=0;i<blocks.size();++i)
        painter.drawPixmap(blocks[i]->x,blocks[i]->y,blocks[i]->size_x,blocks[i]->size_y,blocks[i]->image);
    for(int i=0;i<missles.size();++i)
        painter.drawPixmap(missles[i]->x,missles[i]->y,missles[i]->size_x,missles[i]->size_y,missles[i]->image);
    painter.drawPixmap(face->x,face->y,face->size_x,face->size_y,face->image);
    int n0=score%10;
    int n1=(score%100)/10;
    int n2=score/100;
    painter.drawPixmap(window_x-36, 0,36,36, numbers, n0 * 72, 0, 72, 72);
    painter.drawPixmap(window_x-72, 0,36,36, numbers, n1 * 72, 0, 72, 72);
    painter.drawPixmap(window_x-108, 0,36,36,numbers, n2 * 72, 0, 72, 72);
    for(int i=0;i<missle_magzine;++i)
    {
        painter.drawPixmap(572-36*(i+1),572-36,36,36,QPixmap(":/fist.png"));
    }
    QPixmap percentage(":/timenumber.bmp");
    int p1=cpu_percent%10;
    int p2=cpu_percent/10;
    painter.drawPixmap(window_x/2, window_y-28, percentage, p1 * 20, 0, 20, 28);
    painter.drawPixmap(window_x/2-20, window_y-28, percentage, p2 * 20, 0, 20, 28);
    if(with_umbrella)
    {
        painter.drawPixmap(face->x+10,face->y,face->size_x,face->size_y,QPixmap(":/umbrella.png"));
    }
    painter.drawPixmap(0,0,shop);
}
void Flappy_face::start()
{

    med_ends();
    syringe_ends();
    gun_ends();
    delete timer;
    delete block_creator_timer;
    timer=new QTimer(this);
    block_creator_timer=new QTimer(this);
    status=true;
    connect(timer, SIGNAL(timeout()), this, SLOT(action()));
    connect(block_creator_timer, SIGNAL(timeout()), this, SLOT(new_block()));
    connect(auto_shoot, SIGNAL(timeout()), this, SLOT(gun_shoot()));
    timer->start(frame);
    block_creator_timer->start(500);
}
void Flappy_face::action()
{
    int temp=0;
    face->action(0,1);
    if(ground->x<=-ground->size_x/10)
        ground->x=0;
    ground->action(-1,0);
    for(auto iter=missles.begin();iter!=missles.end();)
    {
        if((*iter)->x>window_x)
        {
            delete (*iter);
            iter=missles.erase(iter);
        }
        else
        {
            (*iter)->action(1,0);
            ++iter;
        }
    }
    for(auto iter=blocks.begin();iter!=blocks.end();)
    {
        bool destroyed=false;
        for(auto iter_mis=missles.begin();iter_mis!=missles.end();++iter_mis)
        {
            if((*iter)->check_collsion((*iter_mis)))
            {
                missles.erase(iter_mis);
                iter=blocks.erase(iter);
                destroyed=true;
                break;
            }
        }
        if(destroyed)
            continue;
        if((*iter)->x<-(*iter)->size_x)
        {
            ++temp;
            delete (*iter);
            iter=blocks.erase(iter);
        }
        else
        {
            (*iter)->action(-1,0);
            ++iter;
        }
    }
    if(temp/2>0)
        update_score(temp/2);
    update();
    if(face->y>window_y-face->size_y-ground->size_y)
    {
        /*update();
        stop();
        return;*/
        face->v*=-1;
    }
    for(auto iter=blocks.begin();iter!=blocks.end();++iter)
    {
        if((*iter)->check_collsion(face))
        {
            if(with_umbrella)
            {
                delete (*iter);
                iter=blocks.erase(iter);
                with_umbrella=false;
                return;
            }
            else
            {
                update();
                stop();
                return;
            }
        }
    }
}
void Flappy_face::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_5)
        syringe_starts();
    else if(event->key()==Qt::Key_4)
        med_starts();
    else if(event->key()==Qt::Key_3)
        gun_starts();
    else if(event->key()==Qt::Key_2)
        launch_rocket();
    else if(event->key()==Qt::Key_1)
        release_umbrella();
}
void Flappy_face::update_score(int num)
{
    score+=num;
    /*if (score==20)
        face->image=QPixmap(":/sunglasses.png");
    else if(score==50)
        face->image=QPixmap(":/stars.png");
    else if(score==100)
        face->image=QPixmap(":/elder.png");*/
    if (score%5==0)
        if (missle_magzine<5)
            ++missle_magzine;
}
void Flappy_face::stop()
{
    setWindowIcon(QPixmap(":/sweat.png"));
    face->image=QPixmap(":/sweat.png");
    timer->stop();
    block_creator_timer->stop();
    QTimer::singleShot(500,this,SLOT(wait_1_sec()));
    QSound::play(":/death.wav");
}
void Flappy_face::restart()
{
    setWindowIcon(QPixmap(":/smile.png"));
    cpu_percent=0;
    score=0;
    face->image=QPixmap(":/smile.png");
    face->y=150;
    face->v=0;
    ground->x=0;
    missle_magzine=0;
    missles.clear();
    blocks.clear();
    start();
}
void Flappy_face::wait_1_sec()
{
    status=false;
}
void Flappy_face::mousePressEvent(QMouseEvent *event)
{
    if(status)
    {
        if(face->y>-face->size_x)
        {
            if (event->button() == Qt::RightButton)
               launch_missle();
            else
            {
                face->v=-8;
            }
        }
    }
    else
        restart();
}
void Flappy_face::launch_missle()
{
    if(with_gun)
    {
        Object* new_missle=new Object(":/bullet.png",face->x+60,face->y,0,speed*2,32,32,32,32,0,0);
        missles.push_back(new_missle);
    }
    else
    {
        if(missle_magzine<=0)
            return;
        --missle_magzine;
        Object* new_missle=new Object(":/fist.png",face->x+60,face->y,0,speed*2,72,72,72,72,0,0);
        missles.push_back(new_missle);
    }
}
void Flappy_face::launch_rocket()
{
    if(missle_magzine<=0)
        return;
    --missle_magzine;
    Object* rocket1=new Object(":/rocket.png",face->x+60,face->y,0,speed,72,72,72,72,0,0);
    Object* rocket2=new Object(":/rocket.png",face->x+60,face->y,0,speed,72,72,72,72,0,0);
    Object* rocket3=new Object(":/rocket.png",face->x+60,face->y,0,speed,72,72,72,72,0,0);
    Object* rocket4=new Object(":/rocket.png",face->x+60,face->y,0,speed,72,72,72,72,0,0);
    Object* rocket5=new Object(":/rocket.png",face->x+60,face->y,0,speed,72,72,72,72,0,0);
    missles.push_back(rocket1);
    missles.push_back(rocket2);
    missles.push_back(rocket3);
    missles.push_back(rocket4);
    missles.push_back(rocket5);
}
void Flappy_face::release_umbrella()
{
    if (with_umbrella)
        return;
    if(missle_magzine<=0)
        return;
    --missle_magzine;
    with_umbrella=true;
}
void Flappy_face::new_block()
{
    //int height=qrand()%350;
    cpu_percent=monitor.GetCPUUseRate();
    int height=(cpu_percent*window_y)/100%400;
    monitor.Initialize();
    if(height>=100)
    {
        Object* new_block1=new Object(":/cactus_side.png",window_x,window_y-height,0,speed,64,height,64,height,0,0);
        Object* new_block2=new Object(":/cactus_side.png",window_x,0,0,speed,64,window_y-height-200,64,window_y-height-200,0,0);
        blocks.push_back(new_block1);
        blocks.push_back(new_block2);
    }
}
