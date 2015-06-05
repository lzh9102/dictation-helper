#include <QSettings>
#include <QFileDialog>
#include <QFontDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mediaplayerwidget.h"

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
    QSettings settings;
    QString path = settings.value("mainwindow/openaudio_path", QDir::homePath()).toString();
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Audio File"), path);
    if (!filename.isEmpty()) {
        m_player->load(filename);
        settings.setValue("mainwindow/openaudio_path", filename);
    }
}

void MainWindow::slotSetEditorFont()
{
    bool ok;
    QFont font = ui->editor->font();
    font = QFontDialog::getFont(&ok, font, this);
    if (ok) {
        ui->editor->setFont(font);
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

    containerWidget->setLayout(layout);
    layout->addWidget(player);

    ui->playerDock->setWidget(containerWidget);

    // bind visibility to menu (View -> Show Player)
    connect(ui->playerDock, SIGNAL(visibilityChanged(bool)),
            ui->actionShowPlayer, SLOT(setChecked(bool)));
    connect(ui->actionShowPlayer, SIGNAL(toggled(bool)),
            ui->playerDock, SLOT(setShown(bool)));

    // bind player control slots to menu
    connect(ui->actionPlayPause, SIGNAL(triggered(bool)),
            player, SLOT(togglePlayPause()));
    connect(ui->actionFastForward, SIGNAL(triggered(bool)),
            player, SLOT(seekForward()));
    connect(ui->actionRewind, SIGNAL(triggered(bool)),
            player, SLOT(seekBackward()));

    m_player = player;
}

void MainWindow::setupSlots()
{
    connect(ui->actionOpenAudio, SIGNAL(triggered(bool)), SLOT(slotOpenAudio()));
    connect(ui->actionSetEditorFont, SIGNAL(triggered(bool)), SLOT(slotSetEditorFont()));
}

void MainWindow::loadSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/state").toByteArray());
    ui->actionShowPlayer->setChecked(settings.value("mainwindow/view/player", true).toBool());

    // restore editor font
    QFont font = ui->editor->font();
    if (font.fromString(settings.value("mainwindow/editor/font").toString())) {
        ui->editor->setFont(font);
    }
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/state", saveState());
    settings.setValue("mainwindow/view/player", ui->actionShowPlayer->isChecked());

    // save editor font
    QFont font = ui->editor->font();
    settings.setValue("mainwindow/editor/font", font.toString());
}
