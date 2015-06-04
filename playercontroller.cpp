#include "playercontroller.h"
#include "mediaplayerwidget.h"

PlayerController::PlayerController(MediaPlayerWidget *player, QObject *parent)
    : QObject(parent), m_player(player)
{

}
