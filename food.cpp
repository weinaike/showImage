#include "food.h"


Food::Food()
{

}


Food::Food(int x1, int y1, int x2, int y2, QString name)
{
    x1_=x1;
    x2_=x2;
    y1_=y1;
    y2_=y2;
    rect_ = QRect(x1,y1,x2-x1,y2-y1);
    name_ = name;
}
