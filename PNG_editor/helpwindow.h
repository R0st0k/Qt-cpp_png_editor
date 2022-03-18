#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

namespace Ui {
class helpwindow;
}

class helpwindow : public QDialog
{
    Q_OBJECT

public:
    explicit helpwindow(QWidget *parent = 0);
    ~helpwindow();

private slots:

    void on_treeWidget_clicked(const QModelIndex &index);

private:
    Ui::helpwindow *ui;
};

#endif // HELPWINDOW_H
