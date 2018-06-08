#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MCadWidget* w = new MCadWidget(this);
    setCentralWidget(w);

    QActionGroup* g = new QActionGroup(this);
    g->addAction(ui->action1);
    g->addAction(ui->action2);
    g->addAction(ui->action3);

    g->setExclusive(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
