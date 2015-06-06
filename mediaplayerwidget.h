#ifndef MEDIAPLAYERWIDGET_H
#define MEDIAPLAYERWIDGET_H

#include <QWidget>

namespace Ui {
class MediaPlayerWidget;
}

class MyQMPwidget;

class MediaPlayerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MediaPlayerWidget(QWidget *parent = 0);
    ~MediaPlayerWidget();

    bool ok() const;
    double duration() const;
    double position() const;

    double pointA() const;
    double pointB() const;

signals:
    void stateChanged();

public slots:

    /**
     * @brief Load media for playing from @a url.
     *
     * @param url path of the file
     */
    void load(const QString& url);

    /**
     * @brief Reload previously loaded file.
     */
    void reload();

    /**
     * @brief Start playing the loaded file.
     */
    void play();

    /**
     * @brief Seek to @a sec and start playing.
     *
     * @param sec position in seconds
     */
    void seek(int sec);

    /**
     * @brief Pause the media playback.
     */
    void pause();

    /**
     * @brief Toggle between Pause and Play.
     */
    void togglePlayPause();

    /**
     * @brief Trigger the PointA button to record PointA position.
     */
    void triggerPointA();

    /**
     * @brief Trigger the PointB button.
     *
     * The PointB button has an ON/OFF state. If PointB is ON after being triggered,
     * the current position is recorded as PointB position and the playback is restored
     * to PointA.
     */
    void triggerPointB();


protected:
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void refreshTimeDisplay();
    void refreshButtonState();
    void playerStateChanged();
    void seekSliderChanged();
    void seekBackward();
    void seekForward();
    void resetPosition();
    void slotPointA();
    void slotPointB();
    
private:
    Ui::MediaPlayerWidget *ui;
    MyQMPwidget *mplayer;
    QString m_file;

    double m_pointA, m_pointB;

    int m_volume;

    Q_DISABLE_COPY(MediaPlayerWidget)
    void load_volume();
    void save_volume();
};

#endif // MEDIAPLAYERWIDGET_H
