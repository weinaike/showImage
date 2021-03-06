#ifndef BBOX_H
#define BBOX_H
#include <food.h>
#include <QString>
#include <QImage>
#include <QPainter>
class bbox
{
public:
    bbox();
    ~bbox();
    bbox(const QImage & src, const QString & annoFileName );
    void getObjects( QString annoFileName );
    QImage paintObjects(bool IsShowBBOX, bool IsShowName);
    void deleteObject( int iter );
    void addObject ( Food newFood);
    void modObject ( int iter, Food newFood);

    QImage getSubImage(int iter);
    void getList(QList<Food> &list);
    int width();
    int height();
    int size();
private:
    QList<Food> foodList_;
    QList<QString> words;
    QImage src_image_;
//    QImage show_image_;
    int imageWidth_;
    int imageHeight_;
};

#endif // BBOX_H
