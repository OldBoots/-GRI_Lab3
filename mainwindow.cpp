#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    read_data();
    show_data();
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(slot()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(slot1()));
}

void MainWindow::read_data(){
    // Считываем исходное изображение в массив Байт
    QFile file("C:\\Qt\\Project\\GRI_Lab3\\PERTSEV.bmp");
    file.open(QIODevice::ReadOnly);
    arr = file.readAll();
    file.close();
    // Получаем данные о картинки из определенных Байт заголовка
    inf_size = sum_Byts(2, 5);
    inf_wight = sum_Byts(18, 21);
    inf_height = sum_Byts(22, 25);
    inf_depth = sum_Byts(28, 29);
    inf_num_col = sum_Byts(46, 49);
    inf_point_arr = sum_Byts(10, 13);
}
void MainWindow::show_data(){
    // Выводим данные на форму
    QImage img("C:\\Qt\\Project\\GRI_Lab3\\PERTSEV.bmp");
    set_image("C:\\Qt\\Project\\GRI_Lab3\\PERTSEV.bmp");
    QString str;
    str = "Размер файла: " + QString::number(inf_size) + " Байт"
            + "\nРазмер изображения: " + QString::number(sum_Byts(18, 21)) + "x"
            + QString::number(sum_Byts(22, 25))
            + "\nГлубина изображения: " + QString::number(inf_depth) + " бит"
            + "\nФормат изображения: " + format_image(img.format())
            + "\nКол-во цветов в палитре: ";
    if(inf_num_col == 0){
        str += "Палитра отсутствует";
    } else{
        str += QString::number(inf_num_col);
    }
    ui->label_2->setText(str);
}

void MainWindow::edit_data(int data, int begin, int length){
    int ff;
    for(int i = 0, shift = 0; i < length; i++, shift += 8){
        ff = data;
        ff = ff >> shift;
        ff = ff & 0xff;
        arr[begin + i] = (char)ff;
    }
}

void MainWindow::add_pixel(int r, int g, int b, int a){
    arr.push_back((char)r);
    arr.push_back((char)g);
    arr.push_back((char)b);
    arr.push_back((char)a);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_image(QString path)
{
    ui->label->setPixmap(QPixmap(path).scaled(400,
                                              400,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
}

QString MainWindow::format_image(int format)
{
    switch(format){
    case 0:
        return "Invalid";
    case 1:
        return "Mono";
    case 2:
        return "MonoLSB";
    case 3:
        return "Indexed8";
    case 4:
        return "RGB32";
    case 5:
        return "ARGB32";
    case 6:
        return "ARGB32_Premultiplied";
    case 7:
        return "RGB16";
    case 8:
        return "ARGB8565_Premultiplied";
    case 9:
        return "RGB666";
    case 10:
        return "ARGB6666_Premultiplied";
    case 11:
        return "RGB555";
    case 12:
        return "ARGB8555_Premultiplied";
    case 13:
        return "RGB888";
    case 14:
        return "RGB444";
    default:
        return "Other";
    }
}

int MainWindow::sum_Byts(int begin, int end)
{
    int rez = 0;
    for(int i = begin, shift = 0; i <= end; i++, shift += 8){
        rez += QByteArray::fromRawData(&arr[i], 1).toHex().toInt(nullptr, 16) << shift;
    }
    return rez;
}

void MainWindow::slot()
{
    read_data();
    init_matrix();
    for(int i = 0; i < inf_wight; i++){
        for(int j = inf_height - 1; j >= 0; j--){
            add_pixel(matrix_pixel[j][i].red(),
                      matrix_pixel[j][i].green(),
                      matrix_pixel[j][i].blue(),
                      matrix_pixel[j][i].alpha());
        }
    }
    QFile file("C:\\Qt\\Project\\GRI_Lab3\\PERTSEV.bmp");
    file.open(QIODevice::WriteOnly);
    for(int i = 0; i < arr.size(); i++){
        file.putChar(arr[i]);
    }
    file.close();
    set_image("C:\\Qt\\Project\\GRI_Lab3\\PERTSEV.bmp");
    show_data();
}

void MainWindow::slot1()
{
    read_data();
    init_matrix();
    for(int i = inf_wight - 1; i >= 0; i--){
        for(int j = 0; j < inf_height; j++){
            add_pixel(matrix_pixel[j][i].red(),
                      matrix_pixel[j][i].green(),
                      matrix_pixel[j][i].blue(),
                      matrix_pixel[j][i].alpha());
        }
    }
    QFile file("C:\\Qt\\Project\\GRI_Lab3\\PERTSEV.bmp");
    file.open(QIODevice::WriteOnly);
    for(int i = 0; i < arr.size(); i++){
        file.putChar(arr[i]);
    }
    file.close();
    set_image("C:\\Qt\\Project\\GRI_Lab3\\PERTSEV.bmp");
    show_data();
}

int MainWindow::byte_to_int(char byte){
    return QByteArray::fromRawData(&byte, 1).toHex().toInt(nullptr, 16);
}

void MainWindow::init_matrix()
{
    if(!matrix_pixel.isEmpty()){
        matrix_pixel.clear();
    }
    for(int i = 0; i < inf_height; i++){
        row_pixel.clear();
        for(int j = 0; j < inf_wight * 4; j += 4){
            row_pixel << QColor(byte_to_int(arr[inf_point_arr + i * (inf_wight * 4) + j]),
                                byte_to_int(arr[inf_point_arr + i * (inf_wight * 4) + j + 1]),
                                byte_to_int(arr[inf_point_arr + i * (inf_wight * 4) + j + 2]));
        }
        matrix_pixel << row_pixel;
    }
    arr.remove(inf_point_arr, arr.size() - inf_point_arr);
    edit_data(inf_height, 18, 4);
    edit_data(inf_wight, 22, 4);
}

