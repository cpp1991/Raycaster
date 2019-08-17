#include "drawengine.h"
#include <QPainter>
#include <QDebug>
#include "initialscene.h"
#include <cmath>


DrawEngine::DrawEngine(std::vector<std::vector<int>> * _map, QWidget * parent) : QWidget(parent), map{_map}, player{}, elapsedTimer{}, camera{}, ray{}
{
    this->resize(512, 384);
    // start elapsed timer
    this->elapsedTimer.start();
}

/**
 * @brief DrawEngine::paintEvent
 * @param event
 *
 * this function builds the game loop
 *
 */
void DrawEngine::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    double posX = this->player.getPosX();
    double posY = this->player.getPosY();
    double dirX = this->player.getDirX();
    double dirY = this->player.getDirY();

    // draw ceiling and floor
    painter.fillRect(0, 0, InitialScene::SCENE_WIDTH, InitialScene::SCENE_HEIGHT, QBrush(Qt::black));

    // for each pixel of the x axis
    for(auto x = 0; x < InitialScene::SCENE_WIDTH; ++x) {

         //calculate ray direction
         double cameraX = 2 * x / double(InitialScene::SCENE_WIDTH) - 1; //x-coordinate in camera space
         this->ray.ray_dir_x = dirX + this->camera.plane_x * cameraX;
         this->ray.ray_dir_y = dirY + this->camera.plane_y * cameraX;

         //which box of the map is player in
         int mapX = static_cast<int>(posX);
         int mapY = static_cast<int>(posY);

         this->deltaDistX = std::abs(1 / this->ray.ray_dir_x);
         this->deltaDistY = std::abs(1 / this->ray.ray_dir_y);

         calculateStepAndSideDist(posX, posY, mapX, mapY);

         //perform DDA (side: was a NS or a EW wall hit?)
         Triple * triple = performDDA(mapX, mapY);
         mapX = triple->x;
         mapY = triple->y;
         int side = triple->side;
         delete triple;

         //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
         if (side == 0) {
              this->perpWallDist = (mapX - posX + (1 - stepX) / 2) / this->ray.ray_dir_x;
         } else {
            this-> perpWallDist = (mapY - posY + (1 - stepY) / 2) / this->ray.ray_dir_y;
         }

         // choose color of wall
         switch(this->map->at(mapX).at(mapY)) {
         case 1 : painter.setPen(Qt::GlobalColor::yellow);
             break;
         case 2 : painter.setPen(Qt::GlobalColor::green);
             break;
         case 3 : painter.setPen(Qt::GlobalColor::blue);
             break;
         case 4 : painter.setPen(Qt::GlobalColor::red);
             break;
         case 5 : painter.setPen(Qt::GlobalColor::gray);
             break;
         case 6 : painter.setPen(Qt::GlobalColor::white);
             break;
         case 7 : painter.setPen(Qt::GlobalColor::cyan);
             break;
         default: painter.setPen(Qt::GlobalColor::darkMagenta);
         }

         //draw the pixels of the stripe as a vertical line and delete Point pointer afterwards
         Point * point = calculateDrawingBorders(perpWallDist);
         painter.drawLine(x, point->y1, x, point->y1 + (point->y2 - point->y1));
         delete point;

         // calculate delta time and reset timer for calculation of next cycle duration
         double delta_time = this->elapsedTimer.nsecsElapsed() / 1000000000.00;
         this->elapsedTimer.restart();

         //speed modifiers
         double moveSpeed = delta_time * Player::MOVING_SPEED;
         double rotSpeed = delta_time * Player::TURNING_SPEED;

         //move forward or backwards if no wall in front of you
         Point * posPoint = calculateMovement(posX, posY, dirX, dirY, moveSpeed);
         posX = posPoint->y1;
         posY = posPoint->y2;
         delete posPoint;

         //rotate to the right or the left
         Point * dirPoint = calculateRotation(dirX, dirY, rotSpeed);
         dirX = dirPoint->y1;
         dirY = dirPoint->y2;
         delete  dirPoint;

         // update player´s position and direction
         updatePlayerValues(dirX, dirY, posX, posY);
}
    // next frame
    repaint();
}

/*
 * update player´s position, direction and camera´s camera plane
 */
void DrawEngine::updatePlayerValues(double dirX, double dirY, double posX, double posY)
{
    this->player.setDirX(dirX);
    this->player.setDirY(dirY);
    this->player.setPosX(posX);
    this->player.setPosY(posY);
}

void DrawEngine::calculatePerpWallDist(int mapX, int mapY, double posX, double posY, int side)
{
    if (side == 0) {
         perpWallDist = (mapX - posX + (1 - stepX) / 2) / this->ray.ray_dir_x;
    } else {
        perpWallDist = (mapY - posY + (1 - stepY) / 2) / this->ray.ray_dir_y;
    }
}

void DrawEngine::calculateStepAndSideDist(double posX, double posY, int mapX, int mapY)
{
    if (this->ray.ray_dir_x < 0) {
      this->stepX = -1;
      this->sideDistX = (posX - mapX) * deltaDistX;
    } else {
      this->stepX = 1;
      this->sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (this->ray.ray_dir_y < 0) {
      this->stepY = -1;
      sideDistY = (posY - mapY) * deltaDistY;
    } else {
      this->stepY = 1;
      this->sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }
}

Point * DrawEngine::calculateRotation(double dirX, double dirY, double rotSpeed)
{
    double x = dirX;
    double y = dirY;

    if (this->turning_key_pressed == Qt::Key_Right) {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      x = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      y = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = this->camera.plane_x;
      this->camera.plane_x = this->camera.plane_x * cos(-rotSpeed) - this->camera.plane_y * sin(-rotSpeed);
      this->camera.plane_y = oldPlaneX * sin(-rotSpeed) + this->camera.plane_y * cos(-rotSpeed);
    } else if (this->turning_key_pressed == Qt::Key_Left) {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      x = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      y = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = this->camera.plane_x;
      this->camera.plane_x = this->camera.plane_x * cos(rotSpeed) - this->camera.plane_y * sin(rotSpeed);
      this->camera.plane_y = oldPlaneX * sin(rotSpeed) + this->camera.plane_y * cos(rotSpeed);
    }

    return new Point{x, y};
}

Point* DrawEngine::calculateDrawingBorders(double perpWallDist)
{
    //Calculate height of line to draw on screen
    int lineHeight = static_cast<int>(InitialScene::SCENE_HEIGHT / perpWallDist);

    //calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + InitialScene::SCENE_HEIGHT / 2;
    if(drawStart < 0)drawStart = 0;
    int drawEnd = lineHeight / 2 + InitialScene::SCENE_HEIGHT / 2;
    if(drawEnd >= InitialScene::SCENE_HEIGHT)drawEnd = InitialScene::SCENE_HEIGHT - 1;

    return new Point{static_cast<double>(drawStart), static_cast<double>(drawEnd)};
}

Point * DrawEngine::calculateMovement(double posX, double posY, double dirX, double dirY, double moveSpeed)
{
     double x = posX;
     double y = posY;

    if (this->stepping_key_pressed== Qt::Key_Up) {
      if(this->map->at(static_cast<int>(posX + dirX * moveSpeed)).at(static_cast<int>(posY)) == false) x += dirX * moveSpeed;
      if(this->map->at(static_cast<int>(posX)).at(static_cast<int>(posY + dirY * moveSpeed)) == false) y += dirY * moveSpeed;
    } else if (this->stepping_key_pressed == Qt::Key_Down) {
      if(this->map->at(static_cast<int>(posX - dirX * moveSpeed)).at(static_cast<int>(posY)) == false) x -= dirX * moveSpeed;
      if(this->map->at(static_cast<int>(posX)).at(static_cast<int>(posY - dirY * moveSpeed)) == false) y -= dirY * moveSpeed;
    }

    return new Point{x, y};
}

Triple * DrawEngine::performDDA(int mapX, int mapY)
{
    int side;
    int x = mapX;
    int y = mapY;
    bool wallHit = false;
    while (!wallHit) {
      //jump to next map square, OR in x-direction, OR in y-direction
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        x += stepX;
        side = 0;
      } else {
        sideDistY += deltaDistY;
        y += stepY;
        side = 1;
      }
      //Check if ray has hit a wall
      if (this->map->at(x).at(y) > 0) {
          wallHit = true;
      }
    }

    return new Triple{x, y, side};
}

