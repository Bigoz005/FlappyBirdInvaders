#ifndef ENEMYITEM_H
#define ENEMYITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <ShootItem.h>

class EnemyItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
public:
    explicit EnemyItem();
    ~EnemyItem();

    qreal x() const;
    int yPos;
    ShootItem * collidingBullet;

    void freezeInPlace();

signals:
    void collideFail();
    void collideShoot();

public slots:

    void setX(qreal x);

private:

    bool collidesWithBird();
    bool collidesWithShoot();
    QGraphicsPixmapItem * enemy;

    QPropertyAnimation * xAnimation;

    qreal m_x;
    bool pastBird;
    bool collidedBefore;
};

#endif // ENEMYITEM_H
