#ifndef MCAD_UTILS_H
#define MCAD_UTILS_H

#include <QObject>

namespace MCadUtil {

class StopWatch : public QObject
{
public:
    StopWatch();

};

qreal distance(QPointF p1,QPointF p2);

}

#endif // MCAD_UTILS_H
