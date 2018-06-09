#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "paintengine.h"
#include "usercommand.h"

class MCadWidget : public QWidget
{
    Q_OBJECT
public:
    enum EngineType
    {
        Stupid = 0,
        QtGUI,
        OpenGL,
        CNT_ENGINE_TYPE,
    };

    explicit MCadWidget(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    //virtual bool event(QEvent *event){}

private:
    PaintEngine* m_paint_engines[CNT_ENGINE_TYPE];
    PaintEngine* m_current_engine;
    UserCommand* m_command;
    bool         m_use_opengl;

signals:
    void displayHint(QString);
    void displaySPF(QString);

public slots:
    void setEngineType(EngineType type);
};

#endif // MAINWIDGET_H
