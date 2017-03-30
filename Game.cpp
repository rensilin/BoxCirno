#include "Game.h"

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
                    startStatus.box.push(Point(i,j));
                    break;
                case '0':
                    startStatus.man=Point(i,j);
                    break;
                case '+':
                    startStatus.box.push(Point(i,j)); //no break
                case '$':
                    aim.push(Point(i,j));
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

const Vector<int>& Game::solve()
{
    rbtree<Status> ts;
    TreeMap<Status,Status>searched;
    startStatus.h=-1;
    searched.push(startStatus,startStatus);
    startStatus.h=minGH;
    ts.push(startStatus);
//    int cnt=0;
    while(ts.size())
    {
        if(minGH==-1)return ans;
//        cnt++;
//    		if(cnt%10000==0)printf("%d %d\n",cnt,ts.size());
//    		printf("%d: ",ts.size());
//    		for(rbtree<Status>::iterator it=ts.begin();it!=ts.end();it++)
//    		{
//
//			    int cnt=0;
//			    for(TreeSet<Point>::iterator j=it->box.begin();j!=it->box.end();j++)
//			        if(isAim[j->x][j->y])
//			            cnt++;
//				printf("(%d,%d,%d)",it->man.x,it->man.y,cnt);
//		    }
//			printf("\n");
        Status now(*ts.begin());ts.del(ts.begin());
        minGH=min(minGH,now.h);
        now.h=-1;now.g++;
//    		        printf("(%d,%d):",now.man.x,now.man.y);
//    				for(TreeSet<Point>::iterator it=now.box.begin();it!=now.box.end();it++)
//    				    printf(" (%d,%d)",it->x,it->y);
//    				printf("\n");
        for(int i=0;i<4;i++)
        {
            if(minGH==-1)return ans;
            Status next(now);
            next.pre=i;
            next.man.x+=mx[i];
            next.man.y+=my[i];
            if(wall[next.man.x][next.man.y])continue;
            if(next.box.del(next.man))
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
                next.box.push(pushBox);
            }
            if(searched.find(next)!=searched.end())continue;
            searched.push(next,now);
            //printf("(%d,%d)",searched.find(next)->value.man.x,searched.find(next)->value.man.y);
            next.solveH();
//    			int cnt=0;
//				for(TreeSet<Point>::iterator j=next.box.begin();j!=next.box.end();j++)
//			        if(isAim[j->x][j->y])
//			            cnt++;
//			    if(cnt==4)printf("(%d,%d,%d)\n",next.man.x,next.man.y,next.h);
            if(next.h==0)
            {
                next.h=-1;
                for(int i=next.g-1;i>=0;i--)
                {
                    ans[i]=next.pre;
                    TreeMap<Status,Status>::iterator it=searched.find(next);
//					    printf("(%d,%d)->(%d,%d)(%d,%d)\n",it->value.man.x,it->value.man.y,it->key.man.x,it->key.man.y,next.man.x,next.man.y);
                    next=it->value;
                }
//					cout<<cnt<<endl;
                return ans;
            }
//    				printf("    (%d,%d):",next.man.x,next.man.y);
//    				for(TreeSet<Point>::iterator it=next.box.begin();it!=next.box.end();it++)
//    				    printf(" (%d,%d)",it->x,it->y);
//    				printf("\n");
            ts.push(next);
        }
    }
    return ans;
}

void Game::restart()
{
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
    for(TreeSet<Point>::iterator i=player.box.begin();i!=player.box.end();i++)
        mp[i->x][i->y]+=BOX;
}

bool Game::isWin()
{
    for(TreeSet<Point>::iterator i=player.box.begin();i!=player.box.end();i++)
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
        mp[p.x][p.y]-=BOX;
        player.box.del(p);
        mp[pp.x][pp.y]+=BOX;
        player.box.push(pp);
    }
    mp[player.man.x][player.man.y]-=MAN;
    player.man=p;
    mp[player.man.x][player.man.y]+=MAN;
    player.g++;
}

int Game::getStep()
{
    return player.g;
}

void Game::run()
{
    if(ans.getLength())
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

