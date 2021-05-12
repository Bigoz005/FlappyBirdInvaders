#ifndef LASERITEM_H
#define LASERITEM_H

#include <BossItem.h>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class LaserItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)

public:
    LaserItem(BossItem * bossItem);
    ~LaserItem();

    qreal x() const;
    int yPos;
    int xPos;
    void freezeInPlace();
    QGraphicsPixmapItem * laser;

signals:
    void collideWithBird();

public slots:
    void setX(qreal x);

private:
    bool collidesWithBird();
    bool collidedBefore = false;
    QPropertyAnimation * xAnimation;
    qreal m_x;
};

#endif // LASERITEM_H
