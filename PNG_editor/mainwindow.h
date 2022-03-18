#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "picture.h"
#include "helpwindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QLabel>
#include <QColorDialog>
#include <QColor>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QRect>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool MenuCircle = false;
    bool MenuRGB = false;
    bool MenuSplit = false;
    bool MenuSquare = false;
    bool MenuWhite = false;
    QColor Line;
    QColor Background;

private slots:

    void fileopen_check(bool open);

    void filechanged_check(bool changed);

    void save_check();

    void menu_check();

    void load_file();

    void save_file();

    void close_file();

    void close_programm();

    void fullsize_decorator();

    void decorator();

    void alpha_decorator();

    void help();

    void info();

    void line_color_dialog();

    void background_color_dialog();

    void circle_clicked();

    void remove_circle();

    void enter_circle();

    void RGB_clicked();

    void remove_RGB();

    void enter_RGB();

    void split_clicked();

    void remove_split();

    void enter_split();

    void square_clicked();

    void remove_square();

    void enter_square();

    void white_clicked();

    void remove_white();

    void enter_white();

private:
    Ui::MainWindow *ui;
    helpwindow *window;
    Image *img;
    int image_width = 476;
    int image_height = 476;
};

#endif // MAINWINDOW_H
