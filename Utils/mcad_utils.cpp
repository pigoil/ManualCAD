#include <QPointF>
#include <QLineF>
#include <ctime>
#include <QDebug>

#include "mcad_utils.h"

qreal MCadUtil::distance(QPointF p1, QPointF p2)
{
    return sqrt(pow(p2.x() - p1.x(),2) +
                pow(p2.y() - p1.y(),2)
                );
}

qreal MCadUtil::distance(QPointF p, QLineF line)
{
    qreal x0 = p.x();
    qreal y0 = p.y();
    qreal x1 = line.x1();
    qreal y1 = line.y1();
    qreal x2 = line.x2();
    qreal y2 = line.y2();
    return (qAbs((y2 - y1) * x0 +(x1 - x2) * y0 + ((x2 * y1) -(x1 * y2)))) / (sqrt(pow(y2 - y1, 2) + pow(x1 - x2, 2)));
}

MCadUtil::StopWatch::StopWatch()
{

}

void MCadUtil::StopWatch::start()
{
    m_start = std::chrono::system_clock::now();
}

qint64 MCadUtil::StopWatch::tell()
{
    std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                (std::chrono::system_clock::now() - m_start);
    return duration.count();
}
