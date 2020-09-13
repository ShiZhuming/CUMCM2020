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

int main()
{
    for (int i = 0; i <= 27; ++i)
    {
        vector<int> a;
        graph.push_back(a);
    }
    fstream fin("B-1-1邻接表.txt", ios::in);
    for (int i = 1; i <= 27; ++i)
    {
        int a;
        while (fin >> a && a != -1)
        {
            graph[i].push_back(a);
        }
    }
    fin.close();
    pair<int, string> d[6] = {bfs(1, 15), bfs(1, 12), bfs(1, 27), bfs(15, 12), bfs(15, 27), bfs(12, 27)};

    for (int initwater = 98; initwater <= 400; ++initwater)
    {
        for (int initfood = 98; initfood <= 600; ++initfood)
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
                //去村庄
                {
                state next = t;
                next.p = 15;
                for (int i = d[0].first; i > 0 && next.d <= 30;)
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
                next.path = t.path + d[0].second;
                qu.push(next);
                }
                /*
                //去终点
                {
                state next = t;
                next.p = 27;
                for (int i = d[2].first; i > 0;)
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
                next.path = t.path + d[2].second;
                qu.push(next);
                }
                */

                break;
            }
            case 12:
            {
                if (t.f <= 12 || t.w <= 10 || t.d > 25)
                    break;//无法回到村庄或终点
                if (t.f < 50 || t.w < 50 || t.d == 13)
                {//去村庄,只在食物或水少于50的情况下考虑
                    state next = t;
                    next.p = 15;
                    for (int i = d[3].first; i > 0&&next.d <= 30;)
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
                    next.path = t.path + "-13-15";
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
                if (t.d == 18)
                {//休息一天
                    state next = t;
                    next.d += 1;
                    next.f -= foodcost[weather[next.d]];
                    next.w -= watercost[weather[next.d]];
                    next.path = t.path + "-rest";
                    qu.push(next);
                }
                break;
            }
            case 15:
            {
                if (t.d<=22)
                {//枚举买了多少水和食物，去矿山
                for (int buywater = 0; buywater <= 400; ++buywater)
                {
                    for (int buyfood = 0; buyfood <= 600; ++buyfood)
                    {
                        state next = t;
                        int restmoney = t.money - 10 * buywater - 20 * buyfood;
                        if (2 * (buyfood + t.f) + 3 * (buywater + t.w) >= 1199 &&
                            2 * (buyfood + t.f) + 3 * (buywater + t.w) <= 1200 && restmoney >= 0
                        && (buywater + t.w)>=55 && (buyfood + t.f)>=66)
                        {
                            next.f += buyfood, next.w += buywater, next.money = restmoney;
                            next.villagebuyfood += buyfood, next.villagebuywater += buywater;
                        }
                        else if (2 * (buyfood + t.f) + 3 * (buywater + t.w) > 1200)
                            break;
                        else
                            continue;
                        next.p = 12;
                        for (int i = d[3].first; i > 0&&next.d <= 30;)
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
                        next.path = t.path + d[3].second;
                        qu.push(next);
                    }
                }
                }

                {//计算好还需要多少水和食物，然后去终点
                    state next = t;
                    next.p = 27;
                    for (int i = d[4].first; i > 0&&next.d <= 30;)
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
                    next.path += d[4].second;
                    qu.push(next);
                }
                break;
            }
            case 27:
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
/*
剪枝条件：
1、枚举在起点购买的水和食物数量时，从到达村庄所需的最小数量开始枚举，并且以达到负重上限为条件（这是因为起点的水和食物价格较便宜）。
2、不考虑从起点直接去终点的情况（因为已经找到比直接去终点收益更高的方法）。
3、所有路线在经过村庄的情况下最短距离不变，所以可以认为都从村庄经过，减少了路线的选择。
4、在矿山时，只有当所剩水和食物较少时才考虑去村庄。直接剪掉所剩水和食物或天数不足以到达村庄或终点的情况。
5、不考虑除了沙暴以外的在路上停留（因为消耗资源必然变多，而且损失了一天时间）。
6、在村庄枚举购买水和食物数量时，以达到负重上限为条件。如果到终点还有剩余的，再加上相应的钱数（可以视作原来在村庄少买了一些）。
*/