#ifndef SHOOTITEM_H
#define SHOOTITEM_H

#include <BirdItem.h>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class ShootItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)

public:
    ShootItem(BirdItem *birdItem);
    ~ShootItem();

    qreal x() const;
    int yPos;
    int xPos;
    void freezeInPlace();
    QGraphicsPixmapItem * bullet;

public slots:
    void setX(qreal x);

private:
    QPropertyAnimation * xAnimation;
    qreal m_x;
};

#endif // SHOOTITEM_H
