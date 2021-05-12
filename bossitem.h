#ifndef BOSSITEM_H
#define BOSSITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <ShootItem.h>

class BossItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
public:
    explicit BossItem();
    ~BossItem();

    qreal x() const;
    qreal y() const;
    int yPos;
    int xPos;
    void setUpLaserTimer();

    ShootItem * collidingBullet;
    QGraphicsPixmapItem * wood;

    QTimer * laserTimer;
    void freezeInPlace();

signals:
    void collideFail();
    void collideShoot();

public slots:

    void setX(qreal x);
    void setY(qreal y);

private:

    bool collidesWithBird();
    bool collidesWithShoot();
    QGraphicsPixmapItem * boss;

    QPropertyAnimation * xAnimation;
    QPropertyAnimation * y1Animation;
    QPropertyAnimation * y2Animation;

    qreal m_x;
    qreal m_y;
    int bossLives = 5;
    bool pastBird;
    bool collidedBefore;
};

#endif // BOSSITEM_H
