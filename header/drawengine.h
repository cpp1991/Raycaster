#ifndef DRAWENGINE_H
#define DRAWENGINE_H

#include <QWidget>
#include <QPaintEvent>
#include <QElapsedTimer>
#include <player.h>
#include <camera.h>
#include <map.h>
#include <ray.h>
#include <point.h>
#include <triple.h>


class DrawEngine : public QWidget
{
public:
    DrawEngine(std::vector<std::vector<int>> * _map, QWidget * parent = 0);
    Qt::Key& getSteppingKeyPressed (){return stepping_key_pressed;}
    void setSteppingKeyPressed(Qt::Key key) {this->stepping_key_pressed = key;}
    Qt::Key& getTurningKeyPressed (){return turning_key_pressed;}
    void setTurningKeyPressed(Qt::Key key) {this->turning_key_pressed = key;}
    void paintEvent(QPaintEvent *event);
    void updatePlayerValues(double dirX, double dirY, double posX, double posY);
    void calculateStepAndSideDist(double posX, double posY, int mapX, int mapY);
    Triple * performDDA(int mapX, int mapY);
    Point * calculateDrawingBorders(double perpWallDist);
    Point * calculateMovement(double posX, double posY, double dirX, double dirY, double moveSpeed);
    Point * calculateRotation(double dirX, double dirY, double rotSpeed);
    void calculatePerpWallDist(int mapX, int mapY, double posX, double posY, int side);

private:
    Player player;
    std::vector<std::vector<int>> * map;
    Qt::Key stepping_key_pressed;
    Qt::Key turning_key_pressed;
    QElapsedTimer elapsedTimer;
    Camera camera;
    Ray ray;

    //length of ray from current position to next x or y-side
    double sideDistY;
    double sideDistX;

    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    //length of ray from one x or y-side to next x or y-side
   double deltaDistX;
   double deltaDistY;
   double perpWallDist;

};

#endif // DRAWENGINE_H
