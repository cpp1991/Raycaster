#ifndef INITIALSCENE_H
#define INITIALSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <memory>
#include <QObject>
#include <QElapsedTimer>
#include <drawengine.h>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <drawobject.h>


/*
 *  this class handles the rendering and holds all objects to be displayed
 */
class InitialScene : public QGraphicsScene, public QGraphicsRectItem
{
     Q_OBJECT

public:
    InitialScene(std::vector<std::vector<int>> * _map);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void setFocus() {QGraphicsRectItem::setFocus();}
    static const int SCENE_WIDTH = 512;
    static const int SCENE_HEIGHT = 384;

private:
    DrawEngine draw_engine;
};

#endif // INITIALSCENE_H
