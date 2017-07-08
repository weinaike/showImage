#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include <QDir>
#include <QListWidgetItem>
#include <bbox.h>
#include <QList>
#include <QMouseEvent>
using namespace std;

namespace Ui {
class Dialog;
}



class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void showImage();
    void showImage_2();
    void showImage_3();
    void delete_object(int startNum);
    void showFullImage(int startNum);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private slots:
    void on_load_clicked();

    void on_upper_clicked();

    void on_next_clicked();

    void on_lineEdit_NUM_editingFinished();

    void on_del_1_clicked();

    void on_lineEdit_DIR_Anno_editingFinished();



    void on_del_2_clicked();

    void on_del_3_clicked();

    void on_del_4_clicked();

    void on_del_5_clicked();

    void on_del_6_clicked();

    void on_load_2_clicked();

    void on_upper_2_clicked();

    void on_next_2_clicked();

    void on_lineEdit_DIR_Anno_2_editingFinished();

    void on_load_3_clicked();

    void on_upper_3_clicked();

    void on_next_3_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_clicked(const QModelIndex &index);

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_saveLabel_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

private:
    Ui::Dialog *ui;
    QString imageDir;
    QString imageDir_2;
    QString imageDir_3;
    QString AnnoDir;
    QString AnnoDir_2;
    QString AnnoDir_3;
    int start_num;
    int start_num_2;
    int start_num_3;
    int all_num;
    int all_num_2;
    int all_num_3;
    QList<QFileInfo> *fileInfo;
    QDir *mydir;
    bbox *mybbox;
    int ind; //foodlist's index now
    QList<QString> words_;

    // draw rect
    QPoint start_;
    QPoint end_;
    QString word_taked_;



};

#endif // DIALOG_H
