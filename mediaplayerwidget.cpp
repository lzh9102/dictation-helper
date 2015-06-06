#include <QSettings>
#include <QWheelEvent>
#include <QLabel>
#include "mediaplayerwidget.h"
#include "ui_mediaplayerwidget.h"
#include "myqmpwidget.h"

#define DEFAULT_VOLUME 50
#define MAX_VOLUME 100
#define VOLUME_SETTING_KEY "mediaplayer/volume"

namespace {
QString sec2hms(int seconds)
{
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = (seconds % 60);
    QString result;
    result.sprintf("%02d:%02d:%02d", h, m, s);
    return result;
}
QString sec2hms(double seconds)
{
    int h = (int)seconds / 3600;
    int m = ((int)seconds % 3600) / 60;
    int s = ((int)seconds % 60);
    int ms = (seconds - (int)seconds) * 1000;
    QString result;
    result.sprintf("%02d:%02d:%02d.%02d", h, m, s, ms/10);
    return result;
}
}

MediaPlayerWidget::MediaPlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaPlayerWidget),
    m_pointA(0.0), m_pointB(0.0)
{
    ui->setupUi(this);
    ui->slideVolume->setRange(0, MAX_VOLUME);
    mplayer = new MyQMPwidget(this);
    mplayer->start();
    mplayer->setAcceptDrops(false);
    mplayer->setSeekSlider(ui->slideSeek);
    mplayer->setVolumeSlider(ui->slideVolume);
    ui->layoutPlayer->addWidget(mplayer);
    mplayer->hide();
    //ui->slideSeek->setStyleSheet(SLIDER_STYLESHEET);

    connect(mplayer, SIGNAL(stateChanged(int)), SLOT(playerStateChanged()));
    connect(ui->slideSeek, SIGNAL(valueChanged(int)), SLOT(seekSliderChanged()));
    connect(ui->btnPlayPause, SIGNAL(clicked()), SLOT(togglePlayPause()));
    connect(ui->btnBack, SIGNAL(clicked()), SLOT(seekBackward()));
    connect(ui->btnForward, SIGNAL(clicked()), SLOT(seekForward()));
    connect(ui->btnReset, SIGNAL(clicked()), SLOT(resetPosition()));

    connect(ui->btnPointA, SIGNAL(clicked(bool)), SLOT(slotPointA()));
    connect(ui->btnPointB, SIGNAL(clicked(bool)), SLOT(slotPointB()));

    load_volume();
}

MediaPlayerWidget::~MediaPlayerWidget()
{
    save_volume();
    delete ui;
}

bool MediaPlayerWidget::ok() const
{
    MyQMPwidget::MediaInfo info = mplayer->mediaInfo();
    return info.ok;
}

double MediaPlayerWidget::duration() const
{
    MyQMPwidget::MediaInfo info = mplayer->mediaInfo();
    if (info.ok)
        return info.length;
    else
        return 0;
}

double MediaPlayerWidget::position() const
{
    return mplayer->tell();
}

double MediaPlayerWidget::pointA() const
{
    return m_pointA;
}

double MediaPlayerWidget::pointB() const
{
    if (ui->btnPointB->isChecked())
        return m_pointB;
    else
        return duration();
}

// public slots

void MediaPlayerWidget::load(const QString &url)
{
    m_file = url;
    mplayer->load(url);
    ui->slideVolume->setValue(m_volume);
    mplayer->pause();
    ui->lblFilename->setText(m_file);

    // clear A, B points
    m_pointA = 0.0;
    m_pointB = 0.0;
    ui->btnPointB->setChecked(false);
    ui->btnPointA->setText("A");
    ui->btnPointB->setText("B");
}

void MediaPlayerWidget::reload()
{
    load(m_file);
}

void MediaPlayerWidget::play()
{
    mplayer->play();
    refreshButtonState();
}

void MediaPlayerWidget::pause()
{
    mplayer->pause();
    refreshButtonState();
}

void MediaPlayerWidget::seek(int sec)
{
    mplayer->seek(sec);
}

void MediaPlayerWidget::togglePlayPause()
{
    switch (mplayer->state()) {
    case MyQMPwidget::IdleState:
        reload();
        if (ui->slideSeek->value() < ui->slideSeek->maximum()) { // user seeks
            mplayer->seek(ui->slideSeek->value());
        } else { // otherwise rewind to begin
            ui->slideSeek->setValue(0);
        }
        pause();
        break;
    case MyQMPwidget::PlayingState:
        pause();
        break;
    case MyQMPwidget::PausedState:
        play();
        break;
    default:
        break;
    }
}

void MediaPlayerWidget::triggerPointA()
{
    ui->btnPointA->click();
}

void MediaPlayerWidget::triggerPointB()
{
    ui->btnPointB->click();
}

// events

void MediaPlayerWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8; // delta is in eighths of a degree
    if (event->orientation() == Qt::Vertical) {
        if (numDegrees >= 0) {
            seekForward();
        } else {
            seekBackward();
        }
    }
}

void MediaPlayerWidget::mousePressEvent(QMouseEvent */*event*/)
{
    togglePlayPause();
}

// private slots

void MediaPlayerWidget::refreshTimeDisplay()
{
    MyQMPwidget::MediaInfo info = mplayer->mediaInfo();
    int duration, position, remaining;
    if (info.ok) {
        duration = info.length;
        position = mplayer->tell();
        if (position < 0) position = 0;
        remaining = duration - position;
    } else {
        duration = position = remaining = 0;
    }
    ui->lblPosition->setText(QString("%1 / %2")
                             .arg(sec2hms(position))
                             .arg(sec2hms(duration)));
    ui->lblRemaining->setText(QString("-%1").arg(sec2hms(remaining)));
}

void MediaPlayerWidget::refreshButtonState()
{
    QString button_icon = ":/actions/images/media-playback-start";
    switch (mplayer->state()) {
    case MyQMPwidget::PlayingState:
        button_icon = ":/actions/images/media-playback-pause"; break;
    default:
        break;
    }
    ui->btnPlayPause->setIcon(QIcon(button_icon));
}

void MediaPlayerWidget::playerStateChanged()
{
    refreshTimeDisplay();
    refreshButtonState();
    emit stateChanged();
}

void MediaPlayerWidget::seekSliderChanged()
{
    refreshTimeDisplay();

    // reached point B
    if (ui->btnPointB->isChecked() && mplayer->tell() >= m_pointB) {
        mplayer->seek(m_pointA);
    }
}

void MediaPlayerWidget::seekBackward()
{
    mplayer->seek(-3, MyQMPwidget::RelativeSeek);
}

void MediaPlayerWidget::seekForward()
{
    mplayer->seek(3, MyQMPwidget::RelativeSeek);
}

void MediaPlayerWidget::resetPosition()
{
    mplayer->seek(0);
}

void MediaPlayerWidget::slotPointA()
{
    m_pointA = mplayer->tell();
    ui->btnPointA->setText(QString("A: %1").arg(sec2hms(m_pointA)));
}

void MediaPlayerWidget::slotPointB()
{
    if (ui->btnPointB->isChecked()) {
        m_pointB = mplayer->tell();
        ui->btnPointB->setText(QString("B: %1").arg(sec2hms(m_pointB)));
    } else {
        ui->btnPointB->setText("B");
    }
}

void MediaPlayerWidget::load_volume()
{
    QSettings settings;
    m_volume = DEFAULT_VOLUME;
    if (settings.contains(VOLUME_SETTING_KEY))
        m_volume = settings.value(VOLUME_SETTING_KEY).toInt();
    if (m_volume < 0)
        m_volume = 0;
    if (m_volume > MAX_VOLUME)
        m_volume = MAX_VOLUME;
    ui->slideVolume->setValue(m_volume);
}

void MediaPlayerWidget::save_volume()
{
    m_volume = ui->slideVolume->value();
    QSettings().setValue(VOLUME_SETTING_KEY, m_volume);
}
