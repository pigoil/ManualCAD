#include <QPaintEvent>
#include <QApplication>
#include <QDebug>

#include "mainwidget.h"
#include "mcad_utils.h"

MCadWidget::MCadWidget(QWidget *parent) :
    QWidget(parent),
    m_glwidget(NULL),
    m_use_opengl(false)
{
    QPixmap cursor = QPixmap(":/icons/Resource/cross.png");
    cursor.setDevicePixelRatio(5);
    setCursor(QCursor(cursor));
    m_paint_engines[Stupid] = new StupidPaintEngine(this);
    m_paint_engines[QtGUI]  = new QtPaintEngine(this);
    m_paint_engines[OpenGL] = new QtPaintEngine(this);

    setEngineType(Stupid);
    setMouseTracking(true);

    m_command = new MCadCommand::PlaceCircle(this);
}

void MCadWidget::paintEvent(QPaintEvent *e)
{
    MCadUtil::StopWatch watch;
    watch.start();
    QPainter p;
    if(m_use_opengl)
        p.begin(m_glwidget);
    else
        p.begin(this);

    p.fillRect(e->rect(),Qt::black);
    p.setPen(Qt::green);
    m_command->paint(e,p,m_current_engine);
    emit displaySPF(watch.tell());
}

void MCadWidget::mouseMoveEvent(QMouseEvent *e)
{
    m_command->move(e);
    update();
}

void MCadWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_command->proceed(e);
        emit displayHint(m_command->hint());
        update();
    }
    QWidget::mousePressEvent(e);
}

void MCadWidget::resizeEvent(QResizeEvent *e)
{
    if(m_use_opengl && m_glwidget)
        m_glwidget->setGeometry(0,0,e->size().width()-1,e->size().height()-1);

    QWidget::resizeEvent(e);
}

void MCadWidget::setEngineType(MCadWidget::EngineType type)
{
    if(type == OpenGL)
    {
        if(!m_glwidget)
        {
            m_glwidget = new QGLWidget(this);
            m_glwidget->setAutoFillBackground(false);
            m_glwidget->setMouseTracking(true);
            m_glwidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);
            QApplication::postEvent(this, new QResizeEvent(size(), size()));
        }
        m_use_opengl = true;
        m_glwidget->show();
        //GLWIDGET
        //
    }
    else
    {
        m_use_opengl = false;
        if(m_glwidget)
            m_glwidget->hide();
    }

    m_current_engine = m_paint_engines[type];
}
