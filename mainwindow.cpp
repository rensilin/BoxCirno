#include "mainwindow.h"
#include <QString>
#include <sstream>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QPainter>
#include <QColor>
#include <QAction>
#include <QProgressDialog>
#include <QFileDialog>
#include <QCoreApplication>
#include <QtCore>
#include <QFontDatabase>
#include <QApplication>

using namespace std;

const int BOXSIZE=60;
const int MENUSIZE=23;
const int PANELX=4*BOXSIZE;
const int PANELY=MENUSIZE;
const char title[]="一点也不奇怪的推箱子  v1.1.1";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , wallPixmap(":/image/wall.png")
    , aimPixmap(":/image/aim.png")
    , manPixmap(":/image/cirno.png")
    , manOnAimPixmap(":/image/cirnoOnAim.png")
    , boxPixmap(":/image/box.png")
    , boxOnAimPixmap(":/image/boxOnAim.png")
{
    setWindowIcon(QIcon(":/icon/cirno.ico"));
    cnt=0;
    setFocus();
    wallPixmap=wallPixmap.scaled(BOXSIZE,BOXSIZE);
    aimPixmap=aimPixmap.scaled(BOXSIZE,BOXSIZE);
    manPixmap=manPixmap.scaled(BOXSIZE,BOXSIZE);
    manOnAimPixmap=manOnAimPixmap.scaled(BOXSIZE,BOXSIZE);
    boxPixmap=boxPixmap.scaled(BOXSIZE,BOXSIZE);
    boxOnAimPixmap=boxOnAimPixmap.scaled(BOXSIZE,BOXSIZE);
    this->setWindowTitle(title);
    roundLabel=new QLabel(this);
    roundLabel->setAlignment(Qt::AlignCenter);
    stepLabel=new QLabel(this);
    nextBtn=new QPushButton(QString("下一关"),this);
    restartBtn=new QPushButton(QString("重新开始"),this);
    goBackBtn=new QPushButton(QString("回退"),this);
    preBtn=new QPushButton(QString("上一关"),this);

    menu=menuBar()->addMenu(tr("选项"));
    QAction *action=menu->addAction(tr("读取外部关卡"));
    menu->addSeparator();
    QAction *action2=menu->addAction(tr("重新开始"));
    connect(action2,SIGNAL(triggered()),this,SLOT(onRestartBtnClicked()));
    menu->addSeparator();
    QAction *action3=menu->addAction(tr("上一关"));
    connect(action3,SIGNAL(triggered()),this,SLOT(onPreBtnClicked()));
    QAction *action1=menu->addAction(tr("下一关"));
    connect(action1,SIGNAL(triggered()),this,SLOT(onNextBtnClicked()));

    QMenu *aboutMenu=menuBar()->addMenu(tr("帮助"));
    QAction *helpAction=aboutMenu->addAction(tr("快捷键"));
    connect(helpAction,SIGNAL(triggered()),this,SLOT(showHelpInformation()));
    QAction *aboutAction=aboutMenu->addAction(tr("关于"));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(showAboutInformation()));

    connect(action,SIGNAL(triggered()),this,SLOT(onActionClicked()));
    connect(nextBtn,SIGNAL(clicked()),this,SLOT(onNextBtnClicked()));
    connect(restartBtn,SIGNAL(clicked()),this,SLOT(onRestartBtnClicked()));
    connect(goBackBtn,SIGNAL(clicked()),this,SLOT(onGoBackBtnClicked()));
    connect(preBtn,SIGNAL(clicked()),this,SLOT(onPreBtnClicked()));

    game=NULL;
    round=0;
    loadGame();
}

bool MainWindow::loadGame(QString filename,QString qs)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))return false;
    delete game;
    string str=QString(file.readAll()).toStdString();
    istringstream sin(str);
    string s,ss;
    int n,m;
    sin>>n>>m;getline(sin,ss);
    while(getline(sin,ss))s.append(ss);
    while(n<7)
    {
        n++;
        for(int i=0;i<m;i++)s.append(" ");
    }
//    cout<<n<<' '<<m<<"\n"<<s<<endl;
    game=new Game(n,m,s);
    repaint();
    setFixedSize(BOXSIZE*(m+4),MENUSIZE+BOXSIZE*n);
    QFont font;font.setPointSize(BOXSIZE/2);
    int x=BOXSIZE/2,y=MENUSIZE+BOXSIZE/2;
    int w=BOXSIZE*3,h=BOXSIZE;
    roundLabel->setGeometry(x,y,w,h);
    if(qs!=tr(""))roundLabel->setText("第"+qs+"关");
    else roundLabel->setText("自定义");
    roundLabel->setFont(font);
    y+=BOXSIZE+BOXSIZE/2;
    preBtn->setGeometry(x,y,w,h);
    y+=BOXSIZE+BOXSIZE/2;
    restartBtn->setGeometry(x,y,w/2-2,h);
    goBackBtn->setGeometry(x+restartBtn->width()+4,y,w/2-2,h);
    y+=BOXSIZE+BOXSIZE/2;
    nextBtn->setGeometry(x,y,w,h);
    y=(y+BOXSIZE+n*(BOXSIZE))/2-h/2+MENUSIZE/2;
    font.setPointSize(font.pointSize()*2/3);
    stepLabel->setFont(font);
    stepLabel->setText("步数: 0");
    stepLabel->setGeometry(x,y,w,h);
    return true;
}

bool MainWindow::loadGame()
{
    QString qs;
    qs.setNum(round);
    return loadGame(":/map/map"+qs+".txt",qs);
}

MainWindow::~MainWindow()
{
    delete game;
    delete roundLabel;
    delete stepLabel;
    delete nextBtn;
    delete restartBtn;
    delete preBtn;
    delete goBackBtn;
}

void MainWindow::nextStep()
{
    game->move(*showAnsIt);
    showAnsIt++;
    checkGame();
    if(showAnsIt==showAns.end())
    {
        cnt=0;
    }
}

void MainWindow::preStep()
{
    if(showAnsIt==showAns.begin())return;
    game->goBack();
    showAnsIt--;
    checkGame();
}

#define PAINT(pixmap) painter.drawPixmap(PANELX+j*BOXSIZE,PANELY+i*BOXSIZE,BOXSIZE,BOXSIZE,pixmap)

void MainWindow::paintEvent(QPaintEvent *)
{
    if(cnt==4)
    {
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::red);
        stepLabel->setPalette(pe);
    }
    else
    {
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::black);
        stepLabel->setPalette(pe);
    }
    QPainter painter(this);
    QColor color(222,222,222);
    painter.setPen(color);
    painter.setBrush(QBrush(color,Qt::SolidPattern));
    painter.drawRect(QRect(PANELX,PANELY,BOXSIZE*game->m,BOXSIZE*game->n));
    for(int i=0;i<game->n;i++)
    {
        for(int j=0;j<game->m;j++)
        {
            if(game->mp[i][j]==Game::WALL)PAINT(wallPixmap);
            else if(game->mp[i][j]==Game::AIM)PAINT(aimPixmap);
            else if(game->mp[i][j]==Game::MAN)PAINT(manPixmap);
            else if(game->mp[i][j]==(Game::MAN|Game::AIM))PAINT(manOnAimPixmap);
            else if(game->mp[i][j]==Game::BOX)PAINT(boxPixmap);
            else if(game->mp[i][j]==(Game::BOX|Game::AIM))PAINT(boxOnAimPixmap);
        }
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QMutexLocker locker(&mutex);
    if(event->button()==Qt::LeftButton)//&&(event->buttons()&Qt::LeftButton))
    {
        if(cnt==4)nextStep();
        else
        {
            int y=(event->pos().x()-PANELX)/BOXSIZE,x=(event->pos().y()-PANELY)/BOXSIZE;
            if(x<0||x>=game->n||y<0||y>=game->m)return;
            vector<int>v=game->move(x,y);
            //setEnabled(false);
            for(vector<int>::iterator i=v.begin();i!=v.end();i++)
            {
               game->move(*i);
               QElapsedTimer timer;
               timer.start();
               while(timer.elapsed()<50);
                   //QCoreApplication::processEvents();
               checkGame();
            }
            //setEnabled(true);
        }

    }
    else if(event->button()==Qt::RightButton)
    {
        if(cnt==4)preStep();
        else onGoBackBtnClicked();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    QMutexLocker locker(&mutex);
    if(cnt==4)
    {
        switch(e->key())
        {
        case Qt::Key_Escape:
            cnt=0;
            checkGame();
            break;
        case Qt::Key_Left:
            preStep();
            break;
        case Qt::Key_Right:
            nextStep();
            break;
        }
        return;
    }
    if(cnt==3)
    {
        if(e->key()=='E')
        {
            cnt++;
            game->start();

            int nowV=0,maxV=game->getH();
            int maxRange=6666;
            QProgressDialog process(this);
            QString qs;
            process.setLabelText(tr("搜索路径中...\n估计步数")+qs.setNum(maxV*10));
            process.setWindowTitle(this->windowTitle());
            process.setWindowModality(Qt::WindowModal);
            process.setRange(0,maxRange);
            process.setCancelButtonText(tr("取消"));
            process.setMinimumDuration(0);
            process.setMinimumWidth(300);
            process.show();
            while(nowV<maxRange)
            {
                nowV=min(nowV+1,(maxV-game->minGH)*maxRange/maxV);
                process.setValue(nowV);
                QApplication::processEvents();
                if(process.wasCanceled())
                {
                    game->minGH=-1;
                    process.setLabelText(tr("正在取消搜索..."));
                    game->wait();
                    cnt=0;
                    return;
                }
            }
            game->wait();
            showAns=game->ans;
            showAnsIt=showAns.begin();
            game->restart();
            checkGame();
            return;
        }
        else if(e->key()=='K')return;
        else cnt=0;
    }
    else if(e->key()=='K')
    {
        cnt++;
    }

    else cnt=0;
    switch (e->key()) {
    case 'W':
    case Qt::Key_Up:
        game->move(Game::UP);
        break;
    case 'S':
    case Qt::Key_Down:
        game->move(Game::DOWN);
        break;
    case 'A':
    case Qt::Key_Left:
        game->move(Game::LEFT);
        break;
    case 'D':
    case Qt::Key_Right:
        game->move(Game::RIGHT);
        break;
    case 'R':
        onRestartBtnClicked();
        break;
    case 'P':
        onGoBackBtnClicked();
    }
    checkGame();
}

void MainWindow::checkGame()
{
    repaint();
    QString s;
    stepLabel->setText("步数: "+s.setNum(game->getStep()));
    if(game->isWin())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("恭喜过关"),
                                            tr("使用")+stepLabel->text()+"\n开始下一关?",
                                            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)round++;
        else
        {
            game->restart();
            checkGame();
            return;
        }
        if(!loadGame())
        {
            reply = QMessageBox::question(this, tr("恭喜通关"),
                                                tr("您已通关,是否从第零关开始?"),
                                                QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes)
            {
                round=0;
                loadGame();
            }
            else if(reply == QMessageBox::No)exit(0);
            else round--;
        }
    }
}

void MainWindow::onNextBtnClicked()
{
    if(cnt==4){
        //killTimer(timerId);
        cnt=0;}
    round++;
    if(!loadGame())round--;
    update();
    setFocus();
}

void MainWindow::onRestartBtnClicked()
{
    if(cnt==4){
        //killTimer(timerId);
        cnt=0;}
    game->restart();
    checkGame();
    setFocus();
}

void MainWindow::onPreBtnClicked()
{
    if(cnt==4){
        //killTimer(timerId);
        cnt=0;}
    round--;
    if(!loadGame())round++;
    update();
    setFocus();
}

void MainWindow::onGoBackBtnClicked()
{if(cnt==4)return;
    game->goBack();
    checkGame();
}

void MainWindow::onActionClicked()
{
    if(cnt==4){
        //killTimer(timerId);
        cnt=0;}
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("选择自定义地图"),
                                                    tr(""),
                                                    tr(".txt(*.txt)")
                                                    );
    loadGame(filename,tr(""));
}

void MainWindow::showHelpInformation()
{
    QMessageBox::information(NULL, "帮助",tr(
                                 "移动	方向键,WASD,鼠标\n"
                                 "重新开始	R\n"
                                 "回退	P,鼠标右键\n"
                                 "自动寻路	(听说这个是神秘代码)\n"
                                 "寻路上一步	右方向键，鼠标左键\n"
                                 "寻路下一步	左方向键，鼠标右键\n"
                                 "停止寻路	Esc\n")
                             , QMessageBox::Yes);
}
void MainWindow::showAboutInformation()
{
    QMessageBox::information(NULL, "关于",tr(
                             "作者: kkke  (输入这个可能会有神秘事件发生)\n"
                             "邮箱: kkke@nwsuaf.edu.cn\n"
                             "CSDN博客:http://blog.csdn.net/tookkke\n"
                             "本游戏始于kkke的课程设计，不能用于商业用途\n"
                             "详见帮助文档")
                             , QMessageBox::Yes);
}
