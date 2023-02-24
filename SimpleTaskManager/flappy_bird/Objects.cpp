#include"Objects.h"
Object::Object(QString image_route,int init_x,int init_y,double init_a,double init_v,int x_size,int y_size,int x_collision,int y_collision,int colbias_x,int colbias_y)
{
    image=QPixmap(image_route);
    x=init_x;
    y=init_y;
    a=init_a;
    v=init_v;
    bias_x=colbias_x;
    bias_y=colbias_y;
    collision_x=x_collision;
    collision_y=y_collision;
    size_x=x_size;
    size_y=y_size;
}
void Object::action(double cos,double sin)
{
    v+=a;
    x+=cos*v;
    y+=sin*v;
}
bool dot_in_range(int dotx,int doty,int range_basex,int range_basey,int collison_x,int collision_y)
{
    int distance_x=dotx-range_basex;
    int distance_y=doty-range_basey;
    if(distance_x<=collison_x&&distance_x>=0&&distance_y<=collision_y&&distance_y>=0)
        return true;
    else
        return false;
}
bool Object::check_collsion(Object* other)
{
    int biased_x=x+bias_x;
    int biased_y=y+bias_y;
    int other_biased_x=other->x+other->bias_x;
    int other_biased_y=other->y+other->bias_y;
    return dot_in_range(other_biased_x,other_biased_y,biased_x,biased_y,collision_x,collision_y)||
           dot_in_range(other_biased_x,other_biased_y+other->collision_y,biased_x,biased_y,collision_x,collision_y)||
           dot_in_range(other_biased_x+other->collision_x,other_biased_y,biased_x,biased_y,collision_x,collision_y)||
           dot_in_range(other_biased_x+other->collision_x,other_biased_y+other->collision_y,biased_x,biased_y,collision_x,collision_y);
}
