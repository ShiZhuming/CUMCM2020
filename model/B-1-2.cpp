#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
vector<vector<int> > graph;
int weather[31] = {0, 2, 2, 1, 3, 1, 2, 3, 1, 2, 2,
                   3, 2, 1, 2, 2, 2, 3, 3, 2, 2,
                   1, 1, 2, 1, 3, 2, 1, 1, 2, 2};//1-晴朗，2-高温，3-沙暴
int watercost[4] = {0, 5, 8, 10};
int foodcost[4] = {0, 7, 6, 10};
pair<int, string> d[7][7] = {};

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
    bool visited[70] = {};
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
        for (int i = 0; i < graph[t.point].size(); ++i)
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

int main()
{
    for (int i = 0; i <= 64; ++i)
    {
        vector<int> a;
        graph.push_back(a);
    }
    for (int i = 1; i <= 64; ++i)
    {
        if (i % 8 != 1)
        {
            graph[i].push_back(i - 1);
            graph[i - 1].push_back(i);
        }
        if (i > 8)
        {
            if (((i - 1) / 8) % 2 == 1)//偶数行
            {
                if (i % 16 != 0)
                {
                    graph[i].push_back(i - 7);
                    graph[i - 7].push_back(i);
                }
                graph[i].push_back(i - 8);
                graph[i - 8].push_back(i);
            }
            if (((i - 1) / 8) % 2 == 0)//奇数行
            {
                if (i % 16 != 1)
                {
                    graph[i].push_back(i - 9);
                    graph[i - 9].push_back(i);
                }
                graph[i].push_back(i - 8);
                graph[i - 8].push_back(i);
            }
        }
    }
    d[1][2] = bfs(1, 30), d[1][3] = bfs(1, 39), d[1][4] = bfs(1, 55), d[1][5] = bfs(1, 62), d[1][6] = bfs(1, 64);
    d[2][1] = bfs(30, 1), d[2][3] = bfs(30, 39), d[2][4] = bfs(30, 55), d[2][5] = bfs(30, 62), d[2][6] = bfs(30, 64);
    d[3][1] = bfs(39, 1), d[3][2] = bfs(39, 30), d[3][4] = bfs(39, 55), d[3][5] = bfs(39, 62), d[3][6] = bfs(39, 64);
    d[4][1] = bfs(55, 1), d[4][2] = bfs(55, 30), d[4][3] = bfs(55, 39), d[4][5] = bfs(55, 62), d[4][6] = bfs(55, 64);
    d[5][1] = bfs(62, 1), d[5][2] = bfs(62, 30), d[5][3] = bfs(62, 39), d[5][4] = bfs(62, 55), d[5][6] = bfs(62, 64);
    d[6][1] = bfs(64, 1), d[6][2] = bfs(64, 30), d[6][3] = bfs(64, 39), d[6][4] = bfs(64, 55), d[6][5] = bfs(64, 62);
    
    for (int initwater = 130; initwater <= 400; ++initwater)
    {
        for (int initfood = 122; initfood <= 600; ++initfood)
        {
            if (1199 <= (2 * initfood + 3 * initwater) && (2 * initfood + 3 * initwater) <= 1200)
            {
                qu.push(state(1, 0, initfood, initwater, 0, 10000 - 5 * initwater - 10 * initfood, "1"));
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
        if (t.d > 30)
            continue;
        if (t.f < 0 || t.w < 0)
        {
            continue;
        }
        //printf("t.d=%d, t.p=%d, t.w=%d, t.f=%d, t.m=%d\n", t.d, t.p, t.w, t.f, t.m);
        switch(t.p)
        {
            case 1:
            {
                //去矿山30
                {
                state next = t;
                next.p = 30;
                for (int i = d[1][2].first; i > 0 && next.d <= 30;)
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
                next.path = t.path + d[1][2].second;
                qu.push(next);
                }

                //去矿山55
                {
                state next = t;
                next.p = 55;
                for (int i = d[1][4].first; i > 0 && next.d <= 30;)
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
                next.path = t.path + d[1][4].second;
                qu.push(next);
                }

                //去村庄62
                {
                state next = t;
                next.p = 62;
                for (int i = d[1][5].first; i > 0 && next.d <= 30;)
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
                next.path = t.path + d[1][5].second;
                qu.push(next);
                }
                
                //去终点64
                {
                state next = t;
                next.p = 64;
                for (int i = d[1][6].first; i > 0;)
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
                next.path = t.path + d[1][6].second;
                qu.push(next);
                }
                

                break;
            }

            case 30:
            {
                if (t.f <= 12 || t.w <= 10 || t.d > 26)
                    break;//无法回到村庄或终点
                if (t.f < 50 || t.w < 50 || t.d == 26)
                {//去村庄39,只在食物或水少于50的情况下考虑
                    state next = t;
                    next.p = 39;
                    for (int i = d[2][3].first; i > 0&&next.d <= 30;)
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
                    next.path = t.path + d[2][3].second;
                    qu.push(next);
                }
                //挖一天矿
                {
                    state next = t;
                    next.d += 1;
                    next.m += 1;
                    next.f -= 3 * foodcost[weather[next.d]];
                    next.w -= 3 * watercost[weather[next.d]];
                    next.money += 1000;
                    next.path = t.path + "-mine";
                    qu.push(next);
                }
                /*
                if (t.d == 18)
                {//休息一天
                    state next = t;
                    next.d += 1;
                    next.f -= foodcost[weather[next.d]];
                    next.w -= watercost[weather[next.d]];
                    next.path = t.path + "-rest";
                    qu.push(next);
                }
                */
                break;
            }
            case 39:
            {
                if (t.d <= 24)
                {//枚举买了多少水和食物，去矿山30
                    for (int buywater = 0; buywater <= 400; ++buywater)
                    {
                        for (int buyfood = 0; buyfood <= 600; ++buyfood)
                        {
                            state next = t;
                            int restmoney = t.money - 10 * buywater - 20 * buyfood;
                            if (2 * (buyfood + t.f) + 3 * (buywater + t.w) >= 1199 &&
                                2 * (buyfood + t.f) + 3 * (buywater + t.w) <= 1200 && restmoney >= 0 && 
                                (buywater + t.w) >= 41 && (buyfood + t.f) >= 42)
                            {
                                next.f += buyfood, next.w += buywater, next.money = restmoney;
                                next.villagebuyfood += buyfood, next.villagebuywater += buywater;
                            }
                            else if (2 * (buyfood + t.f) + 3 * (buywater + t.w) > 1200)
                                break;
                            else
                                continue;
                            next.p = 30;
                            for (int i = d[3][2].first; i > 0 && next.d <= 30;)
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
                            next.path = t.path + d[3][2].second;
                            qu.push(next);
                        }
                    }
                }

                if (t.d <= 25)
                {//枚举买了多少水和食物，去矿山55
                    for (int buywater = 0; buywater <= 400; ++buywater)
                    {
                        for (int buyfood = 0; buyfood <= 600; ++buyfood)
                        {
                            state next = t;
                            int restmoney = t.money - 10 * buywater - 20 * buyfood;
                            if (2 * (buyfood + t.f) + 3 * (buywater + t.w) >= 1199 &&
                                2 * (buyfood + t.f) + 3 * (buywater + t.w) <= 1200 && restmoney >= 0 && 
                                (buywater + t.w) >= 51 && (buyfood + t.f) >= 54)
                            {
                                next.f += buyfood, next.w += buywater, next.money = restmoney;
                                next.villagebuyfood += buyfood, next.villagebuywater += buywater;
                            }
                            else if (2 * (buyfood + t.f) + 3 * (buywater + t.w) > 1200)
                                break;
                            else
                                continue;
                            next.p = 55;
                            for (int i = d[3][4].first; i > 0 && next.d <= 30;)
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
                            next.path = t.path + d[3][4].second;
                            qu.push(next);
                        }
                    }
                }

                {//计算好还需要多少水和食物，然后去终点
                    state next = t;
                    next.p = 64;
                    for (int i = d[3][6].first; i > 0&&next.d <= 30;)
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
                    if (next.f < 0)
                    {
                        next.money += next.f * 20, next.f = 0;
                    }
                    if (next.w < 0)
                    {
                        next.money += next.w * 10, next.w = 0;
                    }
                    if (next.f > 0)
                    {
                        next.money += 10 * next.f;
                        if (next.f <= next.villagebuyfood)
                            next.money += 10 * next.f;
                        else
                            next.money += 10 * next.villagebuyfood;
                        next.f = 0;
                    }
                    if (next.w > 0)
                    {
                        next.money += 5 * next.w;
                        if (next.w <= next.villagebuywater)
                            next.money += 5 * next.w;
                        else
                            next.money += 5 * next.villagebuywater;
                        next.w = 0;
                    }
                    next.path += d[3][6].second;
                    qu.push(next);
                }
                break;
            }

            case 55:
            {
                if (t.f <= 12 || t.w <= 10 || t.d > 28)
                    break;//无法回到村庄或终点
                if (t.f < 30 || t.w < 30)
                {//去村庄62,只在食物或水少于30的情况下考虑
                    state next = t;
                    next.p = 62;
                    for (int i = d[4][5].first; i > 0&&next.d <= 30;)
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
                    next.path = t.path + d[4][5].second;
                    qu.push(next);
                }
                //挖一天矿
                {
                    state next = t;
                    next.d += 1;
                    next.m += 1;
                    next.f -= 3 * foodcost[weather[next.d]];
                    next.w -= 3 * watercost[weather[next.d]];
                    next.money += 1000;
                    next.path = t.path + "-mine";
                    qu.push(next);
                }
                {//计算好还需要多少水和食物，然后去终点
                    state next = t;
                    next.p = 64;
                    for (int i = d[4][6].first; i > 0&&next.d <= 30;)
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
                    if (next.f < 0 || next.w < 0)
                    {
                        break;
                    }
                    if (next.f > 0)
                    {
                        next.money += 10 * next.f;
                        if (next.f <= next.villagebuyfood)
                            next.money += 10 * next.f;
                        else
                            next.money += 10 * next.villagebuyfood;
                        next.f = 0;
                    }
                    if (next.w > 0)
                    {
                        next.money += 5 * next.w;
                        if (next.w <= next.villagebuywater)
                            next.money += 5 * next.w;
                        else
                            next.money += 5 * next.villagebuywater;
                        next.w = 0;
                    }
                    next.path += d[4][6].second;
                    qu.push(next);
                }
                break;
            }

            case 62:
            {
                
                if (t.d <= 26)
                {//枚举买了多少水和食物，去矿山55
                    for (int buywater = 0; buywater <= 400; ++buywater)
                    {
                        for (int buyfood = 0; buyfood <= 600; ++buyfood)
                        {
                            state next = t;
                            int restmoney = t.money - 10 * buywater - 20 * buyfood;
                            if (2 * (buyfood + t.f) + 3 * (buywater + t.w) >= 1199 &&
                                2 * (buyfood + t.f) + 3 * (buywater + t.w) <= 1200 && restmoney >= 0 && 
                                (buywater + t.w) >= 41 && (buyfood + t.f) >= 42)
                            {
                                next.f += buyfood, next.w += buywater, next.money = restmoney;
                                next.villagebuyfood += buyfood, next.villagebuywater += buywater;
                            }
                            else if (2 * (buyfood + t.f) + 3 * (buywater + t.w) > 1200)
                                break;
                            else
                                continue;
                            next.p = 55;
                            for (int i = d[5][4].first; i > 0 && next.d <= 30;)
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
                            next.path = t.path + d[5][4].second;
                            qu.push(next);
                        }
                    }
                }

                {//计算好还需要多少水和食物，然后去终点
                    state next = t;
                    next.p = 64;
                    for (int i = d[5][6].first; i > 0&&next.d <= 30;)
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
                    if (next.f < 0)
                    {
                        next.money += next.f * 20, next.f = 0;
                    }
                    if (next.w < 0)
                    {
                        next.money += next.w * 10, next.w = 0;
                    }
                    if (next.f > 0)
                    {
                        next.money += 10 * next.f;
                        if (next.f <= next.villagebuyfood)
                            next.money += 10 * next.f;
                        else
                            next.money += 10 * next.villagebuyfood;
                        next.f = 0;
                    }
                    if (next.w > 0)
                    {
                        next.money += 5 * next.w;
                        if (next.w <= next.villagebuywater)
                            next.money += 5 * next.w;
                        else
                            next.money += 5 * next.villagebuywater;
                        next.w = 0;
                    }
                    next.path += d[5][6].second;
                    qu.push(next);
                }
                break;
            }

            case 64:
            {
                if (maxm < t.money)
                {
                    maxm = t.money;
                    anspath = t.path;
                    ans = t;
                }
                break;
            }
        }
    }
    cout << maxm << " " << anspath << endl;
}