#include "initialscene.h"
#include <QTimer>
#include <gamemanager.h>
#include <QThread>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>


InitialScene::InitialScene(std::vector<std::vector<int>> * _map) : draw_engine{_map}
{
    // set scene size
    this->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);

    // add draw engine to scene
    this->addWidget(&this->draw_engine);
}

/*
 * handle all key presses
*/
void InitialScene::keyPressEvent(QKeyEvent * event) {
    if(event->key() == Qt::Key_Left) {
        this->draw_engine.setTurningKeyPressed(Qt::Key_Left);
    } else if (event->key() == Qt::Key_Right) {
        this->draw_engine.setTurningKeyPressed(Qt::Key_Right);
    } else if (event->key() == Qt::Key_Up) {
        this->draw_engine.setSteppingKeyPressed(Qt::Key_Up);
    } else if (event->key() == Qt::Key_Down) {
        this->draw_engine.setSteppingKeyPressed(Qt::Key_Down);
    }
}

/*
 * set keyPressed to an unused key if pressed key gets released
*/
void InitialScene::keyReleaseEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Left) {
        this->draw_engine.setTurningKeyPressed(Qt::Key_0);
    } else if (event->key() == Qt::Key_Right) {
        this->draw_engine.setTurningKeyPressed(Qt::Key_0);
    } else if (event->key() == Qt::Key_Up) {
        this->draw_engine.setSteppingKeyPressed(Qt::Key_0);
    } else if (event->key() == Qt::Key_Down) {
        this->draw_engine.setSteppingKeyPressed(Qt::Key_0);
    }
}
