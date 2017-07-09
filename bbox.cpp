#include "bbox.h"
#include <QDomDocument>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>

bbox::bbox(){

}

bbox::bbox(QImage src, QString annoFileName){
    src_image_ = src.copy(src.rect());
    show_image_ = src_image_.copy(src_image_.rect());
    getObjects(annoFileName);
    imageWidth_ = src.width();
    imageHeight_ = src.height();
}

void bbox::getObjects(QString annoFileName){
    QString extend = annoFileName.split('.').last().trimmed();

    if (extend == "xml")
    {
        QFile xmlfile(annoFileName);
    //    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    //    QTextCodec::setCodecForLocale(codec);
    //    QTextCodec::setCodecForCStrings(codec);
    //    QTextCodec::setCodecForTr(codec);
        if (! xmlfile.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            qDebug()<<annoFileName<<endl;
            qDebug()<<"error! can't find the file"<<endl;
    //        QMessageBox::warning(NULL,"open","open file failed");
            return;
        }
        QDomDocument doc;
        if (! doc.setContent( &xmlfile ) )
        {
            qDebug()<<"close"<<endl;
            xmlfile.close();
            return;
        }
        QDomElement root = doc.documentElement();
        QDomNode objectNode=root.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling();

        while (! objectNode.isNull() )
        {
            QDomNode node = objectNode.firstChild();
            QString name = node.toElement().text();
            node = node.nextSibling();
            QString pose = node.toElement().text();
            node = node.nextSibling();
            QString truncated = node.toElement().text();
            node = node.nextSibling();
            int difficult = node.toElement().text().toInt();
            node = node.nextSibling().firstChild();
            int x1 = node.toElement().text().toInt();
            node = node.nextSibling();
            int y1 = node.toElement().text().toInt();
            node = node.nextSibling();
            int x2 = node.toElement().text().toInt();
            node = node.nextSibling();
            int y2 = node.toElement().text().toInt();
            Food objFood(x1,y1,x2,y2,name);
            foodList_.append(objFood);
        }

    }
    else if( extend == "txt")
    {
        QFile annoFile(annoFileName);
        if(!annoFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<< annoFileName << " Open failed." << endl;
//            QMessageBox::warning(NULL,"open","open file failed");
            return ;
        }
        QTextStream txtInput(&annoFile);
        txtInput.setCodec("UTF-8");
        QString lineStr;

        int num = txtInput.readLine().trimmed().toInt();
        if (num > 0)
        {
            while( ! txtInput.atEnd() )
            {

                lineStr = txtInput.readLine();
                QList<QString> lineDate= lineStr.split(' ');
                int x1 = lineDate.at(0).trimmed().toInt();
                int y1 = lineDate.at(1).trimmed().toInt();
                int x2 = lineDate.at(2).trimmed().toInt();
                int y2 = lineDate.at(3).trimmed().toInt();
                QString classname = lineDate.at(4).trimmed();
                Food objFood(x1,y1,x2,y2,classname);
                foodList_.append(objFood);
            }
        }
    }


}

void bbox::addObject(Food newFood){
    foodList_.append(newFood);
}

void bbox::deleteObject(int iter)
{
    foodList_.removeAt( iter );

}

void bbox::modObject(int iter, Food newFood){
    foodList_.replace( iter, newFood );
}


void bbox::paintObjects( bool IsShowBBOX, bool IsShowName )
{
    show_image_ = src_image_.copy(src_image_.rect());

    if ( IsShowBBOX )
    {
        QPainter painter( &show_image_ );
        for (int iter = 0; iter < foodList_.size(); iter++)
        {
            Food food = foodList_.at(iter);
            int x1 = food.x1_;
            int y1 = food.y1_;
            int x2 = food.x2_;
            int y2 = food.y2_;

            QPen pen(QColor(255,y1%255,y2%255), 3);
            painter.setPen(pen);
            painter.drawRect(food.rect_);

            if(IsShowName)
            {
                QFont font;
                font.setFamily("Microsoft YaHei");
                font.setPointSize(20);
                font.setItalic(true);
                painter.setFont(font);
                painter.drawText(QRect(x1,y1,150,30),Qt::AlignLeft,food.name_);
            }
        }
    }
}

QImage bbox::printImage()
{
    return show_image_;
}

QImage bbox::getSubImage(int iter){
    Food food = foodList_.at(iter);
    return src_image_.copy(food.rect_);
}

void bbox::getList(QList<Food> &list)
{
    Food food;
    foreach (food, foodList_) {
        list.append(food);
    }

}

int bbox::width()
{
    return imageWidth_;

}
int bbox::height()
{
    return imageHeight_;
}

int bbox::size()
{
    return foodList_.size();
}


