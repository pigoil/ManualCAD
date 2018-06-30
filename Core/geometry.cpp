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

QVector3D Line3d::toVector()const
{
    return m_p2-m_p1;
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

void Entity::singlePlane(bool s)
{
    m_is_single_plane = s;
}

bool Entity::isSinglePlane()
{
    return m_is_single_plane;
}

void Entity::project(PaintEngine* engine, QPainter& painter, QVector3D view, bool blanking)
{
    for(iterator it = begin() ; it!=end() ; ++it)
    {
        if(!it->maybePlane() || m_is_single_plane)
        {
            for(Plane::iterator p = it->begin() ; p!=it->end() ; ++p)
            {
                engine->drawLine(p->toLineF(),painter);
            }
        }
        else
        {
            if(blanking)
            {
                if(it->visible(view))
                {
                    for(Plane::iterator p = it->begin() ; p!=it->end() ; ++p)
                    {
                        engine->drawLine(p->toLineF(),painter);
                    }
                }
            }
            else
            {
                for(Plane::iterator p = it->begin() ; p!=it->end() ; ++p)
                {
                    engine->drawLine(p->toLineF(),painter);
                }
            }
        }
    }
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
        for(Plane::iterator p = it->begin() ; p!=it->end() ; ++p)
        {
            *p = *p * T2;
            *p = *p * R;
            *p = *p * T1;
        }
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
        for(Plane::iterator p = it->begin() ; p!=it->end() ; ++p)
        {
            points.insert(p->p1());
            points.insert(p->p2());
        }
     }

    QVector3D res(0,0,0);
    int cnt = points.count();
    for(QSet<QVector3D>::iterator it = points.begin() ; it!=points.end() ; ++it)
    {
        res += *it;
    }

    return res/cnt;
}

//如果边数小于两条肯定不是一个平面
bool Plane::maybePlane() const
{
    if(count() < 2)return false;

    return true;
}

//根据两边确定法向量
QVector3D Plane::normal() const
{
    if(!maybePlane())return QVector3D(0,0,0);

    QVector3D v1 = at(0).toVector();
    QVector3D v2 = at(1).toVector();

    return QVector3D::normal(v1,v2);
}

bool Plane::visible(QVector3D view) const
{
    return QVector3D::dotProduct(normal(),view.normalized()) < 0;
}

void Plane::flip()
{
    if(isEmpty())return;

    for(Plane::iterator it = begin() ; it!=end() ; ++it)
    {
        QVector3D tmp = it->p1();
        it->p1() = it->p2();
        it->p2() = tmp;

    }

    std::reverse(begin(),end());
}
