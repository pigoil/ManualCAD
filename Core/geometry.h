#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector3D>

#include "paintengine.h"

class Line3d
{
public:
    Line3d(QVector3D p1, QVector3D p2);
    Line3d();
    QVector3D&  p1();
    QVector3D&  p2();
    QLineF      toLineF();
    QVector3D   toVector() const;
    Line3d      operator* (const QMatrix4x4& mat);
    bool        intersectWith(const QPointF &p, qreal tolerance = 10);

    friend Line3d operator* (const QMatrix4x4& mat,const Line3d& line);

private:
    QVector3D m_p1,m_p2;

};

class Plane : public QList<Line3d>
{
public:
    bool maybePlane() const;
    QVector3D normal() const;
    bool visible(QVector3D view) const;
    void flip();
};

class Entity : public QList<Plane>
{
public:
    enum Axis
    {
        X_Axis,
        Y_Axis,
        Z_Axis
    };
    void select(bool);
    bool isSelected() const;
    void singlePlane(bool);
    bool isSinglePlane();
    void project(PaintEngine *engine , QPainter &painter, QVector3D view , bool blanking);

    void rotate(Axis,float angle);

private:
    bool m_is_selected;
    bool m_is_single_plane;

    QVector3D calc_center();
};

typedef QList<Entity>  GeometryTable;

#endif // GEOMETRY_H
