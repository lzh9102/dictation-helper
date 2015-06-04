#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>

class MediaPlayerWidget;

class PlayerController : public QObject
{
    Q_OBJECT
public:
    explicit PlayerController(MediaPlayerWidget *player, QObject *parent = 0);

signals:

public slots:

private:
    MediaPlayerWidget *m_player;
};

#endif // PLAYERCONTROLLER_H
