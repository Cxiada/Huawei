//
// Created by XDchen on 2019/3/17.
//

#include "Floyd.h"
using namespace std;

//Floyd算法    参考https://blog.csdn.net/glb007/article/details/82562837
vector<vector<int>> zuiduan(int n, vector<Car> &cars,
                            map<int, int > &corssid_map, vector<vector<int>> &L){
    int dis[n][n];        //存储源点到各个顶点的最短路径
    vector<int> path[n][n];
    for (int i = 0; i < n; i++)              //初始化
    {

        for (int j = 0; j < n; j++)
        {
            dis[i][j] = L[i][j];
            path[i][j].push_back(i+1);
            path[i][j].push_back(j+1);
        }
    }
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                //dis[i] = min(dis[i],dis[j] + L[j][i]);
                if (dis[k][i] > dis[k][j] + L[j][i])               //求源点到节点的最短路径，利用现有的L矩阵
                {
                    dis[k][i] = dis[k][j] + L[j][i];

                    path[k][i].clear();                         //保存并更新路径
                    path[k][i].insert(path[k][i].end(), path[k][j].begin(),path[k][j].end());
                    path[k][i].push_back(i+1);
                }
            }
            for (int m = 0; m < i; m++)              //更新节点最短路径
            {
                for(int j = 0; j < n; j++)
                {
                    if (dis[k][m] > dis[k][j] + L[j][m])
                    {
                        dis[k][m] = dis[k][j] + L[j][m];
                        path[k][m].clear();                     //保存并更新路径
                        path[k][m].insert(path[k][m].end(), path[k][j].begin(), path[k][j].end());
                        path[k][m].push_back(m + 1);
                    }
                }
            }
        }
    }
    vector<vector<int>> answers;
    for (int i = 0; i < cars.size(); ++i) {
        int from=corssid_map[cars[i].from];
        int to=corssid_map[cars[i].to];
        vector<int> answer;
        for (int j = 0; j <path[from][to].size() ; ++j) {
            answer.push_back(path[from][to][j]);
        }
        answers.push_back(answer);
    }
    return answers;
}

vector<vector<int>> Floyd_init(vector<Cross> &crosses, map<int, Road* > &road_map,
                               map<int, int > &corssid_map){
    vector<vector<int>> L;
    for (int i = 0; i < crosses.size(); ++i) {
        vector<int> temp={};
        for (int j = 0; j < crosses.size(); ++j) {
            temp.push_back(inf);
        }
        L.push_back(temp);
    }
    for (int i = 0; i < crosses.size(); ++i) {
        if (crosses[i].up != -1){
            int from=corssid_map[crosses[i].idx];
            int to=crosses[i].to_crossidx[0];
            if (to != -1)
                L[from][corssid_map[to]]=road_map[crosses[i].up]->length;
        }
        if (crosses[i].right != -1){
            int from=corssid_map[crosses[i].idx];
            int to=crosses[i].to_crossidx[1];
            if (to != -1)
                L[from][corssid_map[to]]=road_map[crosses[i].right]->length;
        }
        if (crosses[i].down != -1){
            int from=corssid_map[crosses[i].idx];
            int to=crosses[i].to_crossidx[2];
            if (to != -1)
                L[from][corssid_map[to]]=road_map[crosses[i].down]->length;
        }
        if (crosses[i].left != -1){
            int from=corssid_map[crosses[i].idx];
            int to=crosses[i].to_crossidx[3];
            if (to != -1)
                L[from][corssid_map[to]]=road_map[crosses[i].left]->length;
        }

    }
    return L;
}