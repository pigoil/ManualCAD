#ifndef COREPAINTENGINE_H
#define COREPAINTENGINE_H

#include <QObject>
#include <QPainter>

/**
 * @brief 绘制引擎基类
 */
class PaintEngine : public QObject
{
    Q_OBJECT
public:
    typedef QVector<QPointF> PointSet;
    typedef QVector<QLineF> LineSet;

    explicit PaintEngine(QObject *parent = 0);

    virtual void drawPoint(qreal x, qreal y, QPainter& painter) = 0;
    virtual void drawPoint(const QPointF& point,QPainter& painter);
    virtual void drawPoints(const PointSet& ps, QPainter& painter);

    virtual void drawLine(qreal x1, qreal y1, qreal x2, qreal y2, QPainter& painter) = 0;
    virtual void drawLine(const QPointF& p1, const QPointF& p2, QPainter& painter);
    virtual void drawLine(const QLineF& line, QPainter& painter);
    virtual void drawLines(const LineSet& ls, QPainter& painter);

    virtual void drawRect(const QRectF &rectangle, QPainter& painter);
    virtual void drawEllipse(const QPointF &center, qreal rx, qreal ry, QPainter& painter);
    virtual void drawCursor(const QPointF &cursor, const QRect& rect, QPainter& painter);

signals:

public slots:
};

/**
 * @brief 使用Qt GUI内嵌的图形绘制API，CPU绘图,也可使用OpenGL调用GPU绘图
 */
class QtPaintEngine : public PaintEngine
{
public:

    explicit QtPaintEngine(QObject* parent = 0);

    virtual void drawPoint(qreal x, qreal y, QPainter& painter);
    virtual void drawPoint(const QPointF& point,QPainter& painter);
    virtual void drawPoints(const PointSet& ps, QPainter& painter);

    virtual void drawLine(qreal x1, qreal y1, qreal x2, qreal y2, QPainter& painter);
    virtual void drawLine(const QPointF& p1, const QPointF& p2, QPainter& painter);
    virtual void drawLine(const QLineF &line, QPainter& painter);
    virtual void drawLines(const LineSet& ls, QPainter& painter);

    virtual void drawRect(const QRectF &rectangle, QPainter& painter);
    virtual void drawEllipse(const QPointF &center, qreal rx, qreal ry, QPainter& painter);
};

/**
 * @brief 只使用Qt图形绘制API的画点API，CPU绘制
 */
class StupidPaintEngine : public PaintEngine
{
public:
    explicit StupidPaintEngine(QObject* parent = 0);

    virtual void drawPoint(qreal x, qreal y, QPainter& painter);
    virtual void drawLine(qreal x1, qreal y1, qreal x2, qreal y2, QPainter& painter);

};

#endif // COREPAINTENGINE_H
