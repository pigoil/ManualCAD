#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "paintengine.h"
#include "usercommand.h"

class MCadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MCadWidget(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    //virtual bool event(QEvent *event){}

private:
    PaintEngine* m_paint_engine;
    UserCommand* m_command;

signals:

public slots:
};

#endif // MAINWIDGET_H
