#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QKeyEvent>
#include <QGraphicsRectItem>

/*
 * this class handles key presses as well as the game loop
 */
class GameManager : public QGraphicsRectItem
{
public:
    GameManager();
    void keyPressEvent(QKeyEvent * event);

};

#endif // GAMEMANAGER_H
