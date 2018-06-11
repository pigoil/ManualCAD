#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mcad_widget = new MCadWidget(this);
    setCentralWidget(mcad_widget);
    connect(mcad_widget,SIGNAL(displayHint(QString)),this,SLOT(slot_hint(QString)));
    connect(mcad_widget,SIGNAL(displaySPF(int)),this,SLOT(slot_spf(int)));

    labelHint = new QLabel("选择");
    labelSPF = new QLabel;

    ui->statusBar->addWidget(labelHint);
    ui->statusBar->addPermanentWidget(new QLabel("SPF(us):"));
    ui->statusBar->addPermanentWidget(labelSPF);

    labelHint->setStyleSheet("color:red");

    QActionGroup* g = new QActionGroup(this);
    g->addAction(ui->actionUseStupid);
    g->addAction(ui->actionUseQtGui);
    g->addAction(ui->actionUseOpenGL);

    g->setExclusive(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionUseStupid_triggered()
{
    mcad_widget->setEngineType(MCadWidget::Stupid);
}

void MainWindow::on_actionUseQtGui_triggered()
{
    mcad_widget->setEngineType(MCadWidget::QtGUI);
}

void MainWindow::on_actionUseOpenGL_triggered()
{
    mcad_widget->setEngineType(MCadWidget::OpenGL);
}

void MainWindow::slot_hint(QString s)
{
    labelHint->setText(s);
}

void MainWindow::slot_spf(int n)
{
    QString s = QString("%1").arg(n/1000, 5, 10, QChar(' '));
    labelSPF->setText(s);
}

void MainWindow::on_actionAbout_triggered()
{
    QString s("MaunalCAD \
              dsadsadas");
    QMessageBox::about(this,"关于",s);

}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}
