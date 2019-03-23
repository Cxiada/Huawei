//
// Created by XDchen on 2019/3/17.
//

#include "Floyd.h"
using namespace std;

//Floyd算法    参考https://blog.csdn.net/glb007/article/details/82562837
vector<vector<int>> zuiduan(int n, vector<Car> &cars, vector<vector<int>> &L){
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
        int from=cars[i].from-1;
        int to=cars[i].to-1;
        vector<int> answer;
        for (int j = 0; j <path[from][to].size() ; ++j) {
            answer.push_back(path[from][to][j]);
        }
        answers.push_back(answer);
    }
    return answers;
}

vector<vector<int>> Floyd_init(vector<Cross> &crosses, map<int, Road* > &road_map){
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
            int from=crosses[i].idx;
            int to=crosses[i].to_crossidx[0];
            if (to != -1)
                L[from-1][to-1]=100/road_map[crosses[i].up]->channel;
        }
        if (crosses[i].right != -1){
            int from=crosses[i].idx;
            int to=crosses[i].to_crossidx[1];
            if (to != -1)
                L[from-1][to-1]=100/road_map[crosses[i].right]->channel;
        }
        if (crosses[i].down != -1){
            int from=crosses[i].idx;
            int to=crosses[i].to_crossidx[2];
            if (to != -1)
                L[from-1][to-1]=100/road_map[crosses[i].down]->channel;
        }
        if (crosses[i].left != -1){
            int from=crosses[i].idx;
            int to=crosses[i].to_crossidx[3];
            if (to != -1)
                L[from-1][to-1]=100/road_map[crosses[i].left]->channel;
        }

    }
    return L;
}

vector<vector<int>>  creat_map(vector<Car> &cars, vector<Cross> &crosses,
               map<int, Road *> &road_map, map<int, Cross *> &corss_map, string path){
    int n = crosses.size();
    vector<vector<int>> my_answers = {};
    vector<vector<int>> L = Floyd_init(crosses, road_map);
    my_answers = zuiduan(n, cars, L);
    vector<Car_answer> answers_One;


    for (int i = 0; i < my_answers.size(); ++i) {
        auto answer = my_answers[i];
        vector<int> road_id;
        Car_answer car_Oneanswer(cars[i].idx, cars[i].planTime, road_id);
        for (int j = 1; j < answer.size(); ++j) {
            Cross *cross = corss_map[answer[j - 1]];
            int to = answer[j];
            if (cross->to_crossidx[0] == to)
                road_id.push_back(cross->up);
            else if (cross->to_crossidx[1] == to)
                road_id.push_back(cross->right);
            else if (cross->to_crossidx[2] == to)
                road_id.push_back(cross->down);
            else if (cross->to_crossidx[3] == to)
                road_id.push_back(cross->left);
        }
        car_Oneanswer.road_id = road_id;
        answers_One.push_back(car_Oneanswer);

    }
    vector<int> planTime;
    planTime.push_back(0);
    for (int i = 0; i < answers_One.size(); ++i) {
        auto answer = answers_One[i];
        int time = 0;
        for (int j = 0; j < answer.road_id.size(); ++j) {
            time += road_map[answer.road_id[j]]->length;
        }
        time=time/cars[i].speed;
        planTime.push_back(time+planTime.back());
    }
    for (int k = 0; k < planTime.size(); ++k) {
//        cout<<planTime[k]<<endl;
    }
//    for (int i = 1; i < answers_One.size(); ++i) {
//        answers_One[i].planTime=planTime[i];
//    }
    ofstream file(path);
    for (int i = 0; i < answers_One.size(); ++i) {
        auto answer=answers_One[i];
        file<<"("<<answer.idx<<", "<<answer.planTime<<", ";
        for (int j = 0; j < answer.road_id.size()-1; ++j) {
            file<<answer.road_id[j]<<", ";
        }
        if (i==answers_One.size()-1)
            file<<answer.road_id.back()<<")";
        else
            file<<answer.road_id.back()<<")"<<"\n";
    }
    file.close();
    return L;
}



//3.22新增
void UpdateRoute(int n, const int car_wait, map<int, Car_answer *> &car2answer_map,  vector<vector<int>> L,
                 const map<int, Car *> car_map,const map<int, Road *> road_map,map<int, Cross *> &corss_map,
                 map<int, Car_answer *> car2answer_raw_map) {
    //获得当前路口号
    int road_car=car_map.find(car_wait)->second->road;
    int i=0;
    while(car2answer_map[car_wait]->road_id[i]!=road_car)
    {
        ++i;
    }
    int idx_road_id=i+1;
    if ( i==car2answer_map[car_wait]->road_id.size()-1 )
        return;
    int nextroad_car=car2answer_map[car_wait]->road_id[i+1];
    int cross_car=road_jion(road_map.find(road_car)->second,road_map.find(nextroad_car)->second);
    int anothercross_nextroad_car;
    if(cross_car==road_map.find(nextroad_car)->second->from)
        anothercross_nextroad_car=road_map.find(nextroad_car)->second->to;
    else
        anothercross_nextroad_car=road_map.find(nextroad_car)->second->from;
    int anothercross_road_car;
    if(cross_car==road_map.find(road_car)->second->from)
        anothercross_road_car=road_map.find(road_car)->second->to;
    else
        anothercross_road_car=road_map.find(road_car)->second->from;
    //更改地图
    L[cross_car-1][anothercross_nextroad_car-1]=inf/2;
    L[cross_car-1][anothercross_road_car-1]=inf;
    int from=cross_car-1;
    int to=car_map.find(car_wait)->second->to-1;
    //重新调用floyd算法计算新的最短路径
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
    int k=from;

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

    //更改当前车的路径
        vector<int> road_id;
    for (int j = 1; j < path[from][to].size(); ++j) {
        Cross *cross = corss_map[path[from][to][j - 1]];
        int to_road = path[from][to][j];
        if (cross->to_crossidx[0] == to_road)
            road_id.push_back(cross->up);
        else if (cross->to_crossidx[1] == to_road)
            road_id.push_back(cross->right);
        else if (cross->to_crossidx[2] == to_road)
            road_id.push_back(cross->down);
        else if (cross->to_crossidx[3] == to_road)
            road_id.push_back(cross->left);
    }
    car2answer_map[car_wait]->road_id.erase(car2answer_map[car_wait]->road_id.begin()+idx_road_id,car2answer_map[car_wait]->road_id.end());
    car2answer_map[car_wait]->road_id.insert(car2answer_map[car_wait]->road_id.end(),road_id.begin(),road_id.end());

    car2answer_raw_map[car_wait]->road_id.erase(car2answer_raw_map[car_wait]->road_id.begin()+idx_road_id,car2answer_raw_map[car_wait]->road_id.end());
    car2answer_raw_map[car_wait]->road_id.insert(car2answer_raw_map[car_wait]->road_id.end(),road_id.begin(),road_id.end());

}