//
// Created by XDchen on 2019/3/19.
//

#include "Estimate.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stack>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>

//3.19修改
vector<int> isroadempty(map<int, Car*>& m_car,Road* road, int from) {
//车道号，前车id(空为-1) ，有没有空位（0/1）
    vector<int>res={0, -1, 0};

    if(from==road->from)
    {
        for (int i = 0; i < road->channel; ++i) {
            if ( road->Carline[i].empty() ){
                res[0]=i;
                res[1]=-1;
                res[2]=1;
                return res;
            } else
                if (m_car[road->Carline[i].back()]->r >1){
                    res[0]=i;
                    res[1]=road->Carline[i].back();
                    res[2]=1;
                    return res;
                }
        }
        return res;


    } else{
        for (int i = 0; i < road->channel; ++i) {
            if ( road->Carline2[i].empty() ){
                res[0]=i;
                res[1]=-1;
                res[2]=1;
                return res;
            } else
            if (m_car[road->Carline2[i].back()]->r >1){
                res[0]=i;
                res[1]=road->Carline2[i].back();
                res[2]=1;
                return res;
            }
        }
        return res;


    }
}
//车道号，前车id(空为-1) ，有没有空位（0/1）


int where_car_go_next(Car *car,map<int, Car_answer* > car_answer_map) {//返回该车下一时刻走的道路号

    auto it = find(car_answer_map[car->idx]->road_id.begin(),car_answer_map[car->idx]->road_id.end(),car->road);
    int id_carnextroad=*(it+1);

    return id_carnextroad;
}



//3.19修改:没有考虑空指针，地址报错
bool is_car_can_go(vector<Car*>& cars_prior, int id_car,map<int, Car*>& m_car,Cross* cross,
                   map<int, Car_answer *> car_answer_map) {
    //cars 四个路上的第一优先级的车指针，id_car 当前要判断的车在cars中的下标
    vector<int>cross1;
    cross1.push_back(cross->up);
    cross1.push_back(cross->right);
    cross1.push_back(cross->down);
    cross1.push_back(cross->left);//上右下左

    int id_nowcar_nextroad=where_car_go_next(m_car[cars_prior[id_car]->idx],car_answer_map);
    int id_nowcar_road=cars_prior[id_car]->road;
    auto it=find(cross1.begin(),cross1.end(),id_nowcar_road);
    int it1=it-cross1.begin();//当前车所在道路在cross1中的下标
    //若当前车直行
    if(id_nowcar_nextroad==cross1[(it1+2)%4])
    {
    }
        //若当前车左拐
    else if(id_nowcar_nextroad==cross1[(it1+1)%4])
    {
        if (cars_prior[(id_car+3)%4]){
            int id_nowcar_nextroad1=where_car_go_next(m_car[cars_prior[(id_car+3)%4]->idx],car_answer_map);//3.19修改:'+5'-->'+3'
            if(id_nowcar_nextroad==id_nowcar_nextroad1)
                return false;
        }
    }
        //若当前车右拐
    else if(id_nowcar_nextroad==cross1[(it1+3)%4])//3.19修改:'+5'-->'+3'
    {
        if (cars_prior[(id_car + 1) % 4] and cars_prior[(id_car + 2) % 4]) {
            int id_nowcar_nextroad1 = where_car_go_next(m_car[cars_prior[(id_car + 1) % 4]->idx], car_answer_map);
            int id_nowcar_nextroad2 = where_car_go_next(m_car[cars_prior[(id_car + 2) % 4]->idx], car_answer_map);
            if (id_nowcar_nextroad == id_nowcar_nextroad1 || id_nowcar_nextroad == id_nowcar_nextroad2)
                return false;
        }
    }

    return true;
}


bool car_move(Car *car, Road *road, int id_cross,map<int, Road* >& m_road,map<int, Car*>& m_car) {
    //3.19修改:成功的话要改变1、2、3、4、5、6
    //失败返回false
    vector<int> flag1=isroadempty(m_car, road, id_cross);//3.19修改:这个函数会变动，记得兼容
    int r_car=car->r;
    int id=car->idx;
    if(flag1[2]==1) //车道号，前车id(空为-1) ，有没有空位（0/1）
    {
        if (car->road==-1)
            r_car=0;
        else
            r_car=m_road[car->road]->length-r_car;//计算前车道可走长度

        if(flag1[1]==-1)//前面没车
            r_car=min(car->speed_max,road->speed)-r_car;
        else{
            r_car=min( min(car->speed_max, road->speed)-r_car, m_car[flag1[1]]->r-1 );
            if (r_car==m_car[flag1[1]]->r-1 and m_car[flag1[1]]->state==wait)
                return false;
        }


        if(r_car<=0)
            return false;

        //更改车的参数
        car->state=finish;
        car->road=road->idx;
        car->speed=min(car->speed_max,road->speed);
        car->pre=flag1[1];
        car->r=r_car;


        if (id_cross==road->from)
            road->Carline[flag1[0]].push_back(car->idx);
        else road->Carline2[flag1[0]].push_back(car->idx);

        return true;
    } else
        return false;

}
//3.19修改:成功的话要改变1、2、3、4、5、6
//失败返回false




void Carline_clean(Road* road,int car_id, int cross, map<int, Car *>& car_map){
    if (cross==road->to){
        for (int i = 0; i < road->Carline.size(); ++i) {
            if (road->Carline[i].front()==car_id){
                road->Carline[i].erase(road->Carline[i].begin());
                if ( !road->Carline[i].empty())
                    car_map[road->Carline[i].front()]->pre=-1;
                break;
            }
        }
    } else
        for (int i = 0; i < road->Carline2.size(); ++i) {
        if (road->Carline2[i].front()==car_id){
            road->Carline2[i].erase(road->Carline2[i].begin());
            if ( !road->Carline2[i].empty())
                car_map[road->Carline2[i].front()]->pre=-1;
            break;
        }
    }

}
//Carline_clean Carline去掉前车,后车pre=-1

void Carline_move(Road* road,int car_id, int cross, vector<int> &car_wait, map<int, Car *>& car_map){
    if (cross==road->to){
        for (int i = 0; i < road->Carline.size(); ++i) {
            if (road->Carline[i].front()==car_id){
                for (int j = 1; j < road->Carline[i].size(); ++j) {
                    Car* car=car_map[road->Carline[i][j]];
                    Car* car_pre=car_map[road->Carline[i][j-1]];
                    if (car->state==wait) {
                        if (car->speed + car->r < car_pre->r and car->state) {
                            car->state = finish;
                            car->r = car->speed + car->r;
                        } else {
                            car->state = finish;
                            car->r = car_pre->r - 1;
                        }
                        auto index = find(car_wait.begin(), car_wait.end(), car->idx);
                        car_wait.erase(index);
                    }

                }
                break;
            }
        }
    } else
        for (int i = 0; i < road->Carline2.size(); ++i) {
            if (road->Carline2[i].front()==car_id){
                for (int j = 1; j < road->Carline2[i].size(); ++j) {
                    Car* car=car_map[road->Carline2[i][j]];
                    Car* car_pre=car_map[road->Carline2[i][j-1]];
                    if (car->state==wait) {
                        if (car->speed + car->r < car_pre->r) {
                            car->state = finish;
                            car->r = car->speed + car->r;
                        } else {
                            car->state = finish;
                            car->r = car_pre->r - 1;
                        }
                        auto index = find(car_wait.begin(), car_wait.end(), car->idx);
                        car_wait.erase(index);
                    }
                }
                break;
            }
        }
}
//Carline_move Carline后车移动


int road_jion(Road* road, Road* toroad){
    if (road->from==toroad->from)
        return road->from;
    if (road->from==toroad->to)
        return road->from;
    if (road->to==toroad->from)
        return road->to;
    if (road->to==toroad->to)
        return road->to;
}
//返回两条道路的交点号

Car* findfirstpriority(Road *road,int to,map<int, Car*>& m_car) {
    int channel=road->channel;
    if(road->to==to)
    {
        Car* car= nullptr;
        for (int i = 0; i < channel; ++i) {
            if (!road->Carline[i].empty())
                if (m_car[road->Carline[i].front()]->state ==wait)
                    if (car !=nullptr){
                        if (car->r<m_car[road->Carline[i].front()]->r)
                            car=m_car[road->Carline[i].front()];
                    } else car=m_car[road->Carline[i].front()];
        }
        return car;
    }
    else{
        Car* car= nullptr;
        for (int i = 0; i < channel; ++i) {
            if (!road->Carline2[i].empty())
                if (m_car[road->Carline2[i].front()]->state ==wait)
                    if (car !=nullptr){
                        if (car->r<m_car[road->Carline2[i].front()]->r)
                            car=m_car[road->Carline2[i].front()];
                    } else car=m_car[road->Carline2[i].front()];
        }
        return car;
    }


}
//找到第一优先车辆



void Wait2finish(vector<Car_answer> &answers,int Time, vector<int> &car_terminal,
                 vector<Car> &cars, map<int, Car *> &car_map,map<int, Road *> &road_map,
                 vector<int> &cross_wait, vector<int> &car_wait,map<int, Car_answer *> car2answer_raw_map ){
    //所有的车初始状态置wait
    for (int i = 0; i < answers.size(); ++i) {
        if (car_map[answers[i].idx]->state == terminal or car_map[answers[i].idx]->state == normal) //到终点的车或未出发的车
            continue;
        car_map[answers[i].idx]->state = wait;
    }
    int car_finish=0;
    int flag=-1;
    while ( car_finish-flag>0 ) {
        flag=car_finish;

        for (int i = 0; i < answers.size(); ++i)//能finish的finish 不能就wait
        {

            Car *car = car_map[answers[i].idx];
//            if (car->idx==10865){
//                cout<<"";
//                int u=0;
//            }
            if (car->state == terminal || car->state == finish || answers[i].planTime > Time)
                continue;

            if (car->state == normal) { //3.19修改:到发车时间了
                if (car_move(car, road_map[answers[i].road_id[0]], car->from, road_map, car_map)){
                    car_finish++;
                    continue;
                }

                else {
                    car->state=normal;
                    answers[i].planTime++;
                    car2answer_raw_map[car->idx]->planTime++;
//                    car->state = wait;
                    //修正3.20:前面堵车要处理
//                    cross_wait.push_back(car->from);
//                    car_wait.push_back(car->idx);
                    continue;
                }
            }
            if (car->pre == -1)//无前车
                if (car->r + car->speed <= road_map[car->road]->length) {
                    car->r = car->r + car->speed;
                    car->state = finish;
                    car_finish++;
                    continue;
                }
                else  //无前车,要出路口（到终点）
                if (answers[i].road_id.back() == car->road) { //到达终点
                    car->state = terminal;
                    car_terminal.push_back(car->idx);
                    Carline_clean(road_map[car->road], car->idx, car->to, car_map);
                    car_finish++;
                    //3.19修改:增加清理Carline
                    continue;
                }
                else { //无前车,出路口,置wait
                    car->state = wait;
//                    auto location_index = find(answers[i].road_id.begin(), answers[i].road_id.end(),
//                                               car->road);  //道路号搜索
//                    int jionid = road_jion(road_map[*(location_index)], road_map[*(location_index + 1)]);
//                    cross_wait.push_back(jionid);
//                    car_wait.push_back(car->idx);
                    continue;
                }
            else //(car->pre!=-1)出车库
            if (car->r + car->speed < car_map[car->pre]->r) { //前车和后车距离很远
                car->r = car->r + car->speed;
                car->state = finish;
                car_finish++;
                continue;
            } else //前车和后车距离很近
            if (car_map[car->pre]->state == wait) { //前车wait状态,置wait
                car->state = wait;
//                auto location_index = find(answers[i].road_id.begin(), answers[i].road_id.end(), car->road);  //道路号搜索
//                int jionid = road_jion(road_map[*(location_index)], road_map[*(location_index + 1)]);
//                cross_wait.push_back(jionid);
//                car_wait.push_back(car->idx);
                continue;
            } else { //前车finish状态
                car->state = finish;
                car->r = car_map[car->pre]->r - 1;
                car_finish++;
                continue;
            }

        }
    }
    for (int i = 0; i < answers.size(); ++i) {
        Car *car = car_map[answers[i].idx];
        if (car->state == terminal || car->state == finish || answers[i].planTime > Time)
            continue;
//        if (car->road==-1){
//            cross_wait.push_back(car->from);
//            car_wait.push_back(car->idx);
//        } else {
            auto location_index = find(answers[i].road_id.begin(), answers[i].road_id.end(), car->road);  //道路号搜索
            if ( *location_index == answers[i].road_id.back())  //前方终点
                cross_wait.push_back(car->to);
            else{
                int jionid = road_jion(road_map[*(location_index)], road_map[*(location_index + 1)]);
                cross_wait.push_back(jionid);
            }

            car_wait.push_back(car->idx);
//        }
    }
}


void cycle_start(vector<Car*> &car_prior, vector<Road *> &road_wait,Cross* cross,
                 map<int,Road *> &road_map,map<int, Car *> &car_map){

    if (cross->up != -1) {
        Road *road_ = road_map[cross->up];
        road_wait.push_back(road_);
        car_prior.push_back(findfirstpriority(road_, cross->idx, car_map));
    } else {
        road_wait.push_back(nullptr);
        car_prior.push_back(nullptr);
    }
    if (cross->right != -1) {
        Road *road_ = road_map[cross->right];
        road_wait.push_back(road_);
        car_prior.push_back(findfirstpriority(road_, cross->idx, car_map));
    } else {
        road_wait.push_back(nullptr);
        car_prior.push_back(nullptr);
    }
    if (cross->down != -1) {
        Road *road_ = road_map[cross->down];
        road_wait.push_back(road_);
        car_prior.push_back(findfirstpriority(road_, cross->idx, car_map));
    } else {
        road_wait.push_back(nullptr);
        car_prior.push_back(nullptr);
    }
    if (cross->left != -1) {
        Road *road_ = road_map[cross->left];
        road_wait.push_back(road_);
        car_prior.push_back(findfirstpriority(road_, cross->idx, car_map));
    } else {
        road_wait.push_back(nullptr);
        car_prior.push_back(nullptr);
    }
}
