#include "laseritem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include "scene.h"
#include "bossitem.h"
#include <QDebug>
#include <scene.h>

LaserItem::LaserItem(BossItem *bossItem):
    laser(new QGraphicsPixmapItem(QPixmap(":/images/laser.png"))),
    collidedBefore(false)
{
    yPos = bossItem->pos().y()-100;
    xPos = bossItem->pos().x()-200;

    laser->setPos(QPoint(50, 50));
    addToGroup(laser);

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(xPos);
    xAnimation->setEndValue(-350);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    connect(xAnimation, &QPropertyAnimation::finished, [=](){
        delete this;
    });

    xAnimation->start();
}

LaserItem::~LaserItem()
{
    delete laser;
}

qreal LaserItem::x() const
{
    return m_x;
}

void LaserItem::freezeInPlace()
{
    xAnimation->stop();
}

void LaserItem::setX(qreal x)
{
    m_x = x;

    if(collidesWithBird()){
        emit collideWithBird();
    }

    setPos(QPoint(0,0)+ QPoint(x, yPos));
}

bool LaserItem::collidesWithBird()
{
    QList<QGraphicsItem*> collidingItems = laser->collidingItems();

    foreach(QGraphicsItem * item, collidingItems){
        BirdItem * birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem){
            if(!collidedBefore){
                collidedBefore = true;
                qDebug()<<"bird hit";
                int lives = birdItem->lives;
                lives--;
                birdItem->lives = lives;
                qDebug()<<"bird lives: "<<birdItem->lives;
                if(lives==0){
                    return true;
                }else{
                    xAnimation->stop();
                    delete laser;
                }
            }
        }
    }
    return false;
}
