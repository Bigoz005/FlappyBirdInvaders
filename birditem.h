#ifndef BIRDITEM_H
#define BIRDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class BirdItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal y READ y WRITE setY)
public:
    explicit BirdItem(QPixmap pixmap);
    ~BirdItem();

    int lives;
    float energy = 100.0f;
    qreal y() const;
    qreal rotation() const;
    QTimer * birdWingsTimer;

    void shootUp();

    void startFlying();

    void freezeInPlace();

public slots:
    void setY(qreal y);
    void setRotation(qreal rotation);
    void rotateTo(const qreal &end, const int& duration, const QEasingCurve& curve);
    void fallToGroundIfNecessary();

signals:

private :
    enum WingPosition{
        Up,
        Middle,
        Down
    };

    void updatePixmap();

    WingPosition wingPosition;
    bool wingDirection;
    qreal m_y;
    qreal m_rotation;

    qreal groundPosition;
    qreal topPosition;
    QPropertyAnimation * yAnimation;
    QPropertyAnimation * rotationAnimation;
};

#endif // BIRDITEM_H
