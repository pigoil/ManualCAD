#include <QPaintEvent>
#include <QApplication>
#include <QDebug>
#include <QAction>
#include <QTimer>
#include <QMatrix4x4>
#include <QColorDialog>

#include "mainwidget.h"
#include "mcad_utils.h"

MCadWidget::MCadWidget(QWidget *parent) :
    QWidget(parent),
    m_glwidget(NULL),
    m_use_opengl(false),
    m_scale(1.0),
    m_x_rotation(0),
    m_y_rotation(0),
    m_view_vector(0,0,-1),
    m_use_blanking(false),
    m_offset(0,0),
    m_selected(0)
{
    UserCommand::envInit(new GeometryTable); //建立真实图形表，初始化用户命令环境

    QPixmap cursor = QPixmap(":/icons/Resource/cross.png");
    cursor.setDevicePixelRatio(5); //指针的图案太大，设置缩小五倍
    setCursor(QCursor(cursor));//设置鼠标指针

    m_paint_engines[Stupid] = new StupidPaintEngine(this);//初始化三个引擎，实时切换
    m_paint_engines[QtGUI]  = new QtPaintEngine(this);
    m_paint_engines[OpenGL] = new QtPaintEngine(this);

    setEngineType(Stupid);//默认引擎为Stupid
    setMouseTracking(true);//鼠标追踪

    m_command = NULL;
    m_command_action = NULL;
    m_animation_timer = new QTimer(this);
    connect(m_animation_timer,SIGNAL(timeout()),this,SLOT(rotating_animation()));

    m_background_color = Qt::black;
    m_selected_color   = Qt::yellow;
    m_line_color       = Qt::green;
}

//绘制事件
void MCadWidget::paintEvent(QPaintEvent *e)
{
    MCadUtil::StopWatch watch;
    watch.start();
    QPainter p;
    if(m_use_opengl)//根据所选引擎决定画布是否OpenGL画布
        p.begin(m_glwidget);
    else
        p.begin(this);

    p.fillRect(e->rect(),m_background_color);//填充背景

    if(m_command)
        m_command->paint(e,p,m_current_engine);//绘制当前命令

    if(m_use_opengl)
    {
        p.translate(m_scale_center);
        p.scale(m_scale,m_scale);
        p.translate(-m_scale_center);
        p.translate(m_offset);
    }
    GeometryTable& buff = m_use_opengl ? *UserCommand::geoTab() : m_buff;
    foreach(Entity entity,buff)
    {
        if(entity.isSelected())//如果选中，画蓝色
        {
            p.setPen(m_selected_color);
        }
        else
        {
            p.setPen(m_line_color);
        }
        entity.project(m_current_engine,p,m_view_vector,m_use_blanking);
    }

    emit displaySPF(watch.tell());//计算绘制单帧时间
}

//鼠标移动时间
void MCadWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_command)
    {
        m_command->move(e);//通知命令移动的坐标
    }

    if(e->buttons() & Qt::LeftButton)
    {
    }
    if(e->buttons() & Qt::MiddleButton)
    {
        QPoint delta = e->pos() - m_rotate_center;
        if(!m_command || (m_command && m_command->state()==UserCommand::Finished))
        {
            m_x_rotation += (qreal)delta.x()/10;
            m_y_rotation -= (qreal)delta.y()/10;

            if(m_x_rotation > 90)m_x_rotation=90;
            if(m_y_rotation > 90)m_y_rotation=90;
            if(m_x_rotation < -90)m_x_rotation=-90;
            if(m_x_rotation < -90)m_x_rotation=-90;
            m_rotate_center = e->pos();
            refresh_buff();
        }
    }

    if(e->buttons() & Qt::RightButton)
    {
        QPoint delta = e->pos() - m_offset_center;
        if(!m_command || (m_command && m_command->state()==UserCommand::Finished))
        {
            m_offset += delta;
            m_offset_center = e->pos();
            refresh_buff();
        }
    }
    update();

}

//鼠标按下事件
void MCadWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)//左键
    {
        if(m_command && m_command->state() != UserCommand::Finished)//如果当前命令还未完成
        {
            if(m_command->proceed(e) == UserCommand::AskForSelection)//步进命令
            {
                toggle_selection(e->pos(),false);
            }
            if(m_command->state() == UserCommand::Finished)//如果完成
            {
                raise_new_command(m_command_action);//产生同样的命令
            }
            emit displayHint(m_command->hint());//显示命令提示
        }
        else
        {
            toggle_selection(e->pos());
        }
    }
    else if(e->button() == Qt::RightButton)//右键
    {
        quit_current_command(m_command_action);//退出命令
        quit_selection();//清楚选择
        m_offset_center =  e->pos();
    }
    else if(e->button() == Qt::MiddleButton)
    {
        m_rotate_center = e->pos();
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

//滚轮事件
void MCadWidget::wheelEvent(QWheelEvent *e)
{
    if(m_command && m_command->state()!=UserCommand::Finished)return;//命令执行过程中禁止缩放
    qreal cur_scale = m_scale;
    cur_scale += e->delta()/1000.0;

    if(cur_scale<0 || cur_scale >5)return;
    m_scale = cur_scale;
    m_scale_center = e->pos();
    if(!m_use_opengl)
    {
        refresh_buff();
    }
    update();
}

//创建新的命令
void MCadWidget::raise_new_command(QAction *action)
{
    reset_view();
    //如果还未完成，退出当前的命令
    if(m_command && m_command->state()!=UserCommand::Finished)
    {
        quit_current_command(m_command_action);
    }

    QString name = action->objectName();
    m_command_action = action;

    //除了需要选择的命令外，消除选择
    if(name != "actionDelete")
    {
       quit_selection();
    }

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
    else if(name == "actionDelete")
    {
        if(m_command && m_command->state() != UserCommand::Finished)
        {
            delete m_command;
        }

        if(m_selected)
            m_command = new MCadCommand::Delete(this);
        m_selected = 0;
    }
    else if(name == "actionKoch")
    {
        if(m_command && m_command->state() != UserCommand::Finished)
        {
            delete m_command;
        }

        m_command = new MCadCommand::Koch(this);
    }
    else if(name == "actionPodetium")
    {
        if(m_command && m_command->state() != UserCommand::Finished)
        {
            delete m_command;
        }

        m_command = new MCadCommand::Podetium(this);
        emit displayHint(m_command->hint());
    }
    else if(name == "actionCone")
    {
        if(m_command && m_command->state() != UserCommand::Finished)
        {
            delete m_command;
        }

        m_command = new MCadCommand::Cone(this);
        emit displayHint(m_command->hint());
    }
    else if(name == "actionAnimation")
    {
        static bool is_checked = false;

        is_checked = !is_checked;
        if(is_checked)
        {
            m_animation_timer->start(100);
        }
        else
        {
            action->setChecked(false);
            m_animation_timer->stop();
        }
        emit displayHint("选择实体开始动画");
    }

    refresh_buff();
    update();
}

//退出当前命令
void MCadWidget::quit_current_command(QAction *action)
{
    if(action)
        action->setChecked(false);
    if(m_command)
        delete m_command;
    if(m_animation_timer->isActive())
    {
        m_animation_timer->stop();
    }

    emit displayHint("        ");
    m_command = NULL;
}

//消除选择
void MCadWidget::quit_selection()
{
    for(int i=0; i<m_buff.count(); ++i)
    {
        m_buff[i].select(false);
        (*UserCommand::geoTab())[i].select(false);
    }
    m_selected = 0;
}

void MCadWidget::toggle_selection(QPoint p, bool mulity)
{
    if(UserCommand::geoTab()->isEmpty())return;

    GeometryTable::iterator itG = UserCommand::geoTab()->begin();
    Entity::iterator eG = itG->begin();
    Plane::iterator plG = eG->begin();

    for(GeometryTable::iterator it = m_buff.begin() ; it!=m_buff.end() ; ++it)
    {
        for(Entity::iterator e = it->begin() ; e!=it->end() ; ++e)
        {
            for(Plane::iterator pl=e->begin() ; pl!=e->end() ; ++pl)
            {
                if(pl->intersectWith(p))
                {
                    ++m_selected;
                    it->select(true);
                    itG->select(true);
                    if(mulity)
                        break;
                    else
                        return;
                }
                ++plG;
            }
            ++eG;
        }
        ++itG;
    }
}

//更新显示表
void MCadWidget::refresh_buff()
{
    m_buff = *UserCommand::geoTab();

    for(GeometryTable::iterator it=m_buff.begin() ; it!=m_buff.end() ; ++it)
    {
        for(Entity::iterator e = it->begin() ; e!=it->end() ; ++e)
        {
            for(Plane::iterator p = e->begin() ; p!=e->end() ; ++p)
            {
                Line3d& line = *p;
                QMatrix4x4 T;
                T.translate(QVector3D(-m_scale_center));
                line = line*T;
                T = QMatrix4x4();
                T.scale(m_scale);
                line = line*T;
                T = QMatrix4x4();
                T.translate(QVector3D(m_scale_center));
                T.translate(QVector3D(m_offset));
                T.rotate(m_x_rotation,QVector3D(0,1,0));
                T.rotate(m_y_rotation,QVector3D(1,0,0));
                line = line*T;
            }
        }
    }
}

void MCadWidget::reset_view()
{
    m_x_rotation = 0;
    m_y_rotation = 0;
    m_scale = 1;
    m_offset = QPointF(0,0);
    refresh_buff();
}

//设置绘图引擎
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
        refresh_buff();
    }

    m_current_engine = m_paint_engines[type];
}

//创建新的命令
void MCadWidget::startNewCommand(QAction *action)
{
    raise_new_command(action);
}

void MCadWidget::rotating_animation()
{
    for(GeometryTable::iterator it = UserCommand::geoTab()->begin() ; it!=UserCommand::geoTab()->end() ; ++it)
    {
        if(it->isSelected())
        {
            it->rotate(Entity::Y_Axis,5);
            it->rotate(Entity::X_Axis,5);
        }
    }

    refresh_buff();
    update();
}

void MCadWidget::setUseBlanking(bool b)
{
    m_use_blanking = b;
    update();
}

void MCadWidget::setColorOption(QAction *a)
{
    QColorDialog dlg;
    if(a->objectName() == "actionLineColor")
    {
        dlg.setWindowTitle("选择线条颜色");
        dlg.setCurrentColor(m_line_color);
        if(dlg.exec())
        {
            m_line_color = dlg.currentColor();
        }
    }
    else if(a->objectName() == "actionBackgroundColor")
    {
        dlg.setWindowTitle("选择背景颜色");
        dlg.setCurrentColor(m_background_color);
        if(dlg.exec())
        {
            m_background_color = dlg.currentColor();
        }
    }
    else if(a->objectName() == "actionSelectedColor")
    {
        dlg.setWindowTitle("选择选中线条颜色");
        dlg.setCurrentColor(m_selected_color);
        if(dlg.exec())
        {
            m_selected_color = dlg.currentColor();
        }
    }

    update();
}
