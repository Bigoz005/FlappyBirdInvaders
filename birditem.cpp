#include "birditem.h"
#include <QTimer>
#include <QDebug>
#include <QGraphicsScene>

BirdItem::BirdItem(QPixmap pixmap):
    wingPosition(WingPosition::Up),
    wingDirection(0)
{
    setPixmap(pixmap);

    birdWingsTimer = new QTimer(this);
    connect(birdWingsTimer, &QTimer::timeout, [=](){
        updatePixmap();
    });

    setPos(QPoint(0,0) + QPoint(-240, scenePos().y()));

    birdWingsTimer->stop();
    groundPosition = scenePos().y() + 290;

    yAnimation = new QPropertyAnimation(this, "y", this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPosition);
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(1000);

    rotationAnimation = new QPropertyAnimation(this,"rotation", this);
}

BirdItem::~BirdItem()
{
    qDebug() << "Deleted Bird";
}

qreal BirdItem::y() const
{
    return m_y;
}

qreal BirdItem::rotation() const
{
    return m_rotation;
}

void BirdItem::shootUp()
{
    if(energy>=0){
        qreal curPosY = y();
        energy -= 1.5;
        if(curPosY > -280){
            yAnimation->stop();
            rotationAnimation->stop();

            yAnimation->setStartValue(curPosY);
            yAnimation->setEndValue(curPosY - scene()->sceneRect().height()/8);
            yAnimation->setEasingCurve(QEasingCurve::OutQuad);
            yAnimation->setDuration(285);

            connect(yAnimation, &QPropertyAnimation::finished,[=](){
                fallToGroundIfNecessary();
            });

            yAnimation->start();

            rotateTo(-20,200,QEasingCurve::OutCubic);
        }else{
            yAnimation->stop();
            rotationAnimation->stop();
            rotateTo(0,0,QEasingCurve::OutCubic);
            fallToGroundIfNecessary();
        }
    }
}

void BirdItem::startFlying()
{
    yAnimation->start();
    rotateTo(90,1000, QEasingCurve::InQuad);
}

void BirdItem::freezeInPlace()
{
    yAnimation->stop();
    rotationAnimation->stop();
    birdWingsTimer->stop();
}

void BirdItem::setY(qreal y)
{
    moveBy(0,y-m_y);
    m_y = y;
}

void BirdItem::setRotation(qreal rotation)
{
    m_rotation = rotation;

    QPointF c = boundingRect().center();
    QTransform t;

    t.translate(c.x(), c.y());
    t.rotate(rotation);
    t.translate(-c.x(), -c.y());
    setTransform(t);
}

void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);

    rotationAnimation->start();
}

void BirdItem::fallToGroundIfNecessary()
{
    if(y() < groundPosition){

        rotationAnimation->stop();

        yAnimation->setStartValue(y());
        yAnimation->setEndValue(groundPosition);
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setDuration(1000);

        yAnimation->start();

        if(energy>=0){
            rotateTo(90,1000, QEasingCurve::InCubic);
        }else{
            rotateTo(5,1000, QEasingCurve::InCubic);
        }
    }
}

void BirdItem::updatePixmap()
{
    if(wingPosition == WingPosition::Middle){

        if(wingDirection){
            //Up
            setPixmap(QPixmap(":/images/bird1.png"));
            wingPosition = WingPosition::Up;
            wingDirection = 0;
        }else{
            //Down
            setPixmap(QPixmap(":/images/bird3.png"));
            wingPosition = WingPosition::Down;
            wingDirection = 1;
        }

    }else{
        setPixmap(QPixmap(":/images/bird2.png"));
        wingPosition = WingPosition::Middle;
    }

}
