#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QColorDialog>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void set_image(QString str);
    QString format_image(int format);
    QString yes_no(bool flg);
    int sum_Byts(int begin, int end);
    void edit_data(int data, int begin, int length);
    void add_pixel(int r, int g, int b, int a);
    void init_matrix();
    int byte_to_int(char byte);
    void read_data();
    void show_data();
private:
    Ui::MainWindow *ui;
    QByteArray arr;
    QVector <QVector <QColor>> matrix_pixel;
    QVector <QColor> row_pixel;
    int frame_size;
    int inf_size;
    int inf_wight;
    int inf_height;
    int inf_depth;
    int inf_num_col;
    int inf_point_arr;
private slots:
    void slot();
    void slot1();
};
#endif // MAINWINDOW_H
