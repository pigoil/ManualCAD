#include "mainwidget.h"
#include <QPaintEvent>

MCadWidget::MCadWidget(QWidget *parent) :
    QWidget(parent),
    m_use_opengl(false)
{
    m_paint_engines[Stupid] = new StupidPaintEngine(this);
    m_paint_engines[QtGUI]  = new QtPaintEngine(this);
    m_paint_engines[OpenGL] = new QtPaintEngine(this);

    setEngineType(Stupid);
    m_command = new MCadCommand::PlaceCircle(this);

    setMouseTracking(true);
}

void MCadWidget::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    p.fillRect(e->rect(),Qt::black);
    p.setPen(Qt::green);
    m_command->paint(e,p,m_current_engine);
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

void MCadWidget::setEngineType(MCadWidget::EngineType type)
{
    if(type == OpenGL)
    {
        m_use_opengl = true;
        //GLWIDGET
        //
    }
    else
    {
        m_use_opengl = false;
    }

    m_current_engine = m_paint_engines[type];
}
