#include "bossitem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include "scene.h"
#include "birditem.h"
#include "shootitem.h"
#include "laseritem.h"
#include <QDebug>


BossItem::BossItem():
    boss(new QGraphicsPixmapItem(QPixmap(":/images/boss.png"))),
    bossLives(10),
    pastBird(false)
{
    boss->setPos(QPointF(0,0) - QPointF(boss->boundingRect().width(),
                      boss->boundingRect().height()));

    addToGroup(boss);

    yPos = QRandomGenerator::global()->bounded(-250,250);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    setPos(QPoint(0,0) + QPoint(260 + xRandomizer, yPos));

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(100);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    connect(xAnimation, &QPropertyAnimation::finished, [=](){
        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene *>(mScene);
        xPos = x();
        yPos = y();
        myScene->bossTimer->stop();
        setUpLaserTimer();
        laserTimer->start(500);
        y1Animation->setStartValue(y());
        y1Animation->start();
    });

    y1Animation = new QPropertyAnimation(this, "y", this);
    y1Animation->setStartValue(y());
    y1Animation->setEndValue(-150);
    y1Animation->setEasingCurve(QEasingCurve::Linear);
    y1Animation->setDuration(1500);

    connect(y1Animation, &QPropertyAnimation::finished, [=](){
            y2Animation->setStartValue(y());
            y2Animation->start();
    });

    y2Animation = new QPropertyAnimation(this, "y", this);
    y2Animation->setStartValue(y());
    y2Animation->setEndValue(250);
    y2Animation->setEasingCurve(QEasingCurve::Linear);
    y2Animation->setDuration(1500);

    connect(y2Animation, &QPropertyAnimation::finished, [=](){
            y1Animation->setStartValue(y());
            y1Animation->start();
    });

    xAnimation->start();
}

BossItem::~BossItem()
{
    laserTimer->stop();
    QGraphicsScene * mScene = scene();
    Scene * myScene = dynamic_cast<Scene *>(mScene);
    myScene->bossCount--;
    myScene->enemyTimer->start(1000);
    myScene->bossTimer->stop();
    delete boss;
}

qreal BossItem::x() const
{
    return m_x;
}

qreal BossItem::y() const
{
    return m_y;
}

void BossItem::freezeInPlace()
{
    laserTimer->stop();
    xAnimation->stop();
    y1Animation->stop();
    y2Animation->stop();
}

void BossItem::setX(qreal x)
{
    m_x = x;

    if(x < 0 && !pastBird){
        pastBird = true;
        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene *>(mScene);
        if(myScene){
            //QSound::play("sounds/click.mp3");
            //player2->play();

            //myScene->incrementScore();
        }
    }


    if(collidesWithBird()){
        emit collideFail();
    }

    if(collidesWithShoot()){
        emit collideShoot();
        if(bossLives == 0){
            QGraphicsScene * mScene = scene();
            Scene * myScene = dynamic_cast<Scene *>(mScene);
            myScene->bossCount = 0;
            myScene->bird->lives++;
            qDebug()<<myScene->bird->lives;
        }
    }

    setPos(QPoint(0,0)+ QPoint(x, yPos));
}

void BossItem::setY(qreal y)
{
    m_y = y;

    if(collidesWithBird()){
        emit collideFail();
    }

    if(collidesWithShoot()){
        emit collideShoot();
        if(bossLives == 0){
            QGraphicsScene * mScene = scene();
            Scene * myScene = dynamic_cast<Scene *>(mScene);
            myScene->bossCount = 0;
            myScene->bird->lives++;
            qDebug()<<myScene->bird->lives;
            myScene->enemyTimer->start(1000);
            xAnimation->stop();
            y1Animation->stop();
            y2Animation->stop();
            delete this;
        }
    }

    setPos(QPoint(0,0)+ QPoint(xPos, y));
}

bool BossItem::collidesWithBird()
{
    QList<QGraphicsItem*> collidingItems = boss->collidingItems();

    foreach(QGraphicsItem * item, collidingItems){
        BirdItem * birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem){
            if(!collidedBefore){
                collidedBefore = true;
                int lives = birdItem->lives;
                qDebug()<<"Bird lifes before: "<<lives;
                lives--;
                qDebug()<<"Bird lifes after: "<<lives;
                birdItem->lives = lives;
                if(lives==0){
                    return true;
                }
            }
        }
    }
    return false;
}

bool BossItem::collidesWithShoot()
{
    QList<QGraphicsItem*> collidingItems = boss->collidingItems();

    foreach(QGraphicsItem * item, collidingItems){
        ShootItem * shootItem = dynamic_cast<ShootItem*>(item);
        if(shootItem){
            QGraphicsScene * mScene = scene();
            Scene * myScene = dynamic_cast<Scene *>(mScene);
            if(myScene){
                delete shootItem;
                myScene->incrementScore();
                qDebug()<<myScene->score;
                bossLives--;
            }
            return true;
        }
    }
    return false;
}

void BossItem::setUpLaserTimer()
{
    laserTimer = new QTimer(this);
    connect(laserTimer, &QTimer::timeout, [=](){

        LaserItem * laserItem = new LaserItem(this);
        connect(laserItem, &LaserItem::collideWithBird,[=](){
                emit collideFail();
        });

        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene *>(mScene);
        myScene->addItem(laserItem);
    });
}
