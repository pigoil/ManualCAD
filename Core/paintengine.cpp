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

void PaintEngine::drawRect(const QRectF &rect, QPainter &painter)
{
    drawLine(rect.topLeft(),rect.topRight(),painter);
    drawLine(rect.topRight(),rect.bottomRight(),painter);
    drawLine(rect.bottomRight(),rect.bottomLeft(),painter);
    drawLine(rect.bottomLeft(),rect.topLeft(),painter);
}

void PaintEngine::drawEllipse(const QPointF &center, qreal rx, qreal ry, QPainter &painter)
{
    if(rx <= 0 || ry <=0)return;

    qreal a = rx;
    qreal b = ry;
    qreal d = b*b + a*a*(-b + 0.25);
    int x = 0, y = b;
    int fx = a*a / sqrt((float)a*a + b*b);

    while (x != fx)
    {
         if (d < 0)
            d += b*b*(2 * x + 3);
        else
        {
            --y;
            d += b*b*(2 * x + 3) + a*a*(-2 * y + 2);
        }
        ++x;
        drawPoint(center.x() + x, center.y() + y, painter);
        drawPoint(center.x() - x, center.y() + y, painter);
        drawPoint(center.x() + x, center.y() - y, painter);
        drawPoint(center.x() - x, center.y() - y, painter);
    }
    //中点改为(x+0.5,y-1)所以d =
    d = b*b*(x + 0.5)*(x + 0.5) + a*a*(y - 1)*(y - 1) - a*a*b*b;
    while (y > 0)
    {
        if (d < 0)
        {
            ++x;
            d += b*b*(2 * x + 2) + a*a*(-2 * y + 3);
        }
        else
            d += a*a*(-2 * y + 3);
        --y;
        drawPoint(center.x() + x, center.y() + y, painter);
        drawPoint(center.x() - x, center.y() + y, painter);
        drawPoint(center.x() + x, center.y() - y, painter);
        drawPoint(center.x() - x, center.y() - y, painter);
    }
}

void PaintEngine::drawCursor(const QPointF &cursor, const QRect &rect, QPainter &painter)
{
    drawLine(0,cursor.y(),rect.width(),cursor.y(),painter);
    drawLine(cursor.x(),0,cursor.x(),rect.height(),painter);
    drawEllipse(cursor,1,1,painter);
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

void QtPaintEngine::drawRect(const QRectF &rect, QPainter &painter)
{
    painter.drawRect(rect);
}

void QtPaintEngine::drawEllipse(const QPointF &center, qreal rx, qreal ry, QPainter &painter)
{
    painter.drawEllipse(center,rx,ry);
}



StupidPaintEngine::StupidPaintEngine(QObject *parent):
    PaintEngine(parent)
{

}

void StupidPaintEngine::drawPoint(qreal x, qreal y, QPainter &painter)
{
    painter.drawPoint(x,y);
}

void StupidPaintEngine::drawLine(qreal x1, qreal y1, qreal x2, qreal y2, QPainter &painter)
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
