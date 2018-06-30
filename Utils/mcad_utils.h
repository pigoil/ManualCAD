#ifndef MCAD_UTILS_H
#define MCAD_UTILS_H

#include <QObject>
#include <chrono>

namespace MCadUtil {

class StopWatch : public QObject
{
public:
    StopWatch();
    void start();
    qint64 tell();

private:
    typedef std::chrono::system_clock::time_point TimeStruct;
    TimeStruct m_start;
};

qreal distance(QPointF p1,QPointF p2);
qreal distance(QPointF p, QLineF line);

}

#endif // MCAD_UTILS_H
