#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include "enemyitem.h"
#include "bossitem.h"
#include "shootitem.h"
#include "laseritem.h"
#include "birditem.h"
#include <QSoundEffect>
#include <QSound>

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit Scene(QObject *parent = nullptr);

    void addBird();
    void AddBullet();

    void startGame();
    void destroyBullet();

    bool getGameOn() const;
    void setGameOn(bool value);

    void incrementScore();

    void setScore(int value);

    int bossCount = 0;
    QTimer * enemyTimer;
    QTimer * bossTimer;

    int score;
    QString pointsString;
    QString energyString;

    BirdItem * bird;

signals:

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    bool firstPlay = true;
    void showGameOverGraphics();
    void hideGameOverGraphics();
    void setUpTexts();
    void cleanEnemies();
    void cleanBosses();
    void cleanBullets();
    void cleanLasers();
    void setUpEnemyTimer();
    void setUpBossTimer();
    void freezeAllInPlace();

    bool gameOn;

    int bestScore;

    QSoundEffect * hitEffect;
    QSoundEffect * scoreEffect;

    QGraphicsPixmapItem * gameOverPix;
    QGraphicsTextItem * pointsTextItem;
    QGraphicsTextItem * energyTextItem;
    QGraphicsTextItem * scoreTextItem;

};

#endif // SCENE_H
