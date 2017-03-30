#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QWidget>
#include <iostream>
#include <QTime>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QPixmap>
#include <QMenuBar>
#include <QMenu>
#include "Game.h"
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *event);
    ~MainWindow();
    friend class Game;
    QMutex mutex;
private:
    QPixmap wallPixmap;
    QPixmap aimPixmap;
    QPixmap manPixmap;
    QPixmap manOnAimPixmap;
    QPixmap boxPixmap;
    QPixmap boxOnAimPixmap;
    QLabel *roundLabel;
    QLabel *stepLabel;
    QPushButton *nextBtn;
    QPushButton *restartBtn;
    QPushButton *preBtn;
    QPushButton *goBackBtn;
    QMenu *menu;
    int round;
    int cnt;
    int timerId;
    vector<int> showAns;
    vector<int>::iterator showAnsIt;
    Game *game;
    void checkGame();
    bool loadGame();
    bool loadGame(QString filename, QString);
    void nextStep();
    void preStep();
private slots:
    void onNextBtnClicked();
    void onRestartBtnClicked();
    void onPreBtnClicked();
    void onActionClicked();
    void showHelpInformation();
    void showAboutInformation();
    void onGoBackBtnClicked();
};

#endif // MAINWINDOW_H
