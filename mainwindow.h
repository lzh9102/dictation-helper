#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MediaPlayerWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void slotOpenAudio();
    void slotSaveText();
    void slotSaveTextAs();
    void slotSetEditorFont();
    void slotTextChanged();

private:
    Ui::MainWindow *ui;
    MediaPlayerWidget *m_player;

    QString m_textFileName; // text file will be set to this path
    bool m_textDirty; // text not saved yet

    void setupPlayerWidget();
    void setupSlots();
    void loadSettings();
    void saveSettings();

    bool confirmQuit();
};

#endif // MAINWINDOW_H
