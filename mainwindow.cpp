#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mediaplayerwidget.h"
#include "playercontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupPlayerWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPlayerWidget()
{
    /* Use the following hierarchy to put multiple widgets in a dockWidget.
     *
     * dockWidget
     * |- containerWidget
     *    |- layout
     *       |- widget1
     *       |- widget2
     *       |- ...
     */

    QWidget *containerWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    MediaPlayerWidget *player = new MediaPlayerWidget(this);
    PlayerController *controller = new PlayerController(player, this);

    containerWidget->setLayout(layout);
    layout->addWidget(player);

    ui->playerDock->setWidget(containerWidget);
}
