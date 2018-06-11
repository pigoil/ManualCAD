#include <QPaintEvent>
#include <QApplication>
#include <QDebug>
#include <QAction>

#include "mainwidget.h"
#include "mcad_utils.h"

MCadWidget::MCadWidget(QWidget *parent) :
    QWidget(parent),
    m_glwidget(NULL),
    m_use_opengl(false)
{
    QPixmap cursor = QPixmap(":/icons/Resource/cross.png");
    cursor.setDevicePixelRatio(5);
    setCursor(QCursor(cursor));
    m_paint_engines[Stupid] = new StupidPaintEngine(this);
    m_paint_engines[QtGUI]  = new QtPaintEngine(this);
    m_paint_engines[OpenGL] = new QtPaintEngine(this);

    setEngineType(Stupid);
    setMouseTracking(true);

    m_command = NULL;
}

void MCadWidget::paintEvent(QPaintEvent *e)
{
    MCadUtil::StopWatch watch;
    watch.start();
    QPainter p;
    if(m_use_opengl)
        p.begin(m_glwidget);
    else
        p.begin(this);

    p.fillRect(e->rect(),Qt::black);
    p.setPen(Qt::green);

    if(m_command)
        m_command->paint(e,p,m_current_engine);

    emit displaySPF(watch.tell());
}

void MCadWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_command)
    {
        m_command->move(e);
    }
    update();
}

void MCadWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(m_command && m_command->state() != UserCommand::Finished)
        {
            m_command->proceed(e);
            if(m_command->state() == UserCommand::Finished)
            {
                raise_new_command(m_command_action);
            }
            emit displayHint(m_command->hint());
        }
    }
    else if(e->button() == Qt::RightButton)
    {
        quit_current_command(m_command_action);
    }

    update();
    QWidget::mousePressEvent(e);
}

void MCadWidget::resizeEvent(QResizeEvent *e)
{
    if(m_use_opengl && m_glwidget)
        m_glwidget->setGeometry(0,0,e->size().width()-1,e->size().height()-1);

    QWidget::resizeEvent(e);
}

void MCadWidget::raise_new_command(QAction *action)
{
    if(m_command && m_command->state()!=UserCommand::Finished)
    {
        quit_current_command(m_command_action);
    }

    QString name = action->objectName();
    m_command_action = action;
    if(name == "actionPlaceLine")
    {
        if(m_command && m_command->state() != UserCommand::Finished)
        {
            delete m_command;
        }

        m_command = new MCadCommand::PlaceLine(this);
        emit displayHint(m_command->hint());
    }
    else if(name == "actionPlaceRect")
    {
        if(m_command && m_command->state() != UserCommand::Finished)
        {
            delete m_command;
        }

        m_command = new MCadCommand::PlaceRect(this);
        emit displayHint(m_command->hint());
    }
    else if(name == "actionPlaceCircle")
    {
        if(m_command && m_command->state() != UserCommand::Finished)
        {
            delete m_command;
        }

        m_command = new MCadCommand::PlaceCircle(this);
        emit displayHint(m_command->hint());
    }
    else if(name == "actionPlaceText")
    {

    }
}

void MCadWidget::quit_current_command(QAction *action)
{
    if(action)
        action->setChecked(false);
    if(m_command)
        delete m_command;

    emit displayHint("        ");
    m_command = NULL;
}

void MCadWidget::setEngineType(MCadWidget::EngineType type)
{
    if(type == OpenGL)
    {
        if(!m_glwidget)
        {
            m_glwidget = new QGLWidget(this);
            m_glwidget->setAutoFillBackground(false);
            m_glwidget->setMouseTracking(true);
            m_glwidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);
            QApplication::postEvent(this, new QResizeEvent(size(), size()));
        }
        m_use_opengl = true;
        m_glwidget->show();
        //GLWIDGET
        //
    }
    else
    {
        m_use_opengl = false;
        if(m_glwidget)
            m_glwidget->hide();
    }

    m_current_engine = m_paint_engines[type];
}

void MCadWidget::startNewCommand(QAction *action)
{
    raise_new_command(action);
}
