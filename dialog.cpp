#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <iostream>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QList>
#include <QPainter>
#include <QDomDocument>

class Food
{
public:
    Food(int x1_,int y1_,int x2_,int y2_,QString name_);
public:
    int x1;
    int y1;
    int x2;
    int y2;
    QString name;

};
Food::Food(int x1_, int y1_, int x2_, int y2_, QString name_)
{
    x1=x1_;
    x2=x2_;
    y1=y1_;
    y2=y2_;
    name = name_;
}


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    start_num = 0;
    start_num_2 = 1;
    all_num = 0;
    all_num_2 = 0;
    mydir = new QDir();
    fileInfo = new QList<QFileInfo>();
}

Dialog::~Dialog()
{
    delete ui;
    if (mydir->exists())
        delete mydir;
    if (!fileInfo->empty())
        fileInfo->clear();
}


void Dialog::showImage()
{
    int num_max = 6;
    int num = start_num;
    if (all_num < 6)
    {
        num_max = all_num;
//        num = 0;
    }
//    else
//    {
//        if ((all_num-start_num)<6)
//            num = all_num - num_max;
//        if (start_num < 0)
//            num = 0;
//    }

    int Image_Width=ui->label_1->width();
    int Image_Height=ui->label_1->height();
    for (int i = 0 ; i < num_max; i++)
    {
        QFileInfo qfile = fileInfo->at(num);
        QString filename = qfile.absoluteFilePath();
        QImage src =  QImage(filename);
        QString imgName = qfile.fileName();
        QImage image = src.scaled(Image_Width,Image_Height,Qt::KeepAspectRatio);
        switch (i) {
        case 0:
            ui->label_1->setPixmap(QPixmap::fromImage(image));
            ui->name1->setText(imgName);
            break;
        case 1:
            ui->label_2->setPixmap(QPixmap::fromImage(image));
            ui->name2->setText(imgName);
            break;
        case 2:
            ui->label_3->setPixmap(QPixmap::fromImage(image));
            ui->name3->setText(imgName);
            break;
        case 3:
            ui->label_4->setPixmap(QPixmap::fromImage(image));
            ui->name4->setText(imgName);
            break;
        case 4:
            ui->label_5->setPixmap(QPixmap::fromImage(image));
            ui->name5->setText(imgName);
            break;
        case 5:
            ui->label_6->setPixmap(QPixmap::fromImage(image));
            ui->name6->setText(imgName);
            break;
        default:
            break;
        }
        num++;
    }


}


void Dialog::on_load_clicked()
{
    imageDir = ui->lineEdit_DIR->text();
    start_num = ui->lineEdit_NUM->text().toInt();
    mydir = new QDir(imageDir);
    QStringList filter;
    filter<<"*.jpg";
    mydir->setNameFilters(filter);
    fileInfo = new  QList<QFileInfo>(mydir->entryInfoList(filter));
    all_num = fileInfo->count();
    ui->lineEdit_ALL->setText(QString::number(all_num, 10));
    showImage();
}

void Dialog::on_upper_clicked()
{
    start_num = start_num - 6;
    if(start_num < 0)
        start_num = 0;
    showImage();
    ui->lineEdit_NUM->setText(QString::number(start_num, 10));
}

void Dialog::on_next_clicked()
{
    start_num += 6;
    if (start_num >= all_num - 6 )
        start_num = all_num - 6;
    showImage();
    ui->lineEdit_NUM->setText(QString::number(start_num, 10));
}



void Dialog::on_lineEdit_NUM_editingFinished()
{
    start_num = ui->lineEdit_NUM->text().toInt();
}


void Dialog::delete_object(int startNum)
{
    QFileInfo qfile = fileInfo->at(startNum);
    QString filename = qfile.absoluteFilePath();
    QImage src =  QImage(filename);
    int width = src.width();
    int height = src.height();
//    cout << width << "   "<< height <<endl;
    QString imgName = qfile.fileName();
    QString name = imgName.split('.').at(1).trimmed();
    QString annoFileName = AnnoDir + "/" + name + ".txt";
    QFile annoFile(annoFileName);
    if(!annoFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //cout <<annoFileName.toStdString()<< " Open failed." << endl;
        QMessageBox::warning(NULL,"open","open file failed");
        return ;
    }
    QTextStream txtInput(&annoFile);
    txtInput.setCodec("UTF-8");
    QString lineStr;
    QList<QString> classList;
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
//            cout<<x2 - x1 << "                  " <<y2 - y1 <<endl;
            if ((x2 - x1 == width) && ((y2 - y1 == height)))
            {
//                cout<<annoFileName.toStdString()<< " delete one object " <<endl;
                QMessageBox::information(NULL,"delete","delete success");
                num--;
            }
            else
            {
                classList.append(lineStr);
            }
        }
    }
    annoFile.close();
    if(!annoFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL,"open","open file failed");
        return ;
    }
    QTextStream txtOutput(&annoFile);
    txtOutput.setCodec("UTF-8");
    txtOutput <<num<<endl;
    for (int i = 0; i < num ; i++)
    {
        txtOutput<<classList.at(i)<<endl;
    }
    annoFile.close();

}

void Dialog::on_lineEdit_DIR_Anno_editingFinished()
{
    AnnoDir = ui->lineEdit_DIR_Anno->text();
}

void Dialog::on_del_1_clicked()
{
    delete_object(start_num);
}


void Dialog::on_del_2_clicked()
{
    delete_object(start_num+1);
}



void Dialog::on_del_3_clicked()
{
    delete_object(start_num+2);
}

void Dialog::on_del_4_clicked()
{
    delete_object(start_num+3);
}

void Dialog::on_del_5_clicked()
{
    delete_object(start_num+4);
}

void Dialog::on_del_6_clicked()
{
    delete_object(start_num+5);
}

void Dialog::on_load_2_clicked()
{
    imageDir_2 = ui->lineEdit_DIR_2->text();
    AnnoDir_2 = ui->lineEdit_DIR_Anno_2->text();
    start_num_2 = ui->lineEdit_NUM_2->text().toInt();
    mydir = new QDir(imageDir_2);
    QStringList filter;
    filter<<"*.jpg";
    mydir->setNameFilters(filter);
    fileInfo = new  QList<QFileInfo>(mydir->entryInfoList(filter));
    all_num_2 = fileInfo->count();
    ui->lineEdit_ALL_2->setText(QString::number(all_num_2, 10));
    showImage_2();
}

void Dialog::showImage_2()
{

    int num = start_num_2;

    int Image_Width=ui->label_part->width();
    int Image_Height=ui->label_part->height();

    QFileInfo qfile = fileInfo->at(num);
    QString filename = qfile.absoluteFilePath();
    QImage src =  QImage(filename);
    QString imgName = qfile.fileName();
    QImage image = src.scaled(Image_Width,Image_Height,Qt::KeepAspectRatio);
     ui->label_part->setPixmap(QPixmap::fromImage(image));
     ui->name1_2->setText(imgName);

     showFullImage(num);

}

void Dialog::on_upper_2_clicked()
{
    start_num_2--;
    if(start_num_2 < 0)
        start_num_2 = 0;
    showImage_2();
    ui->lineEdit_NUM_2->setText(QString::number(start_num_2, 10));
}

void Dialog::on_next_2_clicked()
{
    start_num_2++;
    if(start_num_2 > all_num_2)
        start_num_2 = all_num_2;
    showImage_2();
    ui->lineEdit_NUM_2->setText(QString::number(start_num_2, 10));

}

void Dialog::showFullImage(int startNum)
{
    AnnoDir_2 = ui->lineEdit_DIR_Anno_2->text();

    QFileInfo qfile = fileInfo->at(startNum);
    QString imgName = qfile.fileName();
    QString name = imgName.split('.').at(1).trimmed();
    QString annoFileName = AnnoDir_2 + "/" + name + ".txt";
    QString imageFileName = AnnoDir_2 + "/" + name + ".jpg";
    QFile annoFile(annoFileName);
    if(!annoFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << AnnoDir_2.toStdString()<<endl;
        cout <<annoFileName.toStdString()<< " Open failed." << endl;
        QMessageBox::warning(NULL,"open","open file failed");
        return ;
    }
    QTextStream txtInput(&annoFile);
    txtInput.setCodec("UTF-8");
    QString lineStr;
    QList<Food> classList;
    int num = txtInput.readLine().trimmed().toInt();
    QImage img(imageFileName);
    QPainter painter(&img);

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
            classList.append(objFood);

            if(ui->checkBox_rect->isChecked())
            {
                QPen pen(QColor(255,y1%255,y2%255), 3);
                painter.setPen(pen);
                painter.drawRect(x1,y1,x2-x1,y2-y1);

                if(ui->checkBox_label->isChecked())
                {
                    QFont font;
                    font.setFamily("Microsoft YaHei");
                    font.setPointSize(20);
                    font.setItalic(true);
                    painter.setFont(font);
                    painter.drawText(QRect(x1,y1,150,30),Qt::AlignLeft,classname);
                }
            }

        }
        painter.end();
    }

    int Image_Width=ui->label_fullImage->width();
    int Image_Height=ui->label_fullImage->height();
    QImage fullImage = img.scaled(Image_Width,Image_Height,
                                                   Qt::KeepAspectRatio);
    ui->label_fullImage->setPixmap(QPixmap::fromImage(fullImage));

}

void Dialog::on_lineEdit_DIR_Anno_2_editingFinished()
{
    AnnoDir_2 = ui->lineEdit_DIR_Anno_2->text();
}

void Dialog::on_load_3_clicked()
{
    imageDir_3 = ui->lineEdit_DIR_3->text();
    AnnoDir_3 = ui->lineEdit_DIR_Anno_3->text();
    start_num_3 = ui->lineEdit_NUM_3->text().toInt();
    mydir = new QDir(imageDir_3);
    QStringList filter;
    filter<<"*.JPEG";
    mydir->setNameFilters(filter);
    fileInfo = new  QList<QFileInfo>(mydir->entryInfoList(filter));
    all_num_3 = fileInfo->count();
    ui->lineEdit_ALL_3->setText(QString::number(all_num_3, 10));
    showImage_3();


}


void Dialog::showImage_3()
{

    int num = start_num_3;

    int Image_Width=ui->label_fullImage_3->width();
    int Image_Height=ui->label_fullImage_3->height();

    QFileInfo qfile = fileInfo->at(num);
    QString filename = qfile.absoluteFilePath();
    QImage src =  QImage(filename);
    QString imgName = qfile.fileName();

    QString name = imgName.split('.').at(0).trimmed();
    QString annoFileName = AnnoDir_3 + "/" + name + ".xml";
    QFile xmlfile(annoFileName);

//    QTextCodec *codec = QTextCodec::codecForName("utf-8");
//    QTextCodec::setCodecForLocale(codec);
//    QTextCodec::setCodecForCStrings(codec);
//    QTextCodec::setCodecForTr(codec);

    if (! xmlfile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        cout<<annoFileName.toStdString()<<endl;
        cout<<"error! can't find the file"<<endl;
//        QMessageBox::warning(NULL,"open","open file failed");
//        QImage image = src.scaled(Image_Width,Image_Height,Qt::KeepAspectRatio);
         ui->label_fullImage_3->setPixmap(QPixmap::fromImage(src));
         ui->name1_3->setText(imgName);
        return;
    }
    QDomDocument doc;
    if (! doc.setContent( &xmlfile ) )
    {
        cout<<"close"<<endl;
        xmlfile.close();
        return;
    }
    QDomElement root = doc.documentElement();
    QDomNode objectNode=root.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling();

    QList<Food> classList;
    QPainter painter(&src);

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
//        if (difficult !=0)
//        {
//            cout<<difficult<<endl;
//            QMessageBox::warning(NULL,"difficult","difficult");
//        }
        node = node.nextSibling().firstChild();
        int x1 = node.toElement().text().toInt();
        node = node.nextSibling();
        int y1 = node.toElement().text().toInt();
        node = node.nextSibling();
        int x2 = node.toElement().text().toInt();
        node = node.nextSibling();
        int y2 = node.toElement().text().toInt();
        Food objFood(x1,y1,x2,y2,name);
        classList.append(objFood);

        if(ui->checkBox_rect_3->isChecked())
        {
            QPen pen(QColor(255,y1%255,y2%255), 3);
            painter.setPen(pen);
            painter.drawRect(x1,y1,x2-x1,y2-y1);

            if(ui->checkBox_label_3->isChecked())
            {
                QFont font;
                font.setFamily("Microsoft YaHei");
                font.setPointSize(20);
                font.setItalic(true);
                painter.setFont(font);
                painter.drawText(QRect(x1,y1,150,30),Qt::AlignLeft,name);
            }
        }

        objectNode = objectNode.nextSibling();
    }
    painter.end();

//    QImage image = src.scaled(Image_Width,Image_Height,Qt::KeepAspectRatio);
     ui->label_fullImage_3->setPixmap(QPixmap::fromImage(src));
     ui->name1_3->setText(imgName);

}

void Dialog::on_upper_3_clicked()
{
    start_num_3--;
    if(start_num_3 < 0)
        start_num_3 = 0;
    showImage_3();
    ui->lineEdit_NUM_3->setText(QString::number(start_num_3, 10));

}

void Dialog::on_next_3_clicked()
{
    start_num_3++;
    if(start_num_3 > all_num_3)
        start_num_3 = all_num_3;
    showImage_3();
    ui->lineEdit_NUM_3->setText(QString::number(start_num_3, 10));
}
