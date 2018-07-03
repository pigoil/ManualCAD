#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QCheckBox>

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

    QActionGroup* groupEngine = new QActionGroup(this);
    groupEngine->addAction(ui->actionUseStupid);
    groupEngine->addAction(ui->actionUseQtGui);
    groupEngine->addAction(ui->actionUseOpenGL);
    groupEngine->setExclusive(true);

    QActionGroup* groupPlace = new QActionGroup(this);
    groupPlace->addAction(ui->actionPlaceLine);
    groupPlace->addAction(ui->actionPlaceCircle);
    groupPlace->addAction(ui->actionPlaceRect);
    groupPlace->setExclusive(true);
    connect(groupPlace,SIGNAL(triggered(QAction*)),mcad_widget,SLOT(startNewCommand(QAction*)));

    QActionGroup* groupOther = new QActionGroup(this);
    groupOther->addAction(ui->actionDelete);
    groupOther->addAction(ui->actionKoch);
    groupOther->addAction(ui->actionCone);
    groupOther->addAction(ui->actionPodetium);
    groupOther->addAction(ui->actionAnimation);
    connect(groupOther,SIGNAL(triggered(QAction*)),mcad_widget,SLOT(startNewCommand(QAction*)));

    connect(ui->menuColor,SIGNAL(triggered(QAction*)),mcad_widget,SLOT(setColorOption(QAction*)));

    QCheckBox* blankingBox = new QCheckBox("开启消隐",this);
    ui->statusBar->addWidget(blankingBox);

    connect(blankingBox,SIGNAL(toggled(bool)),mcad_widget,SLOT(setUseBlanking(bool)));
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
    QString title("关于MaunalCAD");
    QString text("<h1>Manual CAD</h1>"
                 "<h2>简介</h2>"
                 "<font size=\"6\">"
                 "<p>基于Qt 5.8开发的CAD软件，特性：</p>"
                 "<ul>"
                 "<li>绘制二维线、矩形、圆、科赫曲线</li>"
                 "<li>从二维矩形、圆生成锥体、柱体</li>"
                 "<li>删除选中的图形</li>"
                 "<li>非编辑状态下缩放、三维旋转、平移</li>"
                 "<li>非编辑状态下选中</li>"
                 "<li>可开关的消隐效果</li>"
                 "<li>可开关的单体旋转动画</li>"
                 "<li>三种可选绘图引擎</li>"
                 "<li>可选背景色、线条颜色</li>"
                 "</ul>"
                 "</font>"
                 "<h2>组员</h2>"
                 "<font size=\"4\">"
                 "<p>李康清、张逸鸿、何梓秋</p>"
                 "<font>"
                );
    QMessageBox::about(this,title,text);

}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}
