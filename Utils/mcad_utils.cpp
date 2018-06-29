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


MCadUtil::Line3d::Line3d(QVector3D p1, QVector3D p2):
    m_p1(p1),
    m_p2(p2)
{

}

MCadUtil::Line3d::Line3d():
    m_p1(QVector3D()),
    m_p2(QVector3D())
{

}

QVector3D &MCadUtil::Line3d::p1()
{
    return m_p1;
}

QVector3D &MCadUtil::Line3d::p2()
{
    return m_p2;
}

QLineF MCadUtil::Line3d::toLineF()
{
    return QLineF(m_p1.toPointF(),m_p2.toPointF());
}

MCadUtil::Line3d MCadUtil::Line3d::operator*(const QMatrix4x4 &mat)
{
//    return Line3d(m_p1*mat,m_p2*mat);
    return Line3d(mat*m_p1,mat*m_p2);
}

bool MCadUtil::Line3d::intersectWith(const QPointF& p, qreal tolerance)
{
    QLineF line = toLineF();
    QRectF rect(line.p1(), line.p2());

    if(distance(p,line) < tolerance)
    {
        if(rect.contains(p))
        {
            return true;
        }
        else if(rect.height()==0)
        {
            qreal min = line.x1() < line.x2() ? line.x1() : line.x2();
            qreal max = line.x1() > line.x2() ? line.x1() : line.x2();
            if(p.x() >= min && p.x() <= max)
            {
                return true;
            }
        }
        else if(rect.width()==0)
        {
            qreal min = line.y1() < line.y2() ? line.y1() : line.y2();
            qreal max = line.y1() > line.y2() ? line.y1() : line.y2();
            if(p.y() >= min && p.y() <= max)
            {
                return true;
            }
        }
    }

    return false;
}

MCadUtil::Line3d operator*(const QMatrix4x4 &mat, MCadUtil::Line3d &line)
{
    return MCadUtil::Line3d(mat*line.p1(),mat*line.p2());
}


void MCadUtil::Entity::select(bool s)
{
    m_is_selected = s;
}

bool MCadUtil::Entity::isSelected()const
{
    return m_is_selected;
}

void MCadUtil::Entity::rotate(MCadUtil::Entity::Axis a, float angle)
{
    QVector3D center = calc_center();
    QMatrix4x4 T1,R,T2;
    T1.translate(center);
    T2.translate(-center);

    switch(a)
    {
    case X_Axis:
        R.rotate(angle,QVector3D(1,0,0));
        break;
    case Y_Axis:
        R.rotate(angle,QVector3D(0,1,0));
        break;
    case Z_Axis:
        R.rotate(angle,QVector3D(0,0,1));
        break;
    default:break;
    }

    for(iterator it = begin() ; it!=end() ; ++it)
    {
        *it = *it * T2;
        *it = *it * R;
        *it = *it * T1;
    }

}

uint qHash(const QVector3D &point)
{
    uint hash = qHash(QPair<qreal,qreal>(point.x(),point.y()));

    return qHash(QPair<uint,qreal>(hash,point.z()));
}

QVector3D MCadUtil::Entity::calc_center()
{
    QSet<QVector3D> points;

    for(iterator it = begin() ; it!=end() ; ++it)
    {
        points.insert(it->p1());
        points.insert(it->p2());
     }

    QVector3D res(0,0,0);
    int cnt = points.count();
    for(QSet<QVector3D>::iterator it = points.begin() ; it!=points.end() ; ++it)
    {
        res += *it;
    }

    return res/cnt;
}
