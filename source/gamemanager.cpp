#include "gamemanager.h"
#include <QKeyEvent>
#include <QDebug>

GameManager::GameManager()
{
}

void GameManager::keyPressEvent(QKeyEvent * event) {
    if(event->key() == Qt::Key_Left) {
        qDebug() << "left pressed";
    } else if (event->key() == Qt::Key_Right) {
        qDebug() << "right pressed";
    } else if (event->key() == Qt::Key_Up) {
        qDebug() << "up pressed";
    } else if (event->key() == Qt::Key_Down) {
        qDebug() << "down pressed";
    }
};



