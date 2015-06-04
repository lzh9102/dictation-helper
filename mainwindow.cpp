#include <QSettings>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mediaplayerwidget.h"
#include "playercontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_player(NULL)
{
    ui->setupUi(this);
    setupPlayerWidget();
    setupSlots();

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

void MainWindow::slotOpenAudio()
{
    Q_ASSERT(m_player != NULL);
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Audio File"));
    if (!filename.isEmpty()) {
        m_player->load(filename);
    }
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

    m_player = player;
}

void MainWindow::setupSlots()
{
    connect(ui->actionOpenAudio, SIGNAL(triggered(bool)), SLOT(slotOpenAudio()));
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
