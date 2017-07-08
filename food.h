#ifndef FOOD_H
#define FOOD_H
#include <QString>
#include <QRect>

class Food
{
public:
    Food(int x1_,int y1_,int x2_,int y2_,QString name_);
    Food();
public:
    int x1_;
    int y1_;
    int x2_;
    int y2_;
    QRect rect_;
    QString name_;

};

#endif // FOOD_H
