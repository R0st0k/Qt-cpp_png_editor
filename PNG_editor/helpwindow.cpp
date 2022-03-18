#include "helpwindow.h"
#include "ui_helpwindow.h"

helpwindow::helpwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpwindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Справка");
    setMaximumHeight(368);
    setMaximumWidth(604);
}

helpwindow::~helpwindow()
{
    delete ui;
}



void helpwindow::on_treeWidget_clicked(const QModelIndex &index)
{
    if(ui->treeWidget->currentItem()->text(0) == "Открыть файл"){
        ui->label->setText("Предлагается дерево файлов<br>для открытия изображения png");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Сохранить файл"){
        ui->label->setText("Предлагается дерево файлов<br>для сохранения текущего<br>изображения png");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Закрыть файл"){
        ui->label->setText("Кнопка для закрытия текущего<br>файла png");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Закрыть приложение"){
        ui->label->setText("Кнопка для закрытия<br>приложения");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Оригинальный размер"){
        ui->label->setText("Кнопка для включения или<br>отключения оригинальных<br>размеров изображения.<br>При включенном состоянии<br>появляются ползунки<br>для контроля размера<br>изображения; при выключенном<br>состоянии картинка вмещается в<br>окно. При рисовании квадрата<br>или окружности этот<br>параметр поставлен по<br>умолчанию");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Выравнивание изображения"){
        ui->label->setText("Изображение расширяется до<br>оригинальных пропорций,<br>подстраиваясь под текущий<br>размер окна");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Вкл/откл альфа-канал фона"){
        ui->label->setText("Кнопка для включения или<br>отключения альфа-канала<br>фона для явного отображения<br>конца изображения");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Справка"){
        ui->label->setText("Кнопка для открытия справки");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "О приложении"){
        ui->label->setText("Краткая информация об авторе<br>и самом приложении");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Нарисовать окружность"){
        ui->label->setText("Включение режима рисования<br>окружности. Требуется выбрать<br>2 точки на изображении -<br>концы диагонали окружности.<br>Точки выбираются нажатием<br>мыши в одной точке диагонали и<br>отпусканием в другой её точке.<br>Также предлагается выбрать<br>толщину и цвет линии, наличие<br>и цвет заливки");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Изменить RGB-компанент"){
        ui->label->setText("Предлагается выбор из<br>компонентов RGB для замены, а<br>также значение, на которое<br>требуется заменить текущие<br>данные");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Разделить картинку"){
        ui->label->setText("Предлагается выбрать<br>количество частей по осям X и Y,<br>а также цвет и толщину линии<br>разделения");
        return;
    }
    if(ui->treeWidget->currentItem()->text(0) == "Нарисовать квадрат"){
        ui->label->setText("Включение режима рисования<br>квадрата. Требуется выбрать<br>2 точки на изображении -<br>вершина и длина стороны.<br>Точки выбираются нажатием<br>мыши в одной точке диагонали и<br>отпусканием в другой её точке.<br>Также предлагается выбрать<br>толщину и цвет линии, наличие<br>и цвет заливки");
        return;
    }
}
