#ifndef COREPAINTENGINE_H
#define COREPAINTENGINE_H

#include <QObject>

class PaintEngine : public QObject
{
    Q_OBJECT
public:
    explicit PaintEngine(QObject *parent = 0);

signals:

public slots:
};

#endif // COREPAINTENGINE_H