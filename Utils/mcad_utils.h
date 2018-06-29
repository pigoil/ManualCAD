#ifndef MCAD_UTILS_H
#define MCAD_UTILS_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QList>
#include <QSet>
#include <chrono>

namespace MCadUtil {

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

    friend Line3d operator* (const QMatrix4x4& mat,const MCadUtil::Line3d& line);

private:
    QVector3D m_p1,m_p2;

};

//typedef QList<Line3d>  Entity;
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
