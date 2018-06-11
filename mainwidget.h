#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QGLWidget>

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
    virtual void resizeEvent(QResizeEvent* e);

private:
    QGLWidget*   m_glwidget;
    PaintEngine* m_paint_engines[CNT_ENGINE_TYPE];
    PaintEngine* m_current_engine;
    bool         m_use_opengl;

    UserCommand* m_command;

signals:
    void displayHint(QString);
    void displaySPF(int);

public slots:
    void setEngineType(EngineType type);
};

#endif // MAINWIDGET_H
