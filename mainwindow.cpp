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
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
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

    // bind visibility to menu (View -> Show Player)
    connect(ui->playerDock, SIGNAL(visibilityChanged(bool)),
            ui->actionShowPlayer, SLOT(setChecked(bool)));
    connect(ui->actionShowPlayer, SIGNAL(toggled(bool)),
            ui->playerDock, SLOT(setShown(bool)));
}

void MainWindow::loadSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/state").toByteArray());
    ui->actionShowPlayer->setChecked(settings.value("mainwindow/view/player", true).toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/state", saveState());
    settings.setValue("mainwindow/view/player", ui->actionShowPlayer->isChecked());
}
