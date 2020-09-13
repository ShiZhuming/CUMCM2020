#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
vector<vector<int> > graph;
int weather[11] = {};//1-晴朗，2-高温，3-沙暴
int watercost[4] = {0, 3, 9, 10};
int foodcost[4] = {0, 4, 9, 10};

class node
{
public:
    int point;
    string path;
    int step;
    node(int n):point(n){}
};

class state
{
public:
    int p;//当前所在位置
    int d;//日期
    int f;//食物
    int w;//水
    int m;//挖矿次数
    int money;
    int villagebuyfood = 0;//在村庄买的食物
    int villagebuywater = 0;//在村庄买的水
    string path;
    state(int pp, int dd, int ff, int ww, int mm, int moneyy, string pathh) : p(pp), d(dd), f(ff), w(ww), m(mm), money(moneyy), path(pathh){}
    state(){}
};
queue<state> qu;
pair<int, string> bfs(int start, int end)
{
    bool visited[30] = {};
    queue<node> q;
    node s = node(start);
    s.step = 0;
    q.push(s);
    while (!q.empty())
    {
        node t = q.front();
        q.pop();
        visited[t.point] = true;
        if (t.point == end)
        {
            return make_pair(t.step, t.path);
        }
        for (int i = 1; i < graph[t.point].size(); ++i)
        {
            if (visited[graph[t.point][i]])
                continue;
            node nextnode(graph[t.point][i]);
            nextnode.step = t.step + 1;
            nextnode.path = t.path + "-" + to_string(nextnode.point);
            q.push(nextnode);
        }
    }
    return make_pair(-1, "");
}
pair<int, string> d[14][14] = {};

int conutmoney(int start)
{
    for (int initwater = 18; initwater <= 400; ++initwater)
    {
        for (int initfood = 24; initfood <= 600; ++initfood)
        {
            if ((2 * initfood + 3 * initwater) >= 1199 && (2 * initfood + 3 * initwater) <= 1200)
            {
                if (start==1)
                {
                    qu.push(state(start, 1, initfood - foodcost[weather[1]], initwater - watercost[weather[1]],
                                  0, 10000 - 5 * initwater - 10 * initfood, "1-rest"));
                }
                else
                {
                    qu.push(state(start, 1, initfood - 2 * foodcost[weather[1]], initwater - 2 * watercost[weather[1]],
                                  0, 10000 - 5 * initwater - 10 * initfood, "1-" + to_string(start)));
                }
                
            }
            if (2 * initfood + 3 * initwater > 1200)
                break;
        }
    }
    int maxm = 0;
    string anspath;
    state ans;
    while (!qu.empty())
    {
        state t = qu.front();
        qu.pop();
        if (t.d > 10)
            continue;
        if (t.f < 0 || t.w < 0)
        {
            continue;
        }
        //printf("t.d=%d, t.p=%d, t.w=%d, t.f=%d, t.m=%d\n", t.d, t.p, t.w, t.f, t.m);
        if (t.p == start)
            {
                //去矿山
                {
                state next = t;
                next.p = 9;
                for (int i = d[start][9].first; i > 0 && next.d <= 10;)
                {
                    next.d += 1;
                    if (weather[next.d] == 3)
                    {
                        next.f -= foodcost[3];
                        next.w -= watercost[3];
                    }
                    else
                    {
                        next.f -= 2 * foodcost[weather[next.d]];
                        next.w -= 2 * watercost[weather[next.d]];
                        i -= 1;
                    }
                }
                next.path = t.path + d[start][9].second;
                qu.push(next);
                }
                
                //去终点
                {
                state next = t;
                next.p = 13;
                for (int i = d[start][13].first; i > 0 && next.d <= 10;)
                {
                    next.d += 1;
                    if (weather[next.d]==3)
                    {
                        next.f -= foodcost[3];
                        next.w -= watercost[3];
                    }
                    else
                    {
                        next.f -= 2 * foodcost[weather[next.d]];
                        next.w -= 2 * watercost[weather[next.d]];
                        i -= 1;
                    }
                }
                next.path = t.path + d[start][13].second;
                qu.push(next);
                }
            }
        if (t.p == 9)
            {
                if (t.f < 16 || t.w < 12 || t.d > 8)
                    continue;

                if (t.f <= 54 || t.w <= 54 || t.d == 8)
                {//去终点,只在食物或水较少或天数将尽的情况下考虑
                    state next = t;
                    next.p = 13;
                    for (int i = d[9][13].first; i > 0&&next.d <= 10;)
                    {
                        next.d += 1;
                        if (weather[next.d] == 3)
                        {
                            next.f -= foodcost[3];
                            next.w -= watercost[3];
                        }
                        else
                        {
                            next.f -= 2 * foodcost[weather[next.d]];
                            next.w -= 2 * watercost[weather[next.d]];
                            i -= 1;
                        }
                    }
                    next.path = t.path + d[9][13].second;
                    qu.push(next);
                }
                
                //挖一天矿
                {
                    state next = t;
                    next.d += 1;
                    next.m += 1;
                    next.f -= 3 * foodcost[weather[next.d]];
                    next.w -= 3 * watercost[weather[next.d]];
                    next.money += 200;
                    next.path = t.path + "-mine";
                    qu.push(next);
                }
                
                {//休息一天
                    state next = t;
                    next.d += 1;
                    next.f -= foodcost[weather[next.d]];
                    next.w -= watercost[weather[next.d]];
                    next.path = t.path + "-rest";
                    qu.push(next);
                }
            }
            
        if (t.p == 13)
            {
                if (t.f>0)
                {
                    t.money += 10 * t.f;
                }
                if (t.w>0)
                {
                    t.money += 5 * t.w;
                }
                if (maxm < t.money)
                {
                    maxm = t.money;
                    anspath = t.path;
                    ans = t;
                }
            }
        
    }
    cout << maxm << " " << anspath << endl;
    return maxm;
}

int main()
{
    for (int i = 0; i <= 13; ++i)
    {
        vector<int> a;
        graph.push_back(a);
    }
    fstream fin("B-2-3邻接表.txt", ios::in);
    for (int i = 1; i <= 13; ++i)
    {
        int a;
        while (fin >> a && a != -1)
        {
            graph[i].push_back(a);
        }
    }
    fin.close();

    for (int i = 1; i <= 13; ++i)
    {
        for (int j = 1; j <= 13; ++j)
        {
            if (i == j)
                continue;
            d[i][j] = bfs(i, j);
        }
    }

    //第一天是晴天
    weather[1] = 1;
    cout << "第一天是晴天\n";
    //假设之后都是晴天
    for (int i = 2; i <= 10; ++i)
        weather[i] = 1;
    cout << "假设之后都是晴天\n";
    cout << conutmoney(1) << endl;//第一天停留
    cout << conutmoney(2) << endl;//第一天去2号区域
    cout << conutmoney(4) << endl;//第一天去4号区域
    cout << conutmoney(5) << endl;//第一天去5号区域
    //假设之后都是晴天
    for (int i = 2; i <= 10; ++i)
        weather[i] = 2;
    cout << "假设之后都是高温\n";
    cout << conutmoney(1) << endl;//第一天停留
    cout << conutmoney(2) << endl;//第一天去2号区域
    cout << conutmoney(4) << endl;//第一天去4号区域
    cout << conutmoney(5) << endl;//第一天去5号区域
    
    
    //第一天是高温
    weather[1] = 2;
    cout << "第一天是高温\n";
    //假设之后都是晴天
    for (int i = 2; i <= 10; ++i)
        weather[i] = 1;
    cout << "假设之后都是晴天\n";
    cout << conutmoney(1) << endl;//第一天停留
    cout << conutmoney(2) << endl;//第一天去2号区域
    cout << conutmoney(4) << endl;//第一天去4号区域
    cout << conutmoney(5) << endl;//第一天去5号区域
    //假设之后都是晴天
    for (int i = 2; i <= 10; ++i)
        weather[i] = 2;
    cout << "假设之后都是高温\n";
    cout << conutmoney(1) << endl;//第一天停留
    cout << conutmoney(2) << endl;//第一天去2号区域
    cout << conutmoney(4) << endl;//第一天去4号区域
    cout << conutmoney(5) << endl;//第一天去5号区域
}
