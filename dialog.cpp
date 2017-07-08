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
#include <QDebug>
#include <QTableWidget>

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
    mybbox = new bbox();
    ind = -1;
    ui->pushButton->setEnabled(false);
    QFile wordsfile("D:\\work\\showImage\\foodlist.txt");
    if (wordsfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream txtInput(&wordsfile);
        txtInput.setCodec("UTF-8");
        int i = 0; int j = 0;
        while( ! txtInput.atEnd() ){
            QString lineStr = txtInput.readLine().trimmed();
            words_.append(lineStr);
            QTableWidgetItem *it = new QTableWidgetItem(lineStr);
            ui->tableWidget->setItem(i,j,it);
            i++;
            if (i == ui->tableWidget->rowCount() )
            {
                j++;
                i=0;
            }

        }
    }
    else
    {
        qDebug()<<"words file is not exist"<<endl;
    }

}

Dialog::~Dialog()
{
    delete ui;
    if (mydir->exists())
        delete mydir;
    if (!fileInfo->empty())
        fileInfo->clear();
    delete mybbox;
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
    filter<<"*.JPEG"<<"*.jpg"<<"*.png";
    mydir->setNameFilters(filter);
    fileInfo = new  QList<QFileInfo>(mydir->entryInfoList(filter));
    all_num_3 = fileInfo->count();
    ui->lineEdit_ALL_3->setText(QString::number(all_num_3, 10));

    QFileInfo qfile = fileInfo->at(start_num_3);
    QString filename = qfile.absoluteFilePath();
    QImage src = QImage(filename);
    QString imgName = qfile.fileName();
    QString name = imgName.split('.').at(0).trimmed();

    QString annoFileName;
    if (ui->radioButton_xml->isChecked())
        annoFileName = AnnoDir_3 + "/" + name + ".xml";
    else
        annoFileName = AnnoDir_3 + "/" + name + ".txt";
    mybbox = new bbox(src, annoFileName);
    ui->name1_3->setText(imgName);
    ind = -1;
    ui->pushButton->setEnabled(false);

    showImage_3();
}

void Dialog::showImage_3()
{
    ui->listWidget->clear();

    int Image_Width=ui->label_fullImage_3->width();
    int Image_Height=ui->label_fullImage_3->height();

    QList<Food> list;
    mybbox->getList(list);
    for(int iter = 0; iter<list.size(); iter++)
    {
        Food food = list.at( iter );
        QListWidgetItem *it1 = new QListWidgetItem(QString::number(food.x1_)+" "
                                               +QString::number(food.y1_)+" "
                                               +QString::number(food.x2_)+" "
                                               +QString::number(food.y2_)+" "
                                               +food.name_) ;
        ui->listWidget->insertItem(iter,it1);
    }
    mybbox->paintObjects(ui->checkBox_rect_3->isChecked(),ui->checkBox_label_3->isChecked());
    QImage image = mybbox->printImage().scaled(Image_Width,Image_Height);
    ui->label_fullImage_3->setPixmap(QPixmap::fromImage( image ));

}

void Dialog::on_upper_3_clicked()
{
    start_num_3--;
    if(start_num_3 < 0)
        start_num_3 = 0;

    QFileInfo qfile = fileInfo->at(start_num_3);
    QString filename = qfile.absoluteFilePath();
    QImage src = QImage(filename);
    QString imgName = qfile.fileName();
    QString name = imgName.split('.').at(0).trimmed();

    QString annoFileName;
    if (ui->radioButton_xml->isChecked())
        annoFileName = AnnoDir_3 + "/" + name + ".xml";
    else
        annoFileName = AnnoDir_3 + "/" + name + ".txt";
    mybbox = new bbox(src, annoFileName);
    ui->name1_3->setText(imgName);
    ind = -1;
    ui->pushButton->setEnabled(false);


    showImage_3();
    ui->lineEdit_NUM_3->setText(QString::number(start_num_3, 10));

}

void Dialog::on_next_3_clicked()
{
    start_num_3++;
    if(start_num_3 > all_num_3)
        start_num_3 = all_num_3;

    QFileInfo qfile = fileInfo->at(start_num_3);
    QString filename = qfile.absoluteFilePath();
    QImage src = QImage(filename);
    QString imgName = qfile.fileName();
    QString name = imgName.split('.').at(0).trimmed();

    QString annoFileName;
    if (ui->radioButton_xml->isChecked())
        annoFileName = AnnoDir_3 + "/" + name + ".xml";
    else
        annoFileName = AnnoDir_3 + "/" + name + ".txt";
    mybbox = new bbox(src, annoFileName);
    ui->name1_3->setText(imgName);
    ind = -1;
    ui->pushButton->setEnabled(false);

    showImage_3();
    ui->lineEdit_NUM_3->setText(QString::number(start_num_3, 10));
}

void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{


}

void Dialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

}

void Dialog::on_listWidget_clicked(const QModelIndex &index)
{
    QList<Food> list;
    mybbox->getList(list);
    ind = index.row();
    if (ind >=0)
        ui->pushButton->setEnabled(true);
    else
        ui->pushButton->setEnabled(false);
    QImage part = mybbox->getSubImage(ind)
            .scaled(ui->label_part_3->width(),ui->label_part_3->height(),Qt::KeepAspectRatio);
    ui->label_part_3->setPixmap(QPixmap::fromImage( part ));

}

void Dialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QList<Food> list;
    mybbox->getList(list);
    ind = index.row();

    if (ind >=0)
        ui->pushButton->setEnabled(true);
    else
        ui->pushButton->setEnabled(false);

    Food food = list.at(ind);
    QString classname = food.name_;
    if (classname.split('_').last() == "hard")
        food.name_ = classname.split('_')[0];
    else
        food.name_ = food.name_+"_hard";
    mybbox->modObject(ind,food);

    QImage part = mybbox->getSubImage(ind)
            .scaled(ui->label_part_3->width(),ui->label_part_3->height(),Qt::KeepAspectRatio);
    ui->label_part_3->setPixmap(QPixmap::fromImage( part ));

    showImage_3();

}

void Dialog::on_pushButton_clicked()
{
    if (ind >= 0 )
    {
        if (QMessageBox::question(this,"删除","确定删除?",QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::Yes) == QMessageBox::Yes)
        {
            mybbox->deleteObject(ind);
            ui->label_part_3->clear();
            ui->pushButton->setEnabled(false);
            ind = -1;
        }
    }

    showImage_3();
}

void Dialog::on_pushButton_saveLabel_clicked()
{

    QFileInfo qfile = fileInfo->at(start_num_3);
    QString imgName = qfile.fileName();
    QString name = imgName.split('.').at(0).trimmed();

    QString annoFileName;
    if (ui->radioButton_xml->isChecked())
    {
        annoFileName = AnnoDir_3 + "/" + name + ".xml";
    }
    else if(ui->radioButton_txt->isChecked())
    {
        annoFileName = AnnoDir_3 + "/" + name + ".txt";
        QFile savefile(annoFileName);
        if(!savefile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(NULL,"open","open file failed");
            return ;
        }
        QTextStream txtOutput(&savefile);
        txtOutput.setCodec("UTF-8");
        QList <Food > list;
        mybbox->getList(list);
        txtOutput<< list.size() <<endl;
        for (int i = 0; i < list.size() ; i++)
        {
            Food food = list.at(i);
            txtOutput<<food.x1_<<" "<<food.y1_<<" "<<food.x2_<<" "<<food.y2_<<" "<<food.name_<<endl;
        }
        savefile.close();

    }

}

void Dialog::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    int column = index.column();
    word_taked_ = words_.at(column*ui->tableWidget->rowCount()+row);
    qDebug()<<word_taked_<<endl;
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    if ( event->button()== Qt::LeftButton )
    {
        if ( (ui->label_fullImage_3->x() <= event->x())
             && ( event->x() < ui->label_fullImage_3->x()+ ui->label_fullImage_3->width())
             && (ui->label_fullImage_3->y() <= event->y())
             && (event->y()  < ui->label_fullImage_3->y()+ ui->label_fullImage_3->height()) )
        {
            start_ = QPoint(event->x(),event->y());
            qDebug()<<"start:"<<start_<<endl;
        }
    }

}

void Dialog::mouseReleaseEvent(QMouseEvent *event){
    int x,y;
    if ( (event->x() > start_.x()) && (event->y() > start_.y()) && (start_.x() != -1 ))
    {
        if ( event->x() > ui->label_fullImage_3->x()+ ui->label_fullImage_3->width() )
            x = ui->label_fullImage_3->x()+ ui->label_fullImage_3->width()-1;
        else
            x = event->x();
        if (  event->y() > ui->label_fullImage_3->y()+ ui->label_fullImage_3->height() )
            y =  ui->label_fullImage_3->y() + ui->label_fullImage_3->height() -1 ;
        else
            y = event->y();
        end_ = QPoint(x, y);
    }
    else
    {
        start_= QPoint(-1,-1);
        return;
    }

    qDebug()<<"end:"<<end_<<endl;
    if (word_taked_.isNull()){
        start_ = QPoint(-1,-1);
        end_ = QPoint(-1,-1);
        return;
    }

    int src_width = mybbox->width();
    int src_height = mybbox->height();
    double wRatio = 1.0*src_width/ui->label_fullImage_3->width();
    double hRatio = 1.0*src_height/ui->label_fullImage_3->height();

    Food food( int(start_.x()*wRatio),int(start_.y()*hRatio),
               int(end_.x()*wRatio),int(end_.y()*hRatio), word_taked_ );
    mybbox->addObject(food);
    showImage_3();
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{

}
