#include <QSettings>
#include <QFileDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QTextStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mediaplayerwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_player(NULL),
    m_textDirty(false)
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
    if (!confirmDiscardText()) {
        event->ignore();
        return;
    }

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

void MainWindow::slotOpenText()
{
    if (!confirmDiscardText())
        return;

    QSettings settings;
    QString path = settings.value("mainwindow/textfile_path", QDir::homePath()).toString();
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), path);
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QFile::ReadOnly)) {
            QTextStream s(&file);
            ui->editor->setPlainText(s.readAll());
            setTextFilePath(filename);
            m_textDirty = false;
        } else {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Failed to open file: %1").arg(filename));
        }
    }
}

void MainWindow::slotSaveText()
{
    if (m_textFileName.isEmpty()) {
        slotSaveTextAs();
        return;
    }

    // save text to m_textFileName
    QFile file(m_textFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream s(&file);
        s << ui->editor->toPlainText();
        file.close();
        m_textDirty = false;
    } else { // failed to open file for writing
        QMessageBox::critical(this, tr("Error"),
                              tr("Failed to save file: %1").arg(m_textFileName));
    }
}

void MainWindow::slotSaveTextAs()
{
    QSettings settings;
    QString path = settings.value("mainwindow/textfile_path", QDir::homePath()).toString();
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), path);
    if (!filename.isEmpty()) {
        setTextFilePath(filename);
        slotSaveText();
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

void MainWindow::slotTextChanged()
{
    m_textDirty = true;
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
    connect(ui->actionPointA, SIGNAL(triggered(bool)),
            player, SLOT(triggerPointA()));
    connect(ui->actionPointB, SIGNAL(triggered(bool)),
            player, SLOT(triggerPointB()));

    m_player = player;
}

void MainWindow::setupSlots()
{
    connect(ui->actionOpenAudio, SIGNAL(triggered(bool)), SLOT(slotOpenAudio()));
    connect(ui->actionOpenText, SIGNAL(triggered(bool)), SLOT(slotOpenText()));
    connect(ui->actionSetEditorFont, SIGNAL(triggered(bool)), SLOT(slotSetEditorFont()));
    connect(ui->actionSaveText, SIGNAL(triggered(bool)), SLOT(slotSaveText()));
    connect(ui->actionSaveTextAs, SIGNAL(triggered(bool)), SLOT(slotSaveTextAs()));
    connect(ui->editor, SIGNAL(textChanged()), SLOT(slotTextChanged()));
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

void MainWindow::setTextFilePath(QString filename)
{
    m_textFileName = filename;
    setWindowTitle(filename);
    QSettings().setValue("mainwindow/textfile_path", filename);
}

bool MainWindow::confirmDiscardText()
{
    if (m_textDirty) {
        int response = QMessageBox::question(this, tr("Discard"),
                                             tr("The file contains unsaved changes. "
                                                "Continue without saving?"),
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No);
        if (response != QMessageBox::Yes)
            return false;
    }
    return true;
}
