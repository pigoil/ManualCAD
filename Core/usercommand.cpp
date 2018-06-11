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
        //m_result.points.push_back(event->localPos());//应该是图坐标
    }
    else if(m_state == Busy)
    {
        //m_result.points.push_back(event->localPos());
        end();
    }
    ++m_steps;
}

void MCadCommand::PlaceLine::paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine)
{
    painter.save();
    painter.setPen(DRAWING_COLOR);
    if(m_state == Idle)
    {
        painter.save();
        painter.setPen(Qt::darkMagenta);
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        painter.restore();
    }
    else if(m_state == Busy)
    {
        engine->drawLine(cursor_pos(),m_center,painter);
        engine->drawCursor(cursor_pos(),event->rect(),painter);
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
        //m_result.points.push_back(event->localPos());//应该是图坐标
    }
    else if(m_state == Busy)
    {
        if(event->modifiers() == Qt::ShiftModifier)
        {

        }
        else
        {
            //m_result.figures.push_back(MCadUtil::distance(m_center,event->localPos()));//radius
            end();
        }
    }
    ++m_steps;
}

void MCadCommand::PlaceCircle::paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine)
{
    painter.save();
    painter.setPen(DRAWING_COLOR);
    if(m_state == Idle)
    {
        painter.save();
        painter.setPen(Qt::darkMagenta);
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        painter.restore();
    }
    else if(m_state == Busy)
    {
        qreal radius = MCadUtil::distance(m_center,cursor_pos());
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        engine->drawEllipse(m_center,radius,radius,painter);
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

MCadCommand::PlaceRect::PlaceRect(QObject *parent):
    UserCommand(parent)
{

}

void MCadCommand::PlaceRect::proceed(const QMouseEvent *event)
{
    if(m_state == Idle)
    {
        m_state = Busy;
        m_first_point = event->localPos();
        //m_result.points.push_back(event->localPos());//应该是图坐标
    }
    else if(m_state == Busy)
    {
        if(event->modifiers() == Qt::ShiftModifier)
        {

        }
        else
        {
            end();
        }
    }
    ++m_steps;
}

void MCadCommand::PlaceRect::paint(const QPaintEvent *event, QPainter &painter, PaintEngine *engine)
{
    painter.save();
    painter.setPen(DRAWING_COLOR);
    if(m_state == Idle)
    {
        painter.save();
        painter.setPen(Qt::darkMagenta);
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        painter.restore();
    }
    else if(m_state == Busy)
    {
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        engine->drawRect(QRectF(m_first_point,cursor_pos()),painter);
    }
    painter.restore();
}

QString MCadCommand::PlaceRect::hint()
{
    if(m_state == Idle)
        return QString("选取第一角点");
    else
        return QString("确定第二角点");
}
