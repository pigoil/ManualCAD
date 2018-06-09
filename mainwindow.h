#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "mainwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionUseStupid_triggered();
    void on_actionUseQtGui_triggered();
    void on_actionUseOpenGL_triggered();

    void slot_hint(QString s);
    void slot_spf(QString s);

private:
    Ui::MainWindow *ui;
    MCadWidget* mcad_widget;
    QLabel* labelHint;
    QLabel* labelSPF;
};

#endif // MAINWINDOW_H
