#include <QPointF>

#include "mcad_utils.h"
#include <ctime>

qreal MCadUtil::distance(QPointF p1, QPointF p2)
{
    return sqrt(pow(p2.x() - p1.x(),2) +
                pow(p2.y() - p1.y(),2)
                );
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

