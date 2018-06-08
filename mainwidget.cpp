#include "mainwidget.h"
#include <QPaintEvent>

MCadWidget::MCadWidget(QWidget *parent) : QWidget(parent)
{
    m_paint_engine = new MCadPaintEngine(this);
    m_command = new MCadCommand::PlaceCircle(this);

    setMouseTracking(true);
}

void MCadWidget::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    p.fillRect(e->rect(),Qt::black);
    p.setPen(Qt::green);
    m_command->paint(e,p);
    p.drawText(0,20,m_command->hint());
    //m_paint_engine->drawLine(0,0,20,20,p);
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
        update();
    }
    QWidget::mousePressEvent(e);
}
