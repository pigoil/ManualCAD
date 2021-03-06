#include <QLineF>
#include <QPainter>
#include <QDebug>

#include "usercommand.h"
#include "mcad_utils.h"
#include "parameterdialog.h"

#define DRAWING_COLOR   Qt::gray
#define CURSOR_COLOR    Qt::darkMagenta
#define POINT_COLOR     Qt::red

GeometryTable* UserCommand::geo_tab = NULL;

void UserCommand::envInit(GeometryTable *t)
{
    geo_tab = t;
}

UserCommand::UserCommand(QObject *parent):
    QObject(parent),
    m_state(UserCommand::Idle),
    m_steps(0)
{
    if(geo_tab == NULL)
        throw QString("Geometry env is not initialized.");
}

MCadCommand::PlaceLine::PlaceLine(QObject *parent):
    UserCommand(parent)
{

}

//创建直线的步骤
int MCadCommand::PlaceLine::proceed(const QMouseEvent *event)
{
    if(m_state == Idle)
    {
        m_state = Busy;
        m_p1 = event->localPos();
    }
    else if(m_state == Busy)
    {
        m_p2 = event->localPos();
        redo();
        end();
    }
    ++m_steps;

    return None;
}

//创建直线的绘图函数
void MCadCommand::PlaceLine::paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine)
{
    painter.save();
    painter.setPen(DRAWING_COLOR);
    if(m_state == Idle)
    {
        painter.save();
        painter.setPen(Qt::darkMagenta);
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        painter.restore();
    }
    else if(m_state == Busy)
    {
        engine->drawLine(cursor_pos(),m_p1,painter);
        engine->drawCursor(cursor_pos(),event->rect(),painter);
    }
    painter.restore();
}

//创建直线的提示
QString MCadCommand::PlaceLine::hint()
{
    if(m_state == Idle)
        return QString("选取第一点");
    else
        return QString("选取第二点");
}

//在表中添加直线，因为do是关键字，所以使用redo
void MCadCommand::PlaceLine::redo()
{
    if(MCadUtil::distance(m_p1,m_p2) == 0)return;

    QVector3D p1(m_p1.x(), m_p1.y(), 0);
    QVector3D p2(m_p2.x(), m_p2.y(), 0);

    Entity entity;
    Plane  plane;

    Line3d line(p1,p2);
    plane.append(line);

    entity.append(plane);
    entity.select(false);
    entity.singlePlane(true);
    geo_tab->append(entity);
}

MCadCommand::PlaceCircle::PlaceCircle(QObject *parent):
    UserCommand(parent),
    m_piece(24)
{

}

int MCadCommand::PlaceCircle::proceed(const QMouseEvent *event)
{
    if(m_state == Idle)
    {
        m_state = Busy;
        m_center = event->localPos();
    }
    else if(m_state == Busy)
    {
        if(event->modifiers() == Qt::ShiftModifier)
        {

        }
        else
        {
            m_p2 = event->localPos();
            redo();
            end();
        }
    }
    ++m_steps;

    return None;
}

void MCadCommand::PlaceCircle::paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine)
{
    painter.save();
    painter.setPen(DRAWING_COLOR);
    if(m_state == Idle)
    {
        painter.save();
        painter.setPen(Qt::darkMagenta);
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        painter.restore();
    }
    else if(m_state == Busy)
    {
        qreal radius = MCadUtil::distance(m_center,cursor_pos());
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        engine->drawEllipse(m_center,radius,radius,painter);
    }
    painter.restore();
}

QString MCadCommand::PlaceCircle::hint()
{
    if(m_state == Idle)
        return QString("选取圆心");
    else
        return QString("确定半径");
}

void MCadCommand::PlaceCircle::redo()
{
    if(MCadUtil::distance(m_center,m_p2) == 0)return;

    qreal radius = MCadUtil::distance(m_center,m_p2);

    Entity entity;
    Plane  plane;

    QPointF point;
    qreal step = 360.0/m_piece;
    for(int i=0; i<m_piece; ++i)
    {
        point = calc_point(m_center,radius,step*i/180*3.141592654);
        QVector3D p1 = QVector3D(point);
        point = calc_point(m_center,radius,step*(i+1)/180*3.141592654);
        QVector3D p2 = QVector3D(point);
        Line3d line(p2,p1);
        plane.append(line);
    }

    entity.append(plane);
    entity.singlePlane(true);
    entity.select(false);
    geo_tab->append(entity);
}

void MCadCommand::PlaceCircle::setPiece(int p)
{
    if(p < 3)return;
    m_piece = p;
}

int MCadCommand::PlaceCircle::piece()
{
    return m_piece;
}

QPointF MCadCommand::PlaceCircle::calc_point(QPointF center, qreal radius, qreal angle)
{
    QPointF point = QPointF(radius * cos(angle) + center.x(), radius * sin(angle) + center.y());

    return point;
}

MCadCommand::PlaceRect::PlaceRect(QObject *parent):
    UserCommand(parent)
{

}

int MCadCommand::PlaceRect::proceed(const QMouseEvent *event)
{
    if(m_state == Idle)
    {
        m_state = Busy;
        m_p1 = event->localPos();
    }
    else if(m_state == Busy)
    {
        m_p2 = event->localPos();
        redo();
        end();
    }
    ++m_steps;

    return None;
}

void MCadCommand::PlaceRect::paint(const QPaintEvent *event, QPainter &painter, PaintEngine *engine)
{
    painter.save();
    painter.setPen(DRAWING_COLOR);
    if(m_state == Idle)
    {
        painter.save();
        painter.setPen(Qt::darkMagenta);
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        painter.restore();
    }
    else if(m_state == Busy)
    {
        engine->drawCursor(cursor_pos(),event->rect(),painter);
        engine->drawRect(QRectF(m_p1,cursor_pos()),painter);
    }
    painter.restore();
}

QString MCadCommand::PlaceRect::hint()
{
    if(m_state == Idle)
        return QString("选取第一角点");
    else
        return QString("确定第二角点");
}

void MCadCommand::PlaceRect::redo()
{
    if(MCadUtil::distance(m_p1,m_p2) == 0)return;

    QVector3D p1(m_p1.x(), m_p1.y(), 0);
    QVector3D p2(m_p2.x(), m_p2.y(), 0);
    QVector3D p3(m_p1.x(), m_p2.y(), 0);
    QVector3D p4(m_p2.x(), m_p1.y(), 0);

    Entity entity;
    Plane  plane;

    Line3d l1(p4,p1);
    Line3d l2(p2,p4);
    Line3d l3(p3,p2);
    Line3d l4(p1,p3);

    plane.append(l1);
    plane.append(l2);
    plane.append(l3);
    plane.append(l4);

    entity.append(plane);
    entity.select(false);
    entity.singlePlane(true);
    geo_tab->append(entity);
}

MCadCommand::Delete::Delete(QObject *parent):
    UserCommand(parent)
{
    redo();
    end();
}

void MCadCommand::Delete::redo()
{
    for(GeometryTable::iterator i = geo_tab->begin(); i!=geo_tab->end();)
    {
        if((*i).isSelected())
        {
            i = geo_tab->erase(i);
        }
        else
            ++i;
    }

}

MCadCommand::Koch::Koch(QObject *parent):
    UserCommand(parent)
{
    redo();
    end();
}

void MCadCommand::Koch::redo()
{
    Entity entity;
    Plane  plane;

    koch(0,0,600,600,8,plane);

    entity.append(plane);
    entity.singlePlane(true);
    entity.select(false);
    geo_tab->append(entity);
}

void MCadCommand::Koch::koch(int x1, int y1, int x2, int y2, int n, Plane &e)
{
    if (n > 0)
    {
        double k = 1.0*(y2 - y1) / (x2 - x1);
        int x3 = (x2 - x1) / 3;
        int y3 = (y2 - y1) / 3;

        koch(x1, y1, x1 + x3, y1 + y3, n - 1,e);
        koch(x2 - x3, y2 - y3, x2, y2, n - 1,e);
        if (abs(k) <= 1e-6)
        {
            koch(x1 + x3, y1, x1 + x3 + x3 / 2, y1 + sqrt(3.0) / 2 * x3, n - 1,e);
            koch(x1 + x3 + x3 / 2, y1 + sqrt(3.0) / 2 * x3, x2 - x3, y1, n - 1,e);
        }
        else if (k > 0)
        {
            koch(x1, y2 - y3, x2 - x3, y2 - y3, n - 1,e);
            koch(x1 + x3, y1 + y3, x1, y2 - y3, n - 1,e);
        }
        else
        {
            koch(x1 + x3, y1 + y3, x2, y1 + y3, n - 1,e);
            koch(x2, y1 + y3, x2 - x3, y2 - y3, n - 1,e);
        }
    }
    else
    {
        QVector3D p1(x1,y1,0);
        QVector3D p2(x2,y2,0);
        Line3d line(p1,p2);
        e.append(line);
    }
}

MCadCommand::Podetium::Podetium(QObject *parent):
    UserCommand(parent)
{

}

int MCadCommand::Podetium::proceed(const QMouseEvent *event)
{
    Q_UNUSED(event);

    if(m_state == Idle)
    {
        m_state = Busy;
        m_steps++;
        return AskForSelection;
    }
    else if(m_state == Busy)
    {
        if(get_selection(m_src))
        {
            ParameterDialog dlg;
            dlg.setWindowTitle("确定高度");
            dlg.exec();
            m_deepth = dlg.value();
            redo();
            end();
            return None;
        }

        return AskForSelection;
    }
    ++m_steps;

    return None;
}

QString MCadCommand::Podetium::hint()
{
    if(m_state == Idle)
        return QString("选取底面");
    if(m_state == Busy)
        return QString("点击空白处确定");

    return QString("");
}

void MCadCommand::Podetium::redo()
{
    create_podetium(m_src,m_deepth);
}

bool MCadCommand::Podetium::get_selection(Entity &selected)
{
    foreach (Entity entity, *geo_tab)
    {
        if(entity.isSelected())
        {
            selected = entity;
            return true;
        }
    }

    return false;
}

void MCadCommand::Podetium::create_podetium(Entity poly, float z)
{
    if(!poly.isSinglePlane())return;

    Entity entity;
    Plane  top = poly.at(0);
    Plane  bottom = top;
    Plane  plane;

    entity.append(top);

    for(Plane::iterator it = bottom.begin() ; it!=bottom.end() ; ++it)
    {
        it->p1().setZ(-z);
        it->p2().setZ(-z);
    }

    bottom.flip();
    entity.append(bottom);

    for(int i=0 ; i<bottom.count() ; ++i)
    {
        plane.clear();

        Line3d t_edge = top.at(i);
        Line3d b_edge = bottom.at(bottom.count() - i -1);
        plane.append(t_edge);
        plane.append(Line3d(t_edge.p2(),b_edge.p1()));
        plane.append(b_edge);
        plane.append(Line3d(b_edge.p2(),t_edge.p1()));
        //plane.flip();
        entity.append(plane);
    }

    entity.select(false);
    entity.singlePlane(false);
    geo_tab->append(entity);
}

MCadCommand::Cone::Cone(QObject *parent):
    UserCommand(parent)
{

}

int MCadCommand::Cone::proceed(const QMouseEvent *event)
{
    Q_UNUSED(event);

    if(m_state == Idle)
    {
        m_state = Busy;
        m_steps++;
        return AskForSelection;
    }
    else if(m_state == Busy)
    {
        if(get_selection(m_src))
        {
            ParameterDialog dlg;
            dlg.setWindowTitle("确定高度");
            dlg.exec();
            m_deepth = dlg.value();
            redo();
            end();
            return None;
        }

        return AskForSelection;
    }
    ++m_steps;

    return None;
}

QString MCadCommand::Cone::hint()
{
    if(m_state == Idle)
        return QString("选取底面");
    if(m_state == Busy)
        return QString("点击空白处确定");

    return QString("");
}

void MCadCommand::Cone::redo()
{
    create_cone(m_src,m_deepth);
}

bool MCadCommand::Cone::get_selection(Entity &selected)
{
    foreach (Entity entity, *geo_tab)
    {
        if(entity.isSelected())
        {
            selected = entity;
            return true;
        }
    }

    return false;
}

QVector3D MCadCommand::Cone::get_center(Plane &poly)
{
    int edges = poly.count();

    QVector3D res(0,0,0);

    for(Plane::iterator it=poly.begin() ; it!=poly.end() ; ++it)
    {
        res += it->p1();
    }

    return res/edges;
}

void MCadCommand::Cone::create_cone(Entity poly, float z)
{
    if(!poly.isSinglePlane())return;

    Entity entity;
    Plane  bottom = poly.at(0);
    Plane  plane;
    QVector3D center = get_center(bottom);
    center.setZ(-z);

    entity.append(bottom);

    for(Plane::iterator it = bottom.begin() ; it!=bottom.end() ; ++it)
    {
        plane.clear();
        QVector3D p1 = it->p1();
        QVector3D p2 = it->p2();
        Line3d edge1(p2,center);
        Line3d edge2(center,p1);
        Line3d edge3(p1,p2);
        plane.append(edge1);
        plane.append(edge2);
        plane.append(edge3);
        //plane.flip();
        entity.append(plane);
    }

    entity.select(false);
    entity.singlePlane(false);
    geo_tab->append(entity);
}
