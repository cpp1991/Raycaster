#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H


class DrawObject
{
public:
    DrawObject(int x_ , int draw_start_, int draw_end_, bool isWall);
    int x;
    int draw_start;
    int draw_end;
    bool isWall;

};

#endif // DRAWOBJECT_H
