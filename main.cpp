#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stack>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "input.h"
#include <vector>
#include <map>
#include <algorithm>
#define normal -1
#define wait 2
#define finish 3
#define terminal 4

#include "Floyd.h"
using namespace std;
vector<int> isroadempty(map<int, Road* >& m_road,map<int, Car*>& m_car,int num_road,int id_car);

vector<int> isroadempty(map<int,Road* >& m_road,map<int, Car*>& m_car,int num_road,int id_car) {
    int num_roads=0;
    vector<int>res;
    Road *roads=(m_road.find(num_road)->second);
    int num_channel=0;
    int car_front=-1;
    int from=m_car.find(id_car)->second->from;
    if((*roads).Carline2[0].size()==0&&(*roads).Carline[0].size()==0)
    {
        car_front=-1;
        res.push_back(num_channel);
        res.push_back(car_front);
        res.push_back(1);
        if((*roads).from==m_car.find(id_car)->second->from)

            (*roads).Carline[num_channel].push_back(id_car);
        else
            (*roads).Carline2[num_channel].push_back(id_car);
        return  res;
    }
    if((*roads).from==m_car.find(id_car)->second->from)
    {
        int cycle=0;
       while ((*roads).Carline[cycle].size()==(*roads).length)
       {
           ++cycle;
       }
        num_channel=cycle;
        if((*roads).Carline[cycle].size()!=0)
        {
            car_front=(*roads).Carline[num_channel][(*roads).Carline[num_channel].size()-1];
        } else{
            car_front=-1;
            ++num_channel;
        }

        res.push_back(num_channel);
        res.push_back(car_front);
        if((*roads).Carline[(*roads).Carline.size()-1].size()<(*roads).length)
            res.push_back(1);
        else
            res.push_back(0);

        (*roads).Carline[num_channel].push_back(id_car);

    } else{
        int cycle=0;
        while ((*roads).Carline[cycle].size()==(*roads).length)
        {
            ++cycle;
        }
        num_channel=cycle;
        if((*roads).Carline2[cycle].size()!=0)
        {
            car_front=(*roads).Carline2[num_channel][(*roads).Carline2[num_channel].size()-1];
        } else{
            car_front=-1;
            ++num_channel;
        }

        res.push_back(num_channel);
        res.push_back(car_front);
        if((*roads).Carline2[(*roads).Carline2.size()-1].size()<(*roads).length)
            res.push_back(1);
        else
            res.push_back(0);
        (*roads).Carline2[num_channel].push_back(id_car);

    }
    return res;
}

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

bool car_move(Car *car, Road *road, int id_cross,map<int, Road* >& m_road,map<int, Car*>& m_car) {
    vector<int> flag1=isroadempty(m_road,m_car,road->idx,car->idx);
    int r_car=car->r;
    if(flag1[2]==1)
    {
        if(id_cross==road->from)
        {
            (m_road[road->idx])->Carline[flag1[0]].push_back(car->idx);
            r_car=m_road[car->road]->length-r_car;//计算剩余可走长度
            if(flag1[1]!=-1)
                r_car=max(0,min(road->speed-r_car,m_car[flag1[1]]->r-1));//计算到达后的r
            else
                r_car=max(0,road->speed-r_car);
            //更改车的参数
            car->r=r_car;
            car->pre=flag1[1];
            car->state=finish;
        } else{
            (m_road[road->idx])->Carline2[flag1[0]].push_back(car->idx);
            r_car=m_road[car->road]->length-r_car;//计算剩余可走长度
            if(flag1[1]!=-1)
                r_car=max(0,min(road->speed-r_car,m_car[flag1[1]]->r-1));//计算到达后的r
            else
                r_car=max(0,road->speed-r_car);
            //更改车的参数
            car->r=r_car;
            car->pre=flag1[1];
            car->state=finish;
            car->speed=min(car->speed_max, road->speed);
        }
    } else
        return false;
    return true;
}

int roid_jion(Road* road, Road* toroad){
    if (road->from==toroad->from)
        return road->from;
    if (road->from==toroad->to)
        return road->from;
    if (road->to==toroad->from)
        return road->to;
    if (road->to==toroad->to)
        return road->to;
}


int main() {
    string path1 = "../config/road.txt";
    vector<Road> roads;
    roads = Road_input(path1);
//    for (auto iter=roads.cbegin();iter !=roads.cend();iter++)
//        cout<<(*iter).idx<<(*iter).length<<(*iter).speed<<(*iter).channel<<(*iter).from
//        <<(*iter).to<<(*iter).isDuplex<<endl;
    string path2 = "../config/car.txt";
    vector<Car> cars;
    cars = Car_input(path2);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).from<<(*iter).to<<(*iter).speed_max<<(*iter).planTime<<endl;

    string path3 = "../config/cross.txt";
    vector<Cross> crosses;
    crosses = Cross_input(path3);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).up<<(*iter).right<<(*iter).down<<(*iter).left<<endl;

//    string path4="answer.txt";
//    vector<Car_answer> answers;
//    answers=Car_answer_input(path4);
//    for (auto iter=answers.cbegin();iter !=answers.cend();iter++)
//        cout<<(*iter).idx<<(*iter).planTime<<(*iter).road_id.front()<<endl;
    map<int, Car *> car_map;//car_map从汽车标号到cars下标的映射

    for (int i = 0; i < cars.size(); ++i) {
        car_map.insert(pair<int, Car *>(cars[i].idx, &cars[i]));
//        car2answer_map.insert(pair<int ,Car_answer*>(cars[i].idx,&answers[i]));
    }


    map<int, Road *> road_map;//road_map从road标号到roads下标的映射
    for (int i = 0; i < roads.size(); ++i)
        road_map.insert(pair<int, Road *>(roads[i].idx, &roads[i]));

    map<int, Cross *> corss_map;//corss_map从corss标号到corsses下标的映射
    for (int i = 0; i < crosses.size(); ++i)
        corss_map.insert(pair<int, Cross *>(crosses[i].idx, &crosses[i]));


    for (int i = 0; i < crosses.size(); ++i) {
        crosses[i].set_tocrossidx(road_map);
    }

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
        cout<<planTime[k]<<endl;
    }
    for (int i = 0; i < answers_One.size(); ++i) {
        answers_One[i].planTime=planTime[i];
    }
    ofstream file("../config/answer.txt");
    for (int i = 0; i < answers_One.size(); ++i) {
        auto answer=answers_One[i];
        file<<"("<<answer.idx<<", "<<answer.planTime<<", ";
        for (int j = 0; j < answer.road_id.size()-1; ++j) {
            file<<answer.road_id[j]<<", ";
        }
        file<<answer.road_id.back()<<")"<<"\n";
    }
    file.close();


//    ofstream file("../config/answer.txt");
//    for (int i = 0; i < my_answers.size(); ++i) {
//        vector<int> answer=my_answers[i];
//        file<<"("<<cars[i].idx<<", "<<cars[i].planTime<<", ";
//        for (int j = 0; j < answer.size()-1; ++j) {
//            file<<answer[j]<<", ";
//        }
//        file<<answer.back()<<")"<<"\n";
//    }
//    file.close();

    return 0;
}

