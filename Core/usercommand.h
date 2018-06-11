#ifndef USERCOMMAND_H
#define USERCOMMAND_H

#include <QObject>
#include <QMouseEvent>

#include "paintengine.h"

//struct CommandResult
//{
//    QList<qreal> figures;
//    QList<QPointF> points;
//    //QList<CAD Object> objects;
//};

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

    explicit UserCommand(QObject *parent = 0);

    State state(){return m_state;}
    int   steps(){return m_steps;}
    //CommandResult result(){return m_result;}

    virtual void proceed(const QMouseEvent* event) = 0;
    virtual void move(const QMouseEvent* event){m_cursor_pos = event->localPos();}
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine*) = 0;
    virtual void end(){m_state = Finished;}
    virtual QString hint() = 0;

protected:
    QPointF m_cursor_pos;
    State m_state;
    //CommandResult m_result;
    int m_steps;

    QPointF cursor_pos(){return m_cursor_pos;}

signals:

public slots:
};

namespace MCadCommand {

class PlaceLine : public UserCommand
{
public:
    explicit PlaceLine(QObject *parent = 0);

    virtual void proceed(const QMouseEvent* event);
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine);
    virtual QString hint();

private:
    QPointF m_center;
};

class PlaceCircle : public UserCommand
{
public:
    explicit PlaceCircle(QObject *parent = 0);

    virtual void proceed(const QMouseEvent* event);
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine);
    virtual QString hint();

private:
    QPointF m_center;
};

class PlaceRect : public UserCommand
{
public:
    explicit PlaceRect(QObject *parent = 0);

    virtual void proceed(const QMouseEvent* event);
    virtual void paint(const QPaintEvent* event, QPainter& painter, PaintEngine* engine);
    virtual QString hint();

private:
    QPointF m_first_point;
};

}

#endif // USERCOMMAND_H
