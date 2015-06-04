#include <QSettings>
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
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
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

void MainWindow::loadSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/state").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/state", saveState());
}
