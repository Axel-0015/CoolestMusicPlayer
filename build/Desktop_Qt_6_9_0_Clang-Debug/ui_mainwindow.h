/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *prevBtn;
    QPushButton *playBtn;
    QPushButton *nextBtn;
    QPushButton *modeBtn;
    QPushButton *listBtn;
    QListWidget *musicList;
    QLabel *coverLabel;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *currentTime;
    QSlider *progressSlider;
    QLabel *totalTime;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(925, 513);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(20, 430, 621, 71));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        prevBtn = new QPushButton(widget);
        prevBtn->setObjectName("prevBtn");

        horizontalLayout->addWidget(prevBtn);

        playBtn = new QPushButton(widget);
        playBtn->setObjectName("playBtn");

        horizontalLayout->addWidget(playBtn);

        nextBtn = new QPushButton(widget);
        nextBtn->setObjectName("nextBtn");

        horizontalLayout->addWidget(nextBtn);

        modeBtn = new QPushButton(widget);
        modeBtn->setObjectName("modeBtn");
        modeBtn->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(modeBtn);

        listBtn = new QPushButton(widget);
        listBtn->setObjectName("listBtn");

        horizontalLayout->addWidget(listBtn);

        musicList = new QListWidget(centralwidget);
        musicList->setObjectName("musicList");
        musicList->setGeometry(QRect(650, 20, 251, 481));
        coverLabel = new QLabel(centralwidget);
        coverLabel->setObjectName("coverLabel");
        coverLabel->setGeometry(QRect(160, 70, 300, 300));
        coverLabel->setScaledContents(true);
        widget1 = new QWidget(centralwidget);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(90, 410, 461, 27));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        currentTime = new QLabel(widget1);
        currentTime->setObjectName("currentTime");

        horizontalLayout_2->addWidget(currentTime);

        progressSlider = new QSlider(widget1);
        progressSlider->setObjectName("progressSlider");
        progressSlider->setOrientation(Qt::Orientation::Horizontal);

        horizontalLayout_2->addWidget(progressSlider);

        totalTime = new QLabel(widget1);
        totalTime->setObjectName("totalTime");

        horizontalLayout_2->addWidget(totalTime);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        prevBtn->setText(QString());
        playBtn->setText(QString());
        nextBtn->setText(QString());
        modeBtn->setText(QString());
        listBtn->setText(QString());
        coverLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        currentTime->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        totalTime->setText(QCoreApplication::translate("MainWindow", "03:00", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
