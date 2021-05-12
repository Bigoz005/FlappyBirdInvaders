#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QSound>

Scene::Scene(QObject *parent) : QGraphicsScene(parent),
    score(0), firstPlay(true), gameOn(false), bestScore(0)
{
    setUpBossTimer();
    setUpEnemyTimer();
    showGameOverGraphics();
}

void Scene::addBird()
{
    bird = new BirdItem(QPixmap(":/images/bird1.png"));
    addItem(bird);
}

void Scene::startGame()
{
    //enemies
    //if(!enemyTimer->isActive()){
      if(!bossTimer->isActive()){
        //Bird

        bird->startFlying();
        bird->birdWingsTimer->start(80);
        bird->lives = 5;
        bird->energy = 100;
        cleanEnemies();
        cleanBosses();
        cleanBullets();
        cleanLasers();
        setScore(0);
        setGameOn(true);
        hideGameOverGraphics();
        setUpTexts();
        bossTimer->start(5000);
      //  enemyTimer->start(1000);
    }
}

void Scene::setUpEnemyTimer()
{
    enemyTimer = new QTimer(this);
    connect(enemyTimer, &QTimer::timeout, [=](){

        EnemyItem * enemyItem = new EnemyItem();
        connect(enemyItem, &EnemyItem::collideFail,[=](){
            enemyTimer->stop();
            freezeAllInPlace();
            setGameOn(false);
            showGameOverGraphics();
        });


        connect(enemyItem, &EnemyItem::collideShoot, [=](){
                delete enemyItem;
        });

        addItem(enemyItem);
    });    
}

void Scene::setUpBossTimer()
{
        bossTimer = new QTimer(this);
        connect(bossTimer, &QTimer::timeout, [=](){

            BossItem * bossItem = new BossItem();
            connect(bossItem, &BossItem::collideFail,[=](){
                bossTimer->stop();
                freezeAllInPlace();
                setGameOn(false);
                showGameOverGraphics();
            });

            connect(bossItem, &BossItem::collideShoot, [=](){
                    //delete(bossItem->collidingBullet);
            }); 

            addItem(bossItem);
        });            
}


void Scene::AddBullet()
{
    if(getGameOn()){
        ShootItem * bulletItem = new ShootItem(bird);
        bulletItem->setZValue(2);
        addItem(bulletItem);
    }
}

void Scene::freezeAllInPlace()
{
    //Freeze bird
    bird->freezeInPlace();

    //Freeze enemys
    QList<QGraphicsItem*> sceneItems = items();
    foreach(QGraphicsItem *item, sceneItems){
        EnemyItem * enemy = dynamic_cast<EnemyItem *>(item);
        if(enemy){
            enemy->freezeInPlace();
        }
    }

    QList<QGraphicsItem*> bossItems = items();
    foreach(QGraphicsItem *item, sceneItems){
        BossItem * boss = dynamic_cast<BossItem *>(item);
        if(boss){
            boss->freezeInPlace();
        }
    }

    //Freeze bullets
    QList<QGraphicsItem*> bulletItems = items();
    foreach(QGraphicsItem *item, bulletItems){
        ShootItem * bullet = dynamic_cast<ShootItem *>(item);
        if(bullet){
            bullet->freezeInPlace();
        }
    }

    //Freeze lasers
    QList<QGraphicsItem*> laserItems = items();
    foreach(QGraphicsItem *item, laserItems){
        LaserItem * laser = dynamic_cast<LaserItem *>(item);
        if(laser){
            laser->freezeInPlace();
        }
    }
    //hitEffect->play();
}

void Scene::setScore(int value)
{
    score = value;
}

bool Scene::getGameOn() const
{
    return gameOn;
}

void Scene::setGameOn(bool value)
{
    gameOn = value;
}

void Scene::incrementScore()
{
    score++;

    if(score > bestScore){
        bestScore = score;
    }

    if(score %10 == 0 && score != 0){
        enemyTimer->stop();
        bossTimer->start(7000);
    }

    if(score%3==0){
        bird->energy += 10;
    }

}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space){
        if(gameOn){
            AddBullet();
            qDebug() << "SHOOT";
        }
    }

    if(event->key() == Qt::Key_S){
        if(gameOn){
            bird->shootUp();
            qDebug() << "JUMP";
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(gameOn){
            bird->shootUp();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::showGameOverGraphics()
{

    scoreTextItem = new QGraphicsTextItem();
    if(firstPlay){
        QString string = "Press start button to play";
        scoreTextItem->setHtml(string);
    }else{
        gameOverPix = new QGraphicsPixmapItem(QPixmap(":/images/Game_over.png"));
        gameOverPix->setZValue(100);
        addItem(gameOverPix);
        gameOverPix->setPos(QPointF(0,0) - QPointF(gameOverPix->boundingRect().width()/2,
                                                   gameOverPix->boundingRect().height()/2));

        QString string = "Score: " + QString::number(score)
            + " Best Score: " + QString::number(bestScore);
        scoreTextItem->setHtml(string);
    }
    QFont mFont("Times New Roman", 25, QFont::Bold);

    scoreTextItem->setFont(mFont);
    scoreTextItem->setDefaultTextColor(Qt::red);
    scoreTextItem->setZValue(100);
    addItem(scoreTextItem);

    scoreTextItem->setPos(QPoint(-175,-20));
}

void Scene::hideGameOverGraphics()
{    
    if(!firstPlay){
        if(gameOverPix){
            //removeItem(gameOverPix);
            delete gameOverPix;
            gameOverPix = nullptr;
        }
    }
    if(scoreTextItem){
        //removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
    firstPlay=false;
}

void Scene::setUpTexts(){
    QFont inGameFont("Times New Roman", 20, QFont::Bold);

    pointsTextItem = new QGraphicsTextItem();
    energyTextItem = new QGraphicsTextItem();

    pointsTextItem->setHtml(pointsString);
    energyTextItem->setHtml(energyString);

    pointsTextItem->setFont(inGameFont);
    energyTextItem->setFont(inGameFont);

    pointsTextItem->setDefaultTextColor(Qt::black);
    energyTextItem->setDefaultTextColor(Qt::black);

    pointsTextItem->setZValue(300);
    energyTextItem->setZValue(300);


    pointsTextItem->setPos(QPointF(100,0));
    energyTextItem->setPos(QPointF(-50,0));

    addItem(pointsTextItem);
    addItem(energyTextItem);
}

void Scene::cleanEnemies()
{
    QList<QGraphicsItem*> sceneItems = items();
    foreach(QGraphicsItem *item, sceneItems){
        EnemyItem * enemy = dynamic_cast<EnemyItem *>(item);
        if(enemy){
            //removeItem(enemy);
            delete enemy;
        }
    }
}

void Scene::cleanBosses()
{
    QList<QGraphicsItem*> sceneItems = items();
    foreach(QGraphicsItem *item, sceneItems){
        BossItem * boss = dynamic_cast<BossItem *>(item);
        if(boss){
            //removeItem(enemy);
            delete boss;
        }
    }
}

void Scene::cleanBullets()
{
    QList<QGraphicsItem*> sceneItems = items();
    foreach(QGraphicsItem *item, sceneItems){
        ShootItem * bullet = dynamic_cast<ShootItem *>(item);
        if(bullet){
            //removeItem(bullet);
            delete bullet;
        }
    }
}

void Scene::cleanLasers()
{
    QList<QGraphicsItem*> sceneItems = items();
    foreach(QGraphicsItem *item, sceneItems){
        LaserItem * laser = dynamic_cast<LaserItem *>(item);
        if(laser){
            //removeItem(bullet);
            delete laser;
        }
    }
}
