#include <QLineF>
#include <QPainter>

#include "usercommand.h"
#include "mcad_utils.h"

#define DRAWING_COLOR   Qt::gray
#define CURSOR_COLOR    Qt::darkMagenta
#define POINT_COLOR     Qt::red

UserCommand::UserCommand(QObject *parent):
    QObject(parent),
    m_state(UserCommand::Idle),
    m_steps(0)
{

}

void UserCommand::draw_cursor(QPointF point, QRect region, QPainter &painter)
{
    painter.save();
    painter.setPen(CURSOR_COLOR);
    painter.drawLine(0,point.y(),region.width(),point.y());
    painter.drawLine(point.x(),0,point.x(),region.height());
    painter.setBrush(POINT_COLOR);
    painter.setPen(POINT_COLOR);
    painter.drawEllipse(point,1,1);
    painter.restore();
}

MCadCommand::PlaceLine::PlaceLine(QObject *parent):
    UserCommand(parent)
{

}

void MCadCommand::PlaceLine::proceed(const QMouseEvent *event)
{
    if(m_state == Idle)
    {
        m_state = Busy;
        m_center = event->localPos();
        m_result.points.push_back(event->localPos());//应该是图坐标
    }
    else
    {
        m_result.points.push_back(event->localPos());
        end();
    }
    ++m_steps;
}

void MCadCommand::PlaceLine::paint(const QPaintEvent* event, QPainter& painter)
{
    painter.save();
    painter.setPen(DRAWING_COLOR);
    if(m_state == Idle)
    {
        draw_cursor(cursor_pos(),event->rect(),painter);
    }
    else
    {
        painter.drawLine(cursor_pos(),m_center);
        draw_cursor(cursor_pos(),event->rect(),painter);
    }
    painter.restore();
}

QString MCadCommand::PlaceLine::hint()
{
    if(m_state == Idle)
        return QString("选取第一点");
    else
        return QString("选取第二点");
}

MCadCommand::PlaceCircle::PlaceCircle(QObject *parent):
    UserCommand(parent)
{

}

void MCadCommand::PlaceCircle::proceed(const QMouseEvent *event)
{
    if(m_state == Idle)
    {
        m_state = Busy;
        m_center = event->localPos();
        m_result.points.push_back(event->localPos());//应该是图坐标
    }
    else
    {
        if(event->modifiers() == Qt::ShiftModifier)
        {

        }
        else
        {
            m_result.figures.push_back(MCadUtil::distance(m_center,event->localPos()));//radius
            end();
        }
    }
    ++m_steps;
}

void MCadCommand::PlaceCircle::paint(const QPaintEvent* event, QPainter& painter)
{
    painter.save();
    painter.setPen(DRAWING_COLOR);
    if(m_state == Idle)
    {
        draw_cursor(cursor_pos(),event->rect(),painter);
    }
    else
    {
        qreal radius = MCadUtil::distance(m_center,cursor_pos());
        draw_cursor(cursor_pos(),event->rect(),painter);
        painter.drawEllipse(m_center,radius,radius);
    }
    painter.restore();
}

QString MCadCommand::PlaceCircle::hint()
{
    if(m_state == Idle)
        return QString("选取圆心");
    else
        return QString("确定半径");
}
