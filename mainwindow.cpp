#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIcon>
#include <QPixmap>
#include <QPalette>

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QAudioDevice>
#include <QMediaDevices>

#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QMessageBox>

#include <QRandomGenerator>

#include <QPropertyAnimation>

#include <QMediaMetaData>

#include <QPainter>

#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_mode(ORDER_MODE),
    m_isShowFlag(false)
{
    ui->setupUi(this);

    // Set window title and fixed size
    setWindowTitle(u8"Coolest Music Player");
    setFixedSize(920, 520);

    // Set custom background image
    setBackground(":/image/blackbackground.jpg");

    // Initialize button icons and connect signals
    initButtons();

    // Create audio output device
    m_audio = new QAudioOutput(this);
    m_audio->setDevice(QMediaDevices::defaultAudioOutput());  // Set default system audio output
    m_audio->setVolume(1.0);                                   // Set max volume
    m_audio->setMuted(false);                                  // Ensure it's not muted

    // Create media player and link to audio output
    m_player = new QMediaPlayer(this);
    m_player->setAudioOutput(m_audio);

    // Fetch albumn cover
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::updateCoverFromMetadata);

    //rotate albumn cover
    m_rotationTimer = new QTimer(this);
    connect(m_rotationTimer, &QTimer::timeout, this, &MainWindow::rotateCover);

    // Connect progress bar
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::handlePositionSlot);
    // Handle music duration
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::handleDurationSlot);

    // Handle atuoplay next song
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia) {
            if (m_mode == CYCLE_MODE) {
                m_player->setPosition(0);
                m_player->play();
            } else {
                handleNextSlot();
            }
        }
    });



    //Loading music directory
    m_musicDir = "/Users/axelli/Documents/Documents - Axel’s MacBook Air/QTProject/QTOnline/MusicPlayer/music/";
    loadAppointMusicDir(m_musicDir);


    srand(time(NULL));
}


void MainWindow::ensureSourceLoaded()
{
    if (m_player->source().isEmpty()) {
        QListWidgetItem* item = ui->musicList->currentItem();
        if (!item) return;
        QString musicPath = m_musicDir + item->text() + ".mp3";
        m_player->setSource(QUrl::fromLocalFile(musicPath));
        m_player->stop();
        m_player->setPosition(0);
        ui->progressSlider->setValue(0);
    }
}

// Helper function to apply styling to control buttons
void MainWindow::setButtonStyle(QPushButton *button, const QString &file){
    button->setFixedSize(50, 50);                 // Set button size
    button->setIcon(QIcon(file));                 // Set icon from file
    button->setIconSize(button->size());          // Icon fits button size
    button->setStyleSheet("background-color:transparent");  // Make background transparent
}

// Helper function to set the background image of the main window
void MainWindow::setBackground(const QString &file){
    QPixmap bg(file);  // Load background image
    QPalette p;
    p.setBrush(QPalette::Window,
               bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));  // Scale to window size
    setAutoFillBackground(true);  // Enable background painting
    setPalette(p);                // Apply background
}

// Setup all control buttons and their icons, connect play button to slot
void MainWindow::initButtons(){
    setButtonStyle(ui->prevBtn, ":/Icon/prev.png");
    setButtonStyle(ui->playBtn, ":/Icon/play.png");
    setButtonStyle(ui->nextBtn, ":/Icon/next.png");
    setButtonStyle(ui->modeBtn, ":/Icon/order.png");
    setButtonStyle(ui->listBtn, ":/Icon/music.png");

    ui->musicList->setStyleSheet(
        "QListWidget {"
        "  border: none;"
        "  border-radius: 20px;"
        "  background-color: rgba(255, 255, 255, 180);"
        "  padding: 5px;"
        "  font-size: 10px;"
        "}"
        "QListWidget::item {"
        "  padding: 10px;"
        "  border-radius: 10px;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: rgba(0, 230, 200, 100);"
        "}"
        "QListWidget::item:selected {"
        "  background-color: rgba(0, 230, 200, 180);"
        "  color: white;"
        "  font-weight: bold;"
        "}"
        );

    // ui->progressBar->setTextVisible(false);
    ui->progressSlider->setFixedHeight(6);
    ui->progressSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "  border: none;"
        "  height: 8px;"
        "  background: rgba(255,255,255,80);"
        "  border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "  background: transparent;"
        "  border: none;"
        "  width: 0px;"
        "  margin: 0px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "  background: #00FFC6;"
        "  border-radius: 4px;"
        "}"
        );

    ui->coverLabel->setStyleSheet("background-color: transparent; border: none;");

    ui->musicList->hide();

    ui->totalTime->setStyleSheet("color: #00FFC6;");
    ui->currentTime->setStyleSheet("color: #00FFC6;");

    // Connect play button click to play/pause handler
    connect(ui->playBtn, &QPushButton::clicked, this, &MainWindow::handlePlaySlot);
    // Connect mode button click to switch mode
    connect(ui->modeBtn, &QPushButton::clicked, this, &MainWindow::handleModeSlot);
    // Connect next button click to switch to next song
    connect(ui->nextBtn, &QPushButton::clicked, this, &MainWindow::handleNextSlot);
    // Connect prev button click to switch to prev song
    connect(ui->prevBtn, &QPushButton::clicked, this, &MainWindow::handlePrevSlot);
    // Connect list button click to show music list
    connect(ui->listBtn, &QPushButton::clicked, this, &MainWindow::handleListSlot);
    // Connect musicList clicked to play that music
    connect(ui->musicList, &QListWidget::itemClicked, this, &MainWindow::handleListClickSlot);
    // Connect progressSlider to bar slide
    connect(ui->progressSlider, &QSlider::sliderReleased, this, [this]() {
        m_player->setPosition(ui->progressSlider->value());
    });
}

//Handle music duration
void MainWindow::handleDurationSlot(int duration)
{
    if (ui && ui->progressSlider)
        ui->progressSlider->setMaximum(duration);

    int seconds = duration / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    ui->totalTime->setText(QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')));
}

// Handle progress bar
void MainWindow::handlePositionSlot(int position)
{
    if (ui && ui->progressSlider)
        ui->progressSlider->setValue(position);

    int seconds = position / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    ui->currentTime->setText(QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')));
}

// Handle music playing list
void MainWindow::handleListSlot()
{
    if(m_isShowFlag == false)
    {
        ui->musicList->show();
        showAnimation(ui->musicList);
        m_isShowFlag = true;
    }
    else
    {
        hideAnimation(ui->musicList);
        m_isShowFlag = false;
    }
}

//show music list
void  MainWindow::showAnimation(QWidget * window)
{
    QPropertyAnimation* animation = new QPropertyAnimation(window, "pos");
    //animation duration
    animation->setDuration(300);
    //animation starting cordinate
    animation->setStartValue(QPoint(this->width(), 20));
    //animation ending cordinate
    animation->setEndValue(QPoint(this->width() - ui->musicList->width()-20, 20));
    animation->start();

    QEventLoop loop;
    connect(animation, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    loop.exit();
}

//hide music list
void  MainWindow::hideAnimation(QWidget * window)
{
    QPropertyAnimation* animation = new QPropertyAnimation(window, "pos");
    //animation duration
    animation->setDuration(300);
    //animation starting cordinate
    animation->setStartValue(QPoint(this->width() - ui->musicList->width()-20, 20));
    //animation ending cordinate
    animation->setEndValue(QPoint(this->width(), 20));
    animation->start();

    QEventLoop loop;
    connect(animation, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    loop.exit();
}

// Play the music clicked
void MainWindow::handleListClickSlot(QListWidgetItem *item)
{
    QString musicName = item->text();
    QString musicPath = m_musicDir + musicName + ".mp3";
    m_player->setSource(QUrl::fromLocalFile(musicPath));
    handlePlaySlot();
}

//play music
void MainWindow::startPlayingMusic()
{
    QString musicName = ui->musicList->currentItem()->text();
    //music path
    QString musicPath = m_musicDir + musicName + ".mp3";
    m_player->setSource(QUrl::fromLocalFile(musicPath));
    m_player->play();
    ui->playBtn->setIcon(QIcon(":/Icon/stop.png"));
    m_rotationTimer->start(30);
}

// Handle next song
void MainWindow::handlePrevSlot()
{
    int currentRow = ui->musicList->currentRow();
    int prevRow = 0;
    if(m_mode == ORDER_MODE)
    {
        prevRow = (currentRow - 1);
        if(prevRow < 0)
        {
            prevRow = ui->musicList->count() - 1;
        }
    }
    else if(m_mode == RANDOM_MODE)
    {
        int cnt = 0;
        do
        {
            prevRow = QRandomGenerator::global()->bounded(ui->musicList->count());
        } while(currentRow == prevRow && cnt <= 3);
    }
    else
    {
        prevRow = currentRow;
    }
    ui->musicList->setCurrentRow(prevRow);
    startPlayingMusic();
}

// Handle next song
void MainWindow::handleNextSlot()
{
    int currentRow = ui->musicList->currentRow();
    int nextRow = 0;
    if(m_mode == ORDER_MODE)
    {
        nextRow = (currentRow + 1) % ui->musicList->count();
    }
    else if(m_mode == RANDOM_MODE)
    {
        int cnt = 0;
        do
        {
            nextRow = QRandomGenerator::global()->bounded(ui->musicList->count());
        } while(currentRow == nextRow && cnt <= 3);
    }
    else
    {
        nextRow = currentRow;
    }
    ui->musicList->setCurrentRow(nextRow);

    startPlayingMusic();
}

void MainWindow::handleModeSlot()
{
    if(m_mode == ORDER_MODE)
    {
        m_mode = RANDOM_MODE;
        ui->modeBtn->setIcon(QIcon(":/Icon/random.png"));
    }
    else if(m_mode == RANDOM_MODE)
    {
        m_mode = CYCLE_MODE;
        ui->modeBtn->setIcon(QIcon(":/Icon/cycle.png"));
    }
    else
    {
        m_mode = ORDER_MODE;
        ui->modeBtn->setIcon(QIcon(":/Icon/order.png"));
    }

}
// Slot: Handle play/pause toggle when play button is clicked
void MainWindow::handlePlaySlot()
{
    ensureSourceLoaded();
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();  // Pause playback
        ui->playBtn->setIcon(QIcon(":/Icon/play.png"));  // Change icon to play
        m_rotationTimer->stop();
    } else {
        m_player->play();   // Start playback
        ui->playBtn->setIcon(QIcon(":/Icon/stop.png"));  // Change icon to stop
        m_rotationTimer->start(30);
    }
}

//Loading directory
void MainWindow::loadAppointMusicDir(const QString & filepath)
{
    QDir dir(filepath);
    if(dir.exists() == false)
    {
        QMessageBox::warning(this, "directory", "fail to open directory");
        return;
    }

    QFileInfoList fileList = dir.entryInfoList(QDir::Files);
    for(auto element : fileList) {
        if(element.suffix() == "mp3")
        {
            ui->musicList->addItem(element.baseName());
        }
    }
    if (ui->musicList->count() > 0) {
        ui->musicList->setCurrentRow(0);
        m_player->setSource(QUrl::fromLocalFile(m_musicDir + ui->musicList->currentItem()->text() + ".mp3"));
    }

}

// Fetching the albumn cover
void MainWindow::updateCoverFromMetadata()
{
    const auto &md = m_player->metaData();


    QImage img = md.value(QMediaMetaData::CoverArtImage).value<QImage>();
    if (img.isNull())
        img = md.value(QMediaMetaData::ThumbnailImage).value<QImage>();

    if (img.isNull())
        img = extractCover(m_player->source().toLocalFile());

    if (img.isNull()) {
        qDebug() << "No cover embedded:" << m_player->source();
        ui->coverLabel->clear();
        return;
    }

    m_originalCover = QPixmap::fromImage(img).scaled(
        ui->coverLabel->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    rotateCover();
}

QImage MainWindow::extractCover(const QString &filePath)
{
    TagLib::MPEG::File mp3(filePath.toUtf8().constData());
    if (!mp3.isValid()) return {};

    auto *id3 = mp3.ID3v2Tag();
    if (!id3) return {};

    const auto frames = id3->frameListMap()["APIC"];   // Attached Picture
    if (frames.isEmpty()) return {};

    auto *pic = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
    if (!pic) return {};

    QImage img;
    img.loadFromData(
        reinterpret_cast<const uchar*>(pic->picture().data()),
        pic->picture().size());
    return img;
}

// Let the cover roate
void MainWindow::rotateCover()
{
    if (m_originalCover.isNull()) return;

    QPixmap canvas(m_originalCover.size());
    canvas.fill(Qt::transparent);

    QPainter p(&canvas);
    p.setRenderHint(QPainter::Antialiasing);

    p.translate(canvas.width()/2, canvas.height()/2);
    p.rotate(m_coverAngle);
    p.translate(-canvas.width()/2, -canvas.height()/2);
    p.drawPixmap(0, 0, m_originalCover);
    p.end();

    QPixmap circular(canvas.size());
    circular.fill(Qt::transparent);
    QPainter maskPainter(&circular);
    maskPainter.setRenderHint(QPainter::Antialiasing);
    maskPainter.setBrush(Qt::white);
    maskPainter.setPen(Qt::NoPen);
    maskPainter.drawEllipse(circular.rect());
    maskPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    maskPainter.drawPixmap(0, 0, canvas);
    maskPainter.end();

    ui->coverLabel->setPixmap(circular);

    m_coverAngle += 1;
    if (m_coverAngle >= 360) m_coverAngle -= 360;
}


// Destructor
MainWindow::~MainWindow(){
    delete ui;
}
