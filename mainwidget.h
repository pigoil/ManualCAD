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
    virtual void wheelEvent(QWheelEvent* e);

private:
    QGLWidget*   m_glwidget;
    PaintEngine* m_paint_engines[CNT_ENGINE_TYPE];
    PaintEngine* m_current_engine;
    bool         m_use_opengl;

    GeometryTable m_buff;

    UserCommand* m_command;
    QAction*     m_command_action;

    qreal        m_scale;
    QPoint       m_scale_center;
    qreal        m_x_rotation;
    qreal        m_y_rotation;
    QPoint       m_rotate_center;
    QVector3D    m_view_vector;
    bool         m_use_blanking;

    QPointF      m_offset;
    QPoint       m_offset_center;

    int          m_selected;

    QTimer*      m_animation_timer;

    QColor       m_background_color;
    QColor       m_line_color;
    QColor       m_selected_color;

    void         raise_new_command(QAction* action);
    void         quit_current_command(QAction* action);
    void         quit_selection();
    void         toggle_selection(QPoint p, bool mulity = true);
    void         refresh_buff();
    void         reset_view();

signals:
    void displayHint(QString);
    void displaySPF(int);

public slots:
    void setEngineType(EngineType type);
    void startNewCommand(QAction* action);

private slots:
    void rotating_animation();
    void setUseBlanking(bool b);
    void setColorOption(QAction* a);
};

#endif // MAINWIDGET_H
