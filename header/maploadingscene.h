#ifndef MAPLOADINGSCENE_H
#define MAPLOADINGSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>


class maploadingscene : public QGraphicsView
{
public:
     maploadingscene();

private:
    QGraphicsScene scene;
};

#endif // MAPLOADINGSCENE_H
