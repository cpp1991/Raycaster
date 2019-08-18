#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
    Player();

    // constants (moving: squares per second, turning: radians per second)
    static const int MOVING_SPEED = 5;
    static const int TURNING_SPEED = 3;

    // getters
    double getPosX() {return pos_x;}
    double getPosY() {return pos_y;}
    double getDirX() {return dir_x;}
    double getDirY() {return dir_y;}

    // setters
    void setPosX(double x) {this->pos_x = x;}
    void setPosY(double y) {this->pos_y = y;}
    void setDirX(double x) {this->dir_x = x;}
    void setDirY(double y) {this->dir_y = y;}

private:
    double pos_x;
    double pos_y;
    double dir_x;
    double dir_y;
};

#endif // PLAYER_H
