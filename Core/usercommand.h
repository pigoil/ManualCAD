#ifndef USERCOMMAND_H
#define USERCOMMAND_H

#include <QObject>
#include <QMouseEvent>

#include "mcad_utils.h"
#include "paintengine.h"

class UserCommand;

class UserCommand : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        Idle,
        Busy,
        Finished,
        STATE_CNT,
    };

    enum Requires
    {
        None = 0,
        AskForSelection,
        REQ_CNT,
    };

    static void                         envInit(MCadUtil::GeometryTable*);
    static MCadUtil::GeometryTable*     geoTab(){return geo_tab;}

    explicit UserCommand( QObject *parent = 0);

    State state(){return m_state;}
    int   steps(){return m_steps;}

    virtual void move(const QMouseEvent* event){m_cursor_pos = event->localPos();}
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine*) = 0;
    virtual int  proceed(const QMouseEvent* event) = 0;
    virtual void end(){m_state = Finished;}
    virtual QString hint() = 0;

    virtual void redo() = 0;
    virtual void undo() = 0;

protected:
    QPointF m_cursor_pos;
    State m_state;
    int m_steps;

    static MCadUtil::GeometryTable* geo_tab;

    QPointF cursor_pos(){return m_cursor_pos;}

signals:

public slots:
};

namespace MCadCommand {

class PlaceLine : public UserCommand
{
public:
    explicit PlaceLine(QObject *parent = 0);

    virtual int proceed(const QMouseEvent* event);
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine);
    virtual QString hint();

    virtual void redo();
    virtual void undo(){}

private:
    QPointF m_p1,m_p2;
};

class PlaceCircle : public UserCommand
{
public:
    explicit PlaceCircle(QObject *parent = 0);

    virtual int proceed(const QMouseEvent* event);
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine);
    virtual QString hint();

    virtual void redo();
    virtual void undo(){}

    void setPiece(int p);
    int  piece();

private:
    QPointF m_center;
    QPointF m_p2;
    int     m_piece;

    QPointF calc_point(QPointF center, qreal radius, qreal angle);
};

class PlaceRect : public UserCommand
{
public:
    explicit PlaceRect(QObject *parent = 0);

    virtual int proceed(const QMouseEvent* event);
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine);
    virtual QString hint();

    virtual void redo();
    virtual void undo(){}

private:
    QPointF m_p1,m_p2;
};

class Delete : public UserCommand
{
public:
    explicit Delete(QObject *parent = 0);

    virtual int proceed(const QMouseEvent* event)
    {
        Q_UNUSED(event);
        return None;
    }
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine)
    {
        Q_UNUSED(event);
        Q_UNUSED(painter);
        Q_UNUSED(engine);
    }
    virtual QString hint()
    {
        return QString("");
    }

    virtual void redo();
    virtual void undo(){}

private:
    QPointF m_p1,m_p2;
};

class Koch : public UserCommand
{
public:
    explicit Koch(QObject *parent = 0);

    virtual int proceed(const QMouseEvent* event)
    {
        Q_UNUSED(event);
        return None;
    }
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine)
    {
        Q_UNUSED(event);
        Q_UNUSED(painter);
        Q_UNUSED(engine);
    }
    virtual QString hint()
    {
        return QString("");
    }

    virtual void redo();
    virtual void undo(){}

private:
    QPointF m_p1,m_p2;

    void koch(int x1, int y1, int x2, int y2, int n, MCadUtil::Entity &e);
};

class Podetium : public UserCommand
{
public:
    explicit Podetium(QObject *parent = 0);

    virtual int proceed(const QMouseEvent* event);
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine)
    {
        Q_UNUSED(event);
        Q_UNUSED(painter);
        Q_UNUSED(engine);
    }
    virtual QString hint();

    virtual void redo();
    virtual void undo(){}

private:
    bool get_selection(MCadUtil::Entity& selected);
    MCadUtil::Entity m_src;
    qreal m_deepth;

    void create_podetium(MCadUtil::Entity poly, float z);
};

class Cone : public UserCommand
{
public:
    explicit Cone(QObject *parent = 0);

    virtual int proceed(const QMouseEvent* event);
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine)
    {
        Q_UNUSED(event);
        Q_UNUSED(painter);
        Q_UNUSED(engine);
    }
    virtual QString hint();

    virtual void redo();
    virtual void undo(){}

private:
    bool get_selection(MCadUtil::Entity& selected);
    QVector3D get_center(MCadUtil::Entity& poly);

    MCadUtil::Entity m_src;
    qreal m_deepth;

    void create_cone(MCadUtil::Entity poly, float z);
};

}

#endif // USERCOMMAND_H
