#include "paintengine.h"

PaintEngine::PaintEngine(QObject *parent) : QObject(parent)
{

}

void PaintEngine::drawPoint(const QPointF &point, QPainter &painter)
{
    drawPoint(point.x(),point.y(),painter);
}

void PaintEngine::drawPoints(const PaintEngine::PointSet &ps, QPainter &painter)
{
    foreach (QPointF p, ps)
    {
        drawPoint(p,painter);
    }
}

void PaintEngine::drawLine(const QPointF &p1, const QPointF &p2, QPainter &painter)
{
    drawLine(p1.x(),p1.y(),p2.x(),p2.y(),painter);
}

void PaintEngine::drawLine(const QLineF& line, QPainter &painter)
{
    drawLine(line.p1(),line.p2(),painter);
}

void PaintEngine::drawLines(const PaintEngine::LineSet &ls, QPainter &painter)
{
    foreach (QLineF l, ls)
    {
        drawLine(l,painter);
    }
}


QtPaintEngine::QtPaintEngine(QObject *parent):
    PaintEngine(parent)
{

}

void QtPaintEngine::drawPoint(qreal x, qreal y, QPainter &painter)
{
    painter.drawPoint(x,y);
}

void QtPaintEngine::drawPoint(const QPointF &point, QPainter &painter)
{
    painter.drawPoint(point);
}

void QtPaintEngine::drawPoints(const PaintEngine::PointSet &ps, QPainter &painter)
{
    painter.drawPoints(ps);
}

void QtPaintEngine::drawLine(qreal x1, qreal y1, qreal x2, qreal y2, QPainter &painter)
{
    painter.drawLine(x1,y1,x2,y2);
}

void QtPaintEngine::drawLine(const QPointF &p1, const QPointF &p2, QPainter &painter)
{
    painter.drawLine(p1,p2);
}

void QtPaintEngine::drawLine(const QLineF& line, QPainter &painter)
{
    painter.drawLine(line);
}

void QtPaintEngine::drawLines(const PaintEngine::LineSet &ls, QPainter &painter)
{
    painter.drawLines(ls);
}



MCadPaintEngine::MCadPaintEngine(QObject *parent):
    PaintEngine(parent)
{

}

void MCadPaintEngine::drawPoint(qreal x, qreal y, QPainter &painter)
{
    painter.drawPoint(x,y);
}

void MCadPaintEngine::drawLine(qreal x1, qreal y1, qreal x2, qreal y2, QPainter &painter)
{
    //Bresenham 直线生成算法
    qreal dx = x2 - x1;
    qreal dy = y2 - y1;
    qreal ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
    qreal uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
    qreal x = x1, y = y1;
    int eps=0;//eps为累加误差

    dx = qAbs(dx);
    dy = qAbs(dy);

    if (dx > dy)
    {
        for (x = x1; x != x2; x += ux)
        {
             drawPoint(x,y,painter);
             eps += dy;
             if ((eps << 1) >= dx)
             {
                  y += uy; eps -= dx;
             }
        }
    }
    else
    {
        for (y = y1; y != y2; y += uy)
        {
             drawPoint(x,y,painter);
             eps += dx;
             if ((eps << 1) >= dy)
             {
                  x += ux; eps -= dy;
             }
        }
    }
}
