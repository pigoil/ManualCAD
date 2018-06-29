#include <QPointF>
#include <QLineF>
#include <QMatrix4x4>
#include <QVector3D>
#include <QList>
#include <QSet>

#include "geometry.h"
#include "mcad_utils.h"

Line3d::Line3d(QVector3D p1, QVector3D p2):
    m_p1(p1),
    m_p2(p2)
{

}

Line3d::Line3d():
    m_p1(QVector3D()),
    m_p2(QVector3D())
{

}

QVector3D &Line3d::p1()
{
    return m_p1;
}

QVector3D &Line3d::p2()
{
    return m_p2;
}

QLineF Line3d::toLineF()
{
    return QLineF(m_p1.toPointF(),m_p2.toPointF());
}

Line3d Line3d::operator*(const QMatrix4x4 &mat)
{
//    return Line3d(m_p1*mat,m_p2*mat);
    return Line3d(mat*m_p1,mat*m_p2);
}

bool Line3d::intersectWith(const QPointF& p, qreal tolerance)
{
    QLineF line = toLineF();
    QRectF rect(line.p1(), line.p2());

    if(MCadUtil::distance(p,line) < tolerance)
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

Line3d operator*(const QMatrix4x4 &mat, Line3d &line)
{
    return Line3d(mat*line.p1(),mat*line.p2());
}


void Entity::select(bool s)
{
    m_is_selected = s;
}

bool Entity::isSelected()const
{
    return m_is_selected;
}

void Entity::rotate(Entity::Axis a, float angle)
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

QVector3D Entity::calc_center()
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
