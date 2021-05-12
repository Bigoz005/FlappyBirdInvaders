#include "shootitem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include "scene.h"
#include "birditem.h"
#include <QDebug>
#include <scene.h>

ShootItem::ShootItem(BirdItem *birdItem):
    bullet(new QGraphicsPixmapItem(QPixmap(":/images/egg.png")))
{
    yPos = birdItem->pos().y()-50;
    xPos = birdItem->pos().x();

    bullet->setPos(QPoint(50, 50));
    addToGroup(bullet);

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(xPos);
    xAnimation->setEndValue(300);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    connect(xAnimation, &QPropertyAnimation::finished, [=](){
        delete bullet;
    });

    xAnimation->start();
}

ShootItem::~ShootItem()
{    
        delete bullet;
}

qreal ShootItem::x() const
{
    return m_x;
}

void ShootItem::freezeInPlace()
{
    xAnimation->stop();
}

void ShootItem::setX(qreal x)
{
    m_x = x;
    setPos(QPoint(0,0)+ QPoint(x, yPos));
}
