#include <QPointF>

#include "mcad_utils.h"

qreal MCadUtil::distance(QPointF p1, QPointF p2)
{
    return sqrt(pow(p2.x() - p1.x(),2) +
                pow(p2.y() - p1.y(),2)
                );
}
