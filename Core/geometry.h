#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector3D>

class Line3d
{
public:
    Line3d(QVector3D p1, QVector3D p2);
    Line3d();
    QVector3D&  p1();
    QVector3D&  p2();
    QLineF      toLineF();
    Line3d      operator* (const QMatrix4x4& mat);
    bool        intersectWith(const QPointF &p, qreal tolerance = 10);

    friend Line3d operator* (const QMatrix4x4& mat,const Line3d& line);

private:
    QVector3D m_p1,m_p2;

};

class Entity : public QList<Line3d>
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

    void rotate(Axis,float angle);

private:
    bool m_is_selected;
    QVector3D calc_center();
};

typedef QList<Entity>  GeometryTable;

#endif // GEOMETRY_H
