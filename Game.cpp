#include "Game.h"
#include <queue>

const int Game::WALL=-1;
const int Game::AIM=1;
const int Game::MAN=2;
const int Game::BOX=4;
const int Game::mx[]={ 0, 0, 1,-1};
const int Game::my[]={ 1,-1, 0, 0};
const char Game::mv[]={'R','L','D','U'};
const int Game::RIGHT=0;
const int Game::LEFT=1;
const int Game::DOWN=2;
const int Game::UP=3;

Game::Game(int N,int M,string s):n(N),m(M)
{
    startStatus.aim=&aim;
    startStatus.g=0;
    startStatus.pre=-1;
    mp=new int*[n];
    for(int i=0;i<n;i++)mp[i]=new int[m];
    wall=new bool*[n];
    for(int i=0;i<n;i++)wall[i]=new bool[m];
    isAim=new bool*[n];
    for(int i=0;i<n;i++)isAim[i]=new bool[m];
    string::iterator it=s.begin();
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            isAim[i][j]=false;
            if(*it=='#')wall[i][j]=true;
            else
            {
                wall[i][j]=false;
                switch(*it)
                {
                case '*':
                    startStatus.box.insert(Point(i,j));
                    break;
                case '0':
                    startStatus.man=Point(i,j);
                    break;
                case '+':
                    startStatus.box.insert(Point(i,j)); //no break
                case '$':
                    aim.push_back(Point(i,j));
                    isAim[i][j]=true;
                    break;
                }
            }
            it++;
        }
    }
    restart();
}

Game::~Game()
{
    for(int i=0;i<n;i++)delete[]wall[i];
    delete[]wall;
    for(int i=0;i<n;i++)delete[]isAim[i];
    delete[]isAim;
    for(int i=0;i<n;i++)delete[]mp[i];
    delete[]mp;
}

void Game::goBack()
{
    if(oldStatus.empty())return;
    mp[player.man.x][player.man.y]-=MAN;
    for(set<Point>::iterator i=player.box.begin();i!=player.box.end();i++)
        mp[i->x][i->y]-=BOX;
    player=oldStatus.top();oldStatus.pop();
    mp[player.man.x][player.man.y]+=MAN;
    for(set<Point>::iterator i=player.box.begin();i!=player.box.end();i++)
        mp[i->x][i->y]+=BOX;
}

const vector<int>& Game::solve()
{
    multiset<Status> ts;
    map<Status,Status>searched;
    startStatus.h=-1;
    searched.insert(make_pair(startStatus,startStatus));
    startStatus.h=minGH;
    ts.insert(startStatus);
    while(ts.size())
    {
        if(minGH==-1)return ans;
        Status now(*ts.begin());ts.erase(ts.begin());
        minGH=min(minGH,now.h);
        now.h=-1;now.g++;
        for(int i=0;i<4;i++)
        {
            if(minGH==-1)return ans;
            Status next(now);
            next.pre=i;
            next.man.x+=mx[i];
            next.man.y+=my[i];
            if(wall[next.man.x][next.man.y])continue;
            if(next.box.erase(next.man))
            {
                Point pushBox=next.man;
                pushBox.x+=mx[i];
                pushBox.y+=my[i];
                if(wall[pushBox.x][pushBox.y])continue;
                if(next.box.find(pushBox)!=next.box.end())continue;
                if(!isAim[pushBox.x][pushBox.y])//////////////////////////////////////////
                {/////////////////////////////////////////////////////////////////////////
                    int flag=-1;
                    if(wall[pushBox.x+mx[i]][pushBox.y+my[i]])
                    {
                        if(wall[pushBox.x+mx[i^2]][pushBox.y+my[i^2]])continue;
                        if(wall[pushBox.x+mx[i^3]][pushBox.y+my[i^3]])continue;
                        int cntAim=0,cntBox=1;
                        bool ff=true;
                        for(int xx=pushBox.x+mx[i^2],yy=pushBox.y+my[i^2];!wall[xx][yy];xx+=mx[i^2],yy+=my[i^2])
                        {
                            if(!(wall[xx+mx[i]][yy+my[i]]||wall[xx+mx[i^1]][yy+my[i^1]]))
                            {
                                ff=false;
                                break;
                            }
                            if(isAim[xx][yy])cntAim++;
                            else if(next.box.find(Point(xx,yy))!=next.box.end())cntBox++;
                        }
                        if(ff)for(int xx=pushBox.x+mx[i^3],yy=pushBox.y+my[i^3];!wall[xx][yy];xx+=mx[i^3],yy+=my[i^3])
                        {
                            if(!(wall[xx+mx[i]][yy+my[i]]||wall[xx+mx[i^1]][yy+my[i^1]]))
                            {
                                ff=false;
                                break;
                            }
                            if(isAim[xx][yy])cntAim++;
                            else if(next.box.find(Point(xx,yy))!=next.box.end())cntBox++;
                        }
                        if(ff&&cntAim<cntBox)continue;
                        if(next.box.find(Point(pushBox.x+mx[i^2],pushBox.y+my[i^2]))!=next.box.end())flag=i^2;
                        else if(next.box.find(Point(pushBox.x+mx[i^3],pushBox.y+my[i^3]))!=next.box.end())flag=i^3;
                    }
                    else if(next.box.find(Point(pushBox.x+mx[i],pushBox.y+my[i]))!=next.box.end())
                    {
                        if(wall[pushBox.x+mx[i^2]][pushBox.y+my[i^2]])flag=i;
                        else if(wall[pushBox.x+mx[i^3]][pushBox.y+my[i^3]])flag=i;
                    }
                    if(flag!=-1)
                    {
                        if(wall[pushBox.x+mx[flag]+mx[flag^2]][pushBox.y+my[flag]+my[flag^2]])continue;
                        if(wall[pushBox.x+mx[flag]+mx[flag^3]][pushBox.y+my[flag]+my[flag^3]])continue;
                    }
                }/////////////////////////////////////////////////////////////////////////
                next.box.insert(pushBox);
            }
            if(searched.find(next)!=searched.end())continue;
            searched.insert(make_pair(next,now));
            next.solveH();
            if(next.h==0)
            {
                next.h=-1;
                ans.resize(next.g);
                for(int i=next.g-1;i>=0;i--)
                {
                    ans[i]=next.pre;
                    map<Status,Status>::iterator it=searched.find(next);
                    next=it->second;
                }
                return ans;
            }
            ts.insert(next);
        }
    }
    ans.resize(0);
    return ans;
}

void Game::restart()
{
    while(!oldStatus.empty())oldStatus.pop();
    player=startStatus;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(wall[i][j])mp[i][j]=WALL;
            else
            {
                if(isAim[i][j])mp[i][j]=AIM;
                else mp[i][j]=0;
            }
        }
    }
    mp[player.man.x][player.man.y]+=MAN;
    for(set<Point>::iterator i=player.box.begin();i!=player.box.end();i++)
        mp[i->x][i->y]+=BOX;
}

bool Game::isWin()
{
    for(set<Point>::iterator i=player.box.begin();i!=player.box.end();i++)
        if(!isAim[i->x][i->y])
            return false;
    return true;
}

void Game::move(int k)
{
    if(k<0||k>=4)return;
    Point p(player.man.x+mx[k],player.man.y+my[k]);
    if(wall[p.x][p.y])return;
    if(player.box.find(p)!=player.box.end())
    {
        Point pp(p.x+mx[k],p.y+my[k]);
        if(wall[pp.x][pp.y])return;
        if(player.box.find(pp)!=player.box.end())return;
        oldStatus.push(player);
        mp[p.x][p.y]-=BOX;
        player.box.erase(p);
        mp[pp.x][pp.y]+=BOX;
        player.box.insert(pp);
    }
    else oldStatus.push(player);
    mp[player.man.x][player.man.y]-=MAN;
    player.man=p;
    mp[player.man.x][player.man.y]+=MAN;
    player.g++;
}


vector<int> Game::move(int x,int y)
{
    vector<int>v(0);
    if(wall[x][y]||(player.man.x==x&&player.man.y==y))return v;
    if(player.box.find(Point(x,y))!=player.box.end())
    {
        for(int i=0;i<4;i++)
        {
            if(x==player.man.x+mx[i]&&y==player.man.y+my[i])
            {
                v.push_back(i);
                return v;
            }
        }
        return v;
    }

    queue<Point>q;
    int **searched=new int*[n];
    for(int i=0;i<n;i++)
    {
        searched[i]=new int[m];
        for(int j=0;j<m;j++)searched[i][j]=-1;
    }
    searched[player.man.x][player.man.y]=4;
    q.push(player.man);
    bool flag=false;
    while(!q.empty())
    {
        Point now=q.front(),next;q.pop();
        for(int i=0;i<4;i++)
        {
            next.x=now.x+mx[i];
            next.y=now.y+my[i];
            if(searched[next.x][next.y]!=-1)continue;
            searched[next.x][next.y]=i;
            if(wall[next.x][next.y]||player.box.find(next)!=player.box.end())continue;
            if(next.x==x&&next.y==y)
            {
                flag=true;
                break;
            }
            q.push(next);
        }
        if(flag)break;
    }
    if(searched[x][y]!=-1)
    {
        Point now(x,y);
        while(searched[now.x][now.y]!=4)
        {
            int t=searched[now.x][now.y];
            v.push_back(t);
            now.x-=mx[t];
            now.y-=my[t];
        }
        reverse(v.begin(),v.end());
    }
    for(int i=0;i<n;i++)delete[] searched[i];
    delete[] searched;
    return v;
}

int Game::getStep()
{
    return player.g;
}

void Game::run()
{
    if(ans.size())
    {
        minGH=0;
        return;
    }
    solve();
    minGH=0;
}

int Game::getH()
{
    startStatus.solveH();
    return minGH=startStatus.h;
}

