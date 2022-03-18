#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = new Image();
    this->setWindowFlags (Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle("Ваш редактор, Сэр");
    connect(ui->Open_button, SIGNAL(triggered()), SLOT(load_file()));
    connect(ui->Save_button, SIGNAL(triggered()), SLOT(save_file()));
    connect(ui->Closef_button, SIGNAL(triggered()), SLOT(close_file()));
    connect(ui->Close_button, SIGNAL(triggered()), SLOT(close_programm()));
    connect(ui->Fullsize_button, SIGNAL(changed()), SLOT(fullsize_decorator()));
    connect(ui->Beauty_button, SIGNAL(triggered()), SLOT(decorator()));
    connect(ui->Alpha_Button, SIGNAL(triggered()), SLOT(alpha_decorator()));
    connect(ui->Help_button, SIGNAL(triggered()), SLOT(help()));
    connect(ui->Info_button, SIGNAL(triggered()), SLOT(info()));
    connect(ui->Circle, SIGNAL(clicked()), SLOT(circle_clicked()));
    connect(ui->RGB, SIGNAL(clicked()), SLOT(RGB_clicked()));
    connect(ui->Split, SIGNAL(clicked()), SLOT(split_clicked()));
    connect(ui->Square, SIGNAL(clicked()), SLOT(square_clicked()));
    connect(ui->white, SIGNAL(clicked()), SLOT(white_clicked()));
    ui->Save_button->setDisabled(true);
    ui->Closef_button->setDisabled(true);
    ui->Fullsize_button->setDisabled(true);
    ui->Beauty_button->setDisabled(true);
    ui->Alpha_Button->setDisabled(true);
    ui->Circle->setDisabled(true);
    ui->RGB->setDisabled(true);
    ui->Split->setDisabled(true);
    ui->Square->setDisabled(true);
    ui->white->setDisabled(true);
}

MainWindow::~MainWindow()
{
    save_check();
    QApplication::quit();
    delete ui;
}

void MainWindow::fileopen_check(bool open){
    if(open){
        ui->Closef_button->setEnabled(true);
        ui->Fullsize_button->setEnabled(true);
        ui->Beauty_button->setEnabled(true);
        ui->Alpha_Button->setEnabled(true);
        ui->Circle->setEnabled(true);
        ui->RGB->setEnabled(true);
        ui->Split->setEnabled(true);
        ui->Square->setEnabled(true);
        ui->white->setEnabled(true);
    }
    else{
        ui->Closef_button->setDisabled(true);
        ui->Fullsize_button->setDisabled(true);
        ui->Beauty_button->setDisabled(true);
        ui->Alpha_Button->setDisabled(true);
        ui->Circle->setDisabled(true);
        ui->RGB->setDisabled(true);
        ui->Split->setDisabled(true);
        ui->Square->setDisabled(true);
        ui->white->setDisabled(true);
    }
}

void MainWindow::filechanged_check(bool changed){
    //if(changed)
        ui->Save_button->setEnabled(true);
    //else
     //   ui->Save_button->setDisabled(true);
}

void MainWindow::save_check()
{
    if(ui->Save_button->isEnabled()){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Сохранение изображения", "Хотите сохранить изменения?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            save_file();
        }
    }
}

void MainWindow::menu_check()
{
    if(MenuCircle){
        remove_circle();
        MenuCircle = false;
        return;
    }
    if(MenuRGB){
        remove_RGB();
        MenuRGB = false;
        return;
    }
    if(MenuSplit){
        remove_split();
        MenuSplit = false;
        return;
    }
    if(MenuSquare){
        remove_square();
        MenuSquare = false;
        return;
    }
    if(MenuWhite){
        remove_white();
        MenuWhite = false;
        return;
    }
}

void MainWindow::load_file()
{
    save_check();
    menu_check();
    QString path = QFileDialog::getOpenFileName(this, "Открыть файл", "", "*.png");
    if(path.isEmpty())
        return;
    const char* char_path = path.toUtf8().constData();
    int status = img->read_png_file(char_path);
    if(status < 0){
        QMessageBox::warning(this, "Ошибка", "не удалось прочитать файл");
        return;
    }
    QPoint begin = img->get_size();
    image_width = begin.x();
    image_height = begin.y();
    QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
    ui->Image_widget->setScaledPixmap(pixmap);
    setMaximumHeight(image_height + 96);
    setMaximumWidth(image_width + 258);
    fullsize_decorator();
    filechanged_check(false);
    fileopen_check(true);
}

void MainWindow::save_file()
{
    QString path = QFileDialog::getSaveFileName(this, "Сохранить Файл", "", "*.png");
    if(path.isEmpty())
        return;
    const char* char_path = path.toUtf8().constData();
    int status = img->save_png_file(char_path);
    if(status < 0){
        QMessageBox::warning(this, "Ошибка", "не удалось сохранить файл");
        return;
    }
    ui->statusBar->showMessage("Файл сохранен успешно");
    filechanged_check(false);
}

void MainWindow::close_file()
{
    save_check();
    menu_check();
    QPixmap pixmap("");
    ui->Image_widget->setScaledPixmap(pixmap);
    ui->Image_widget->removeScaledPixmap();
    img->close_png_file();
    image_width = 476;
    image_height = 476;
    filechanged_check(false);
    fileopen_check(false);
    fullsize_decorator();
}

void MainWindow::close_programm(){
    menu_check();
    QApplication::quit();
}

void MainWindow::fullsize_decorator(){
    if(ui->Fullsize_button->isChecked()){
        ui->Image_widget->setMinimumHeight(image_height);
        ui->Image_widget->setMinimumWidth(image_width);
    }
    else{
        ui->Image_widget->setMinimumHeight(476);
        ui->Image_widget->setMinimumWidth(476);
    }
}

void MainWindow::decorator(){
    QPoint start_pixmap = ui->Image_widget->resizeWidget();
    if(start_pixmap.x() == 0 && start_pixmap.y() == 0){
        return;
    }
    int x_deviation = 2 * (start_pixmap.x() - ((width() - 734) / 2)) * image_height / image_width;
    int y_deviation = 2 * (start_pixmap.y() - ((height() - 572) / 2)) * image_width / image_height;
    resize(width() - (start_pixmap.x()*2) , height() - (start_pixmap.y()*2));

    if(width() == 734 && x_deviation > 0 && image_height > image_width){
        resize(width(), height() + x_deviation);
    }
    if(height() == 572 && y_deviation > 0 && image_width > image_height){
        resize(width() + y_deviation, height());
    }
}

void MainWindow::alpha_decorator(){
    QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
    ui->Image_widget->setScaledPixmap(pixmap);
}

void MainWindow::help(){
    window = new helpwindow(this);
    window->show();
}

void MainWindow::info(){
    QMessageBox::information(this, "О приложении", "Курсовая работа<br>Тема: Работа с png файлами<br>Автор: Низовцов Ростислав<br>Группа: 9303");
}

void MainWindow::line_color_dialog()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Выберите цвет",  QColorDialog::DontUseNativeDialog);
    if (!color.isValid()){
        Line.setRgb(0, 0, 0);
        return;
    }
    Line = color;
}

void MainWindow::background_color_dialog(){
    QCheckBox *background = qobject_cast<QCheckBox *>(ui->gridLayout->itemAtPosition(3, 1)->widget());
    if(background->isChecked()){
        QColor color = QColorDialog::getColor(Qt::black, this, "Выберите цвет",  QColorDialog::DontUseNativeDialog);
        if (!color.isValid()){
            Background.setRgb(255, 255, 255, 0);
            return;
        }
        Background = color;
    }
}

void MainWindow::circle_clicked(){
    ui->statusBar->showMessage("");
    menu_check();
    ui->MainLable->setText("Выберите размер<br>окружности, зажав<br>клавишу мыши");
    QLabel *width = new QLabel(this);
    width->setText("Толщина:");
    ui->gridLayout->addWidget(width, 1, 0);
    QSpinBox *width_count = new QSpinBox(this);
    width_count->setMinimum(1);
    ui->gridLayout->addWidget(width_count, 1, 1);
    QPushButton *color = new QPushButton(this);
    color->setText("Цвет линии");
    ui->gridLayout->addWidget(color, 2, 1);
    connect(color, SIGNAL(clicked()), SLOT(line_color_dialog()));
    QLabel *background_text = new QLabel(this);
    background_text->setText("Фон");
    ui->gridLayout->addWidget(background_text, 3, 0);
    QCheckBox *background = new QCheckBox(this);
    ui->gridLayout->addWidget(background, 3, 1);
    QPushButton *in_color = new QPushButton(this);
    in_color->setText("Цвет фона");
    ui->gridLayout->addWidget(in_color, 4, 1);
    connect(in_color, SIGNAL(clicked()), SLOT(background_color_dialog()));
    QPushButton *return_button = new QPushButton(this);
    return_button->setText("Назад");
    ui->gridLayout->addWidget(return_button, 5, 0);
    connect(return_button, SIGNAL(clicked()), SLOT(remove_circle()));
    QPushButton *enter_button = new QPushButton;
    enter_button->setText("Готово");
    ui->gridLayout->addWidget(enter_button, 5, 1);
    connect(enter_button, SIGNAL(clicked()), SLOT(enter_circle()));
    ui->Fullsize_button->setChecked(true);
    ui->Fullsize_button->setDisabled(true);
    ui->Image_widget->resetMouse();
    MenuCircle = true;
}

void MainWindow::remove_circle(){
    MenuCircle = false;
    ui->statusBar->showMessage("");
    ui->MainLable->setText("");
    QLabel *width = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    width->hide();
    delete width;
    QSpinBox *width_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(1, 1)->widget());
    width_count->hide();
    delete width_count;
    QPushButton *color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(2, 1)->widget());
    disconnect(color, SIGNAL(clicked()), this, SLOT(line_color_dialog()));
    color->hide();
    delete color;
    QLabel *background_text = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(3, 0)->widget());
    background_text->hide();
    delete background_text;
    QCheckBox *background = qobject_cast<QCheckBox *>(ui->gridLayout->itemAtPosition(3, 1)->widget());
    background->hide();
    delete background;
    QPushButton *in_color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(4, 1)->widget());
    disconnect(in_color, SIGNAL(clicked()), this, SLOT(background_color_dialog()));
    in_color->hide();
    delete in_color;
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 0)->widget());
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_circle()));
    return_button->hide();
    delete return_button;
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 1)->widget());
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_circle()));
    enter_button->hide();
    delete enter_button;
    ui->Fullsize_button->setChecked(false);
    ui->Fullsize_button->setEnabled(true);
}

void MainWindow::enter_circle(){
    ui->statusBar->showMessage("");
    QLabel *width = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    QSpinBox *width_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(1, 1)->widget());
    QPushButton *color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(2, 1)->widget());
    QLabel *background_text = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(3, 0)->widget());
    QCheckBox *background = qobject_cast<QCheckBox *>(ui->gridLayout->itemAtPosition(3, 1)->widget());
    QPushButton *in_color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(4, 1)->widget());
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 0)->widget());
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 1)->widget());
    QPoint first_point = ui->Image_widget->backMouse(1);
    QPoint second_point = ui->Image_widget->backMouse(2);
    if(second_point.x() > image_width)
        second_point.setX(image_width-1);
    if(second_point.x() < 0)
        second_point.setX(0);
    if(second_point.y() > image_height)
        second_point.setY(image_height-1);
    if(second_point.y() < 0)
        second_point.setY(0);
    if(first_point == second_point){
        QMessageBox::warning(this, "Ошибка", "Ошибка: некорректные координаты<br>Попробуйте еще раз");
        return;
    }
    if(second_point.x() < first_point.x()){
        int temp = first_point.x();
        first_point.setX(second_point.x());
        second_point.setX(temp);
    }
    if(second_point.y() < first_point.y()){
        int temp = first_point.y();
        first_point.setY(second_point.y());
        second_point.setY(temp);
    }
    img->circle_func(image_width, image_height,first_point, second_point, width_count->value(), Line, background->isChecked(), Background);
    QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
    ui->Image_widget->setScaledPixmap(pixmap);
    MenuCircle = false;
    filechanged_check(true);
    ui->MainLable->setText("");
    width->hide();
    delete width;
    width_count->hide();
    delete width_count;
    disconnect(color, SIGNAL(clicked()), this, SLOT(line_color_dialog()));
    color->hide();
    delete color;
    background_text->hide();
    delete background_text;
    background->hide();
    delete background;
    disconnect(in_color, SIGNAL(clicked()), this, SLOT(background_color_dialog()));
    in_color->hide();
    delete in_color;
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_circle()));
    return_button->hide();
    delete return_button;
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_circle()));
    enter_button->hide();
    delete enter_button;
    ui->Fullsize_button->setChecked(false);
    ui->Fullsize_button->setEnabled(true);
}

void MainWindow::RGB_clicked()
{
    ui->statusBar->showMessage("");
    menu_check();
    ui->MainLable->setText("Выберите<br>RGB-компонент<br>и значение");
    QRadioButton *red = new QRadioButton(this);
    red->setText("Красный");
    ui->gridLayout->addWidget(red, 1, 0);
    QRadioButton *green = new QRadioButton(this);
    green->setText("Зеленый");
    ui->gridLayout->addWidget(green, 2, 0);
    QRadioButton *blue = new QRadioButton(this);
    blue->setText("Синий");
    ui->gridLayout->addWidget(blue, 3, 0);
    QLabel *count = new QLabel(this);
    count->setText("Значение:");
    ui->gridLayout->addWidget(count, 4, 0);
    QSpinBox *box = new QSpinBox(this);
    box->setMaximum(255);
    box->setMinimum(0);
    ui->gridLayout->addWidget(box, 4, 1);
    QPushButton *return_button = new QPushButton(this);
    return_button->setText("Назад");
    ui->gridLayout->addWidget(return_button, 5, 0);
    connect(return_button, SIGNAL(clicked()), SLOT(remove_RGB()));
    QPushButton *enter_button = new QPushButton;
    enter_button->setText("Готово");
    ui->gridLayout->addWidget(enter_button, 5, 1);
    connect(enter_button, SIGNAL(clicked()), SLOT(enter_RGB()));
    MenuRGB = true;
}

void MainWindow::remove_RGB()
{
    MenuRGB = false;
    ui->statusBar->showMessage("");
    ui->MainLable->setText("");
    QRadioButton *red = qobject_cast<QRadioButton *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    red->hide();
    delete red;
    QRadioButton *green = qobject_cast<QRadioButton *>(ui->gridLayout->itemAtPosition(2, 0)->widget());
    green->hide();
    delete green;
    QRadioButton *blue = qobject_cast<QRadioButton *>(ui->gridLayout->itemAtPosition(3,0)->widget());
    blue->hide();
    delete blue;
    QLabel *count = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(4, 0)->widget());
    count->hide();
    delete count;
    QSpinBox *box = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(4, 1)->widget());
    box->hide();
    delete box;
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 0)->widget());
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_RGB()));
    return_button->hide();
    delete return_button;
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 1)->widget());
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_RGB()));
    enter_button->hide();
    delete enter_button;
}

void MainWindow::enter_RGB()
{
    ui->statusBar->showMessage("");
    QRadioButton *red = qobject_cast<QRadioButton *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    QRadioButton *green = qobject_cast<QRadioButton *>(ui->gridLayout->itemAtPosition(2, 0)->widget());
    QRadioButton *blue = qobject_cast<QRadioButton *>(ui->gridLayout->itemAtPosition(3, 0)->widget());
    QLabel *count = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(4, 0)->widget());
    QSpinBox *box = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(4, 1)->widget());
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 0)->widget());
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 1)->widget());
    if(red->isChecked())
    {
        img->RGB_func(1, box->value());
        QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
        ui->Image_widget->setScaledPixmap(pixmap);
        ui->statusBar->showMessage("Красный компонент успешно изменен");
    } else if(green->isChecked())
    {
        img->RGB_func(2, box->value());
        QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
        ui->Image_widget->setScaledPixmap(pixmap);
        ui->statusBar->showMessage("Зеленый компонент успешно изменен");
    } else if(blue->isChecked())
    {
        img->RGB_func(3, box->value());
        QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
        ui->Image_widget->setScaledPixmap(pixmap);
        ui->statusBar->showMessage("Синий компонент успешно изменен");
    } else
    {
        QMessageBox::warning(this, "Внимание", "Выберите компонент");
        return;
    }
    MenuRGB = false;
    filechanged_check(true);
    ui->MainLable->setText("");
    red->hide();
    delete red;
    green->hide();
    delete green;
    blue->hide();
    delete blue;
    count->hide();
    delete count;
    box->hide();
    delete box;
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_RGB()));
    return_button->hide();
    delete return_button;
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_RGB()));
    enter_button->hide();
    delete enter_button;
}

void MainWindow::split_clicked()
{
    ui->statusBar->showMessage("");
    menu_check();
    ui->MainLable->setText("Выберите<br>количество частей<br>и параметры линий");
    QLabel *x = new QLabel(this);
    x->setText("По X:");
    ui->gridLayout->addWidget(x, 1, 0);
    QSpinBox *x_count = new QSpinBox(this);
    x_count->setMinimum(1);
    ui->gridLayout->addWidget(x_count, 1, 1);
    QLabel *y = new QLabel(this);
    y->setText("По Y:");
    ui->gridLayout->addWidget(y, 2, 0);
    QSpinBox *y_count = new QSpinBox(this);
    y_count->setMinimum(1);
    ui->gridLayout->addWidget(y_count, 2, 1);
    QLabel *width = new QLabel(this);
    width->setText("Толщина:");
    ui->gridLayout->addWidget(width, 3, 0);
    QSpinBox *width_count = new QSpinBox(this);
    width_count->setMinimum(1);
    ui->gridLayout->addWidget(width_count, 3, 1);
    QPushButton *color = new QPushButton(this);
    color->setText("Цвет линии");
    ui->gridLayout->addWidget(color, 4, 1);
    connect(color, SIGNAL(clicked()), SLOT(line_color_dialog()));
    QPushButton *return_button = new QPushButton(this);
    return_button->setText("Назад");
    ui->gridLayout->addWidget(return_button, 5, 0);
    connect(return_button, SIGNAL(clicked()), SLOT(remove_split()));
    QPushButton *enter_button = new QPushButton;
    enter_button->setText("Готово");
    ui->gridLayout->addWidget(enter_button, 5, 1);
    connect(enter_button, SIGNAL(clicked()), SLOT(enter_split()));
    MenuSplit = true;
}

void MainWindow::remove_split()
{
    MenuSplit = false;
    ui->statusBar->showMessage("");
    ui->MainLable->setText("");
    Line.setRgb(255, 255, 255);
    QLabel *x = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    x->hide();
    delete x;
    QSpinBox *x_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(1, 1)->widget());
    x_count->hide();
    delete x_count;
    QLabel *y = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(2, 0)->widget());
    y->hide();
    delete y;
    QSpinBox *y_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(2, 1)->widget());
    y_count->hide();
    delete y_count;
    QLabel *width = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(3, 0)->widget());
    width->hide();
    delete width;
    QSpinBox *width_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(3, 1)->widget());
    width_count->hide();
    delete width_count;
    QPushButton *color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(4, 1)->widget());
    disconnect(color, SIGNAL(clicked()), this, SLOT(line_color_dialog()));
    color->hide();
    delete color;
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 0)->widget());
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_split()));
    return_button->hide();
    delete return_button;
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 1)->widget());
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_split()));
    enter_button->hide();
    delete enter_button;
}

void MainWindow::enter_split()
{
    ui->statusBar->showMessage("");
    QLabel *x = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    QSpinBox *x_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(1, 1)->widget());
    QLabel *y = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(2, 0)->widget());
    QSpinBox *y_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(2, 1)->widget());
    QLabel *width = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(3, 0)->widget());
    QSpinBox *width_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(3, 1)->widget());
    QPushButton *color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(4, 1)->widget());
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 0)->widget());
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 1)->widget());
    img->split_func(x_count->value(), y_count->value(), width_count->value(), Line.rgb());
    QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
    ui->Image_widget->setScaledPixmap(pixmap);
    ui->statusBar->showMessage("Картинка успешно разделена");
    MenuSplit = false;
    filechanged_check(true);
    ui->MainLable->setText("");
    x->hide();
    delete x;
    x_count->hide();
    delete x_count;
    y->hide();
    delete y;
    y_count->hide();
    delete y_count;
    width->hide();
    delete width;
    width_count->hide();
    delete width_count;
    disconnect(color, SIGNAL(clicked()), this, SLOT(line_color_dialog()));
    color->hide();
    delete color;
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_split()));
    return_button->hide();
    delete return_button;
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_split()));
    enter_button->hide();
    delete enter_button;
}

void MainWindow::square_clicked()
{
    ui->statusBar->showMessage("");
    menu_check();
    ui->MainLable->setText("Выберите вершину и<br>размер квадрата, зажав<br>клавишу мыши");
    QLabel *width = new QLabel(this);
    width->setText("Толщина:");
    ui->gridLayout->addWidget(width, 1, 0);
    QSpinBox *width_count = new QSpinBox(this);
    width_count->setMinimum(1);
    width_count->setMaximum(89);
    ui->gridLayout->addWidget(width_count, 1, 1);
    QPushButton *color = new QPushButton(this);
    color->setText("Цвет линии");
    ui->gridLayout->addWidget(color, 2, 1);
    connect(color, SIGNAL(clicked()), SLOT(line_color_dialog()));
    QLabel *background_text = new QLabel(this);
    background_text->setText("Фон");
    ui->gridLayout->addWidget(background_text, 3, 0);
    QCheckBox *background = new QCheckBox(this);
    ui->gridLayout->addWidget(background, 3, 1);
    QPushButton *in_color = new QPushButton(this);
    in_color->setText("Цвет фона");
    ui->gridLayout->addWidget(in_color, 4, 1);
    connect(in_color, SIGNAL(clicked()), SLOT(background_color_dialog()));
    QPushButton *return_button = new QPushButton(this);
    return_button->setText("Назад");
    ui->gridLayout->addWidget(return_button, 5, 0);
    connect(return_button, SIGNAL(clicked()), SLOT(remove_square()));
    QPushButton *enter_button = new QPushButton;
    enter_button->setText("Готово");
    ui->gridLayout->addWidget(enter_button, 5, 1);
    connect(enter_button, SIGNAL(clicked()), SLOT(enter_square()));
    MenuSquare = true;
    ui->Fullsize_button->setChecked(true);
    ui->Fullsize_button->setDisabled(true);
    ui->Image_widget->resetMouse();
}

void MainWindow::remove_square(){
    MenuSquare = false;
    ui->statusBar->showMessage("");
    ui->MainLable->setText("");
    QLabel *width = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    width->hide();
    delete width;
    QSpinBox *width_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(1, 1)->widget());
    width_count->hide();
    delete width_count;
    QPushButton *color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(2, 1)->widget());
    disconnect(color, SIGNAL(clicked()), this, SLOT(line_color_dialog()));
    color->hide();
    delete color;
    QLabel *background_text = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(3, 0)->widget());
    background_text->hide();
    delete background_text;
    QCheckBox *background = qobject_cast<QCheckBox *>(ui->gridLayout->itemAtPosition(3, 1)->widget());
    background->hide();
    delete background;
    QPushButton *in_color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(4, 1)->widget());
    disconnect(in_color, SIGNAL(clicked()), this, SLOT(background_color_dialog()));
    in_color->hide();
    delete in_color;
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 0)->widget());
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_square()));
    return_button->hide();
    delete return_button;
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 1)->widget());
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_square()));
    enter_button->hide();
    delete enter_button;
    ui->Fullsize_button->setChecked(false);
    ui->Fullsize_button->setEnabled(true);
}

void MainWindow::enter_square(){
    ui->statusBar->showMessage("");
    QLabel *width = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    QSpinBox *width_count = qobject_cast<QSpinBox *>(ui->gridLayout->itemAtPosition(1, 1)->widget());
    QPushButton *color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(2, 1)->widget());
    QLabel *background_text = qobject_cast<QLabel *>(ui->gridLayout->itemAtPosition(3, 0)->widget());
    QCheckBox *background = qobject_cast<QCheckBox *>(ui->gridLayout->itemAtPosition(3, 1)->widget());
    QPushButton *in_color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(4, 1)->widget());
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 0)->widget());
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(5, 1)->widget());
    QPoint C = ui->Image_widget->backMouse(1);
    QPoint A = ui->Image_widget->backMouse(2);
    if(A.x() > image_width)
        C.setX(image_width-1);
    if(A.x() < 0)
        C.setX(0);
    if(A.y() > image_height)
        A.setY(image_height-1);
    if(A.y() < 0)
        A.setY(0);
    if(C == A){
        QMessageBox::warning(this, "Ошибка", "Ошибка: некорректные координаты<br>Попробуйте еще раз");
        return;
    }
    if(A.x() < C.x()){
        int temp = C.x();
        C.setX(A.x());
        A.setX(temp);
        temp = C.y();
        C.setY(A.y());
        A.setY(temp);
    }
    img->square_func(image_width, image_height, C, A, width_count->value(), Line, background->isChecked(), Background);
    QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
    ui->Image_widget->setScaledPixmap(pixmap);
    MenuSquare = false;
    filechanged_check(true);
    ui->MainLable->setText("");
    width->hide();
    delete width;
    width_count->hide();
    delete width_count;
    disconnect(color, SIGNAL(clicked()), this, SLOT(line_color_dialog()));
    color->hide();
    delete color;
    background_text->hide();
    delete background_text;
    background->hide();
    delete background;
    disconnect(in_color, SIGNAL(clicked()), this, SLOT(background_color_dialog()));
    in_color->hide();
    delete in_color;
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_square()));
    return_button->hide();
    delete return_button;
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_square()));
    enter_button->hide();
    delete enter_button;
    ui->Fullsize_button->setChecked(false);
    ui->Fullsize_button->setEnabled(true);
}

void MainWindow::white_clicked(){
    Line.setRgb(0, 0, 0);
    ui->statusBar->showMessage("");
    menu_check();
    ui->MainLable->setText("Выберите<br>цвет<br>линии");
    QPushButton *color = new QPushButton(this);
    color->setText("Выбрать цвет");
    color->setMinimumWidth(180);
    ui->gridLayout->addWidget(color, 1, 0);
    connect(color, SIGNAL(clicked()), SLOT(line_color_dialog()));
    QPushButton *return_button = new QPushButton(this);
    return_button->setText("Назад");
    ui->gridLayout->addWidget(return_button, 2, 0);
    connect(return_button, SIGNAL(clicked()), SLOT(remove_white()));
    QPushButton *enter_button = new QPushButton;
    enter_button->setText("Готово");
    ui->gridLayout->addWidget(enter_button, 2, 1);
    connect(enter_button, SIGNAL(clicked()), SLOT(enter_white()));
    ui->Image_widget->resetMouse();
    MenuWhite = true;
}

void MainWindow::remove_white(){
    MenuWhite = false;
    ui->statusBar->showMessage("");
    ui->MainLable->setText("");
    QPushButton *color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    disconnect(color, SIGNAL(clicked()), this, SLOT(line_color_dialog()));
    color->hide();
    delete color;
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(2, 0)->widget());
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_white()));
    return_button->hide();
    delete return_button;
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(2, 1)->widget());
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_white()));
    enter_button->hide();
    delete enter_button;

}

void MainWindow::enter_white(){
    ui->statusBar->showMessage("");
    QPushButton *color = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(1, 0)->widget());
    QPushButton *return_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(2, 0)->widget());
    QPushButton *enter_button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(2, 1)->widget());
    img->white_func(image_width, image_height, Line);
    QPixmap pixmap = img->get_pixmap(ui->Alpha_Button->isChecked());
    ui->Image_widget->setScaledPixmap(pixmap);
    MenuWhite = false;
    filechanged_check(true);
    ui->MainLable->setText("");
    disconnect(color, SIGNAL(clicked()), this, SLOT(line_color_dialog()));
    color->hide();
    delete color;
    disconnect(return_button, SIGNAL(clicked()), this, SLOT(remove_white()));
    return_button->hide();
    delete return_button;
    disconnect(enter_button, SIGNAL(clicked()), this, SLOT(enter_white()));
    enter_button->hide();
    delete enter_button;
}
