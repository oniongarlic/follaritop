#ifndef FOLLARIDISPLAY_H
#define FOLLARIDISPLAY_H

#include <QObject>

#include "cutefollari.h"

class FollariDisplay : public QObject
{
    Q_OBJECT
public:
    explicit FollariDisplay(CuteFollari *cf, QObject *parent = nullptr);

signals:

public slots:
    void updated();

private:
    CuteFollari *m_cf;

    void printStations();
};

#endif // FOLLARIDISPLAY_H
