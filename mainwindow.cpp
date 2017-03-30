#include "mainwindow.h"
#include <QString>
#include <sstream>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QPainter>
#include <QColor>
#include <Qtimer>
#include <QAction>
#include <QProgressDialog>
#include <QFileDialog>
#include <QCoreApplication>
#include <QtCore>
#include <QApplication>

#include "Vector.h"

using namespace std;

const int BOXSIZE=60;
const int MENUSIZE=23;

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
    this->setWindowTitle("一点也不奇怪的推箱子  v1.0.1");
    roundLabel=new QLabel(this);
    roundLabel->setAlignment(Qt::AlignCenter);
    stepLabel=new QLabel(this);
    nextBtn=new QPushButton(QString("下一关"),this);
    restartBtn=new QPushButton(QString("重新开始"),this);
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
    restartBtn->setGeometry(x,y,w,h);
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
}

#define PAINT(pixmap) painter.drawPixmap((4+j)*BOXSIZE,MENUSIZE+i*BOXSIZE,BOXSIZE,BOXSIZE,pixmap)

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor color(222,222,222);
    painter.setPen(color);
    painter.setBrush(QBrush(color,Qt::SolidPattern));
    painter.drawRect(QRect(4*BOXSIZE,MENUSIZE,BOXSIZE*game->m,BOXSIZE*game->n));
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

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(cnt==4)
    {
        if(e->key()==Qt::Key_Escape){killTimer(timerId);cnt=0;}
        else if(e->key()=='Q')
        {
                timerEvent((QTimerEvent*)NULL);
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
            timerId=startTimer(300);
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
    case 'w':
    case 'W':
    case Qt::Key_Up:
        game->move(Game::UP);
        break;
    case 's':
    case 'S':
    case Qt::Key_Down:
        game->move(Game::DOWN);
        break;
    case 'a':
    case 'A':
    case Qt::Key_Left:
        game->move(Game::LEFT);
        break;
    case 'd':
    case 'D':
    case Qt::Key_Right:
        game->move(Game::RIGHT);
        break;
    case 'r':
    case 'R':
        onRestartBtnClicked();
        break;
    }
    checkGame();
}

void MainWindow::checkGame()
{
    update();
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
        killTimer(timerId);
        cnt=0;}
    round++;
    if(!loadGame())round--;
    update();
    setFocus();
}

void MainWindow::onRestartBtnClicked()
{
    if(cnt==4){
        killTimer(timerId);
        cnt=0;}
    game->restart();
    checkGame();
    setFocus();
}

void MainWindow::onPreBtnClicked()
{
    if(cnt==4){
        killTimer(timerId);
        cnt=0;}
    round--;
    if(!loadGame())round++;
    update();
    setFocus();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    mutex.lock();
    if(cnt==0||showAnsIt==showAns.end())
    {
        mutex.unlock();
        return;
    }
    game->move(*showAnsIt);
    if(++showAnsIt==showAns.end())
    {
        killTimer(timerId);
        cnt=0;
    }
    mutex.unlock();
    checkGame();
}

void MainWindow::onActionClicked()
{
    if(cnt==4){
        killTimer(timerId);
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
                             "移动		方向键,WASD\n"
                             "重新开始	R\n"
                             "自动寻路	????(听说这个是神秘代码)\n"
                             "加速寻路	Q\n"
                             "停止寻路	Esc")
                             , QMessageBox::Yes);
}
void MainWindow::showAboutInformation()
{
    QMessageBox::information(NULL, "关于",tr(
                             "作者: kkke  (输入这个可能会有神秘事件发生)\n"
                             "邮箱: kkke@nwsuaf.edu.cn\n"
                             "CSDN博客:http://blog.csdn.net/tookkke\n"
                             "本游戏是kkke的课程设计，不能用于商业用途\n"
                             "详见帮助文档")
                             , QMessageBox::Yes);
}
