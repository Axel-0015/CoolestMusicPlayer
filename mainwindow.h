#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPushButton>
#include <QMediaPlayer>
#include <QListWidgetItem>
#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

enum PLAYMODE
{
    ORDER_MODE,
    RANDOM_MODE,
    CYCLE_MODE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handlePlaySlot();
    void handleModeSlot();
    void handleNextSlot();
    void handlePrevSlot();
    void handleListSlot();
    void handleListClickSlot(QListWidgetItem *item);
    void handlePositionSlot(int position);
    void handleDurationSlot(int duration);
    void updateCoverFromMetadata();
    void rotateCover();
    QImage extractCover(const QString &);
private:
    void setBackground(const QString & filename);
    void setButtonStyle(QPushButton * button, const QString & filename);
    void initButtons();
    void loadAppointMusicDir(const QString & filepath);
    void startPlayingMusic();
    void showAnimation(QWidget * window);
    void hideAnimation(QWidget * window);
    void ensureSourceLoaded();

private:
    Ui::MainWindow *ui;
    QAudioOutput * m_audio;
    QMediaPlayer * m_player;
    PLAYMODE m_mode;
    QString m_musicDir;
    bool m_isShowFlag;

    QTimer *m_rotationTimer = nullptr;
    QPixmap m_originalCover;
    qreal   m_coverAngle = 0;
};

#endif // MAINWINDOW_H
