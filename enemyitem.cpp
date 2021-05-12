#include "enemyitem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include "scene.h"
#include "birditem.h"
#include "shootitem.h"
#include <QDebug>


EnemyItem::EnemyItem():
    enemy(new QGraphicsPixmapItem(QPixmap(":/images/enemy.png"))),
    pastBird(false)
{
    enemy->setPos(QPointF(0,0) - QPointF(enemy->boundingRect().width(),
                      enemy->boundingRect().height()));

    addToGroup(enemy);

    yPos = QRandomGenerator::global()->bounded(-250,250);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    setPos(QPoint(0,0) + QPoint(260 + xRandomizer, yPos));

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-300);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(2000);

    connect(xAnimation, &QPropertyAnimation::finished, [=](){
        scene()->removeItem(this);
        delete this;
    });

    xAnimation->start();
}

EnemyItem::~EnemyItem()
{
    delete enemy;
}

qreal EnemyItem::x() const
{
    return m_x;
}

void EnemyItem::freezeInPlace()
{
    xAnimation->stop();
}

void EnemyItem::setX(qreal x)
{
    m_x = x;

    if(x < 0 && !pastBird){
        pastBird = true;
    }

    if(collidesWithBird()){
        emit collideFail();
    }

    if(collidesWithShoot()){
        emit collideShoot();
    }

    setPos(QPoint(0,0)+ QPoint(x, yPos));
}

bool EnemyItem::collidesWithBird()
{
    QList<QGraphicsItem*> collidingItems = enemy->collidingItems();

    foreach(QGraphicsItem * item, collidingItems){
        BirdItem * birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem){
            if(!collidedBefore){
                collidedBefore = true;
                int lives = birdItem->lives;
                lives--;
                birdItem->lives = lives;
                if(lives==0){
                    return true;
                }else{
                    xAnimation->stop();
                    delete this;
                }
            }
        }
    }
    return false;
}

bool EnemyItem::collidesWithShoot()
{
    QList<QGraphicsItem*> collidingItems = enemy->collidingItems();

    foreach(QGraphicsItem * item, collidingItems){
        ShootItem * shootItem = dynamic_cast<ShootItem*>(item);
        if(shootItem){
            QGraphicsScene * mScene = scene();
            Scene * myScene = dynamic_cast<Scene *>(mScene);
            if(myScene){
                //QSound::play("sounds/click.mp3");
                //player2->play();
                delete shootItem;
                myScene->incrementScore();
            }
            return true;
        }
    }
    return false;
}

