#ifndef OBJECTS_H
#define OBJECTS_H

#endif // OBJECTS_H
#include<QPainter>
class Object
{
public:
    Object(QString image_route,int init_x,int init_y,double init_a,double init_v,int x_size,int y_size,int x_collision,int y_collision,int colbias_x,int colbias_y);
    QPixmap image;
    int x;
    int y;
    int size_x;
    int size_y;
    int bias_x;
    int bias_y;
    int collision_x;
    int collision_y;
    double a;
    double v;
    bool check_collsion(Object* other);
    void action(double cos,double sin);
};
