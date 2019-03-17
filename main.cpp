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


int main() {
    string path1="road.txt";
    vector<Road> roads;
    roads=Road_input(path1);
//    for (auto iter=roads.cbegin();iter !=roads.cend();iter++)
//        cout<<(*iter).idx<<(*iter).length<<(*iter).speed<<(*iter).channel<<(*iter).from
//        <<(*iter).to<<(*iter).isDuplex<<endl;
    string path2="car.txt";
    vector<Car> cars;
    cars=Car_input(path2);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).from<<(*iter).to<<(*iter).speed_max<<(*iter).planTime<<endl;

    string path3="cross.txt";
    vector<Cross> crosses;
    crosses=Cross_input(path3);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).up<<(*iter).right<<(*iter).down<<(*iter).left<<endl;

    string path4="answer.txt";
    vector<Car_answer> answers;
    answers=Car_answer_input(path4);
//    for (auto iter=answers.cbegin();iter !=answers.cend();iter++)
//        cout<<(*iter).idx<<(*iter).planTime<<(*iter).road_id.front()<<endl;
    map<int, Car*> car_map;//car_map从汽车标号到cars下标的映射
    for(int i=0;i<cars.size();++i)
        car_map.insert(pair<int ,Car*>(cars[i].idx,&cars[i]));

    map<int, Road* > road_map;//road_map从road标号到roads下标的映射
    for(int i=0;i<roads.size();++i)
        road_map.insert(pair<int ,Road*>(roads[i].idx,&roads[i]));

    map<int, Cross* > corss_map;//corss_map从corss标号到corsses下标的映射
    for(int i=0;i<crosses.size();++i)
        corss_map.insert(pair<int ,Cross*>(crosses[i].idx,&crosses[i]));

    int time=1;

    for(int i=0;i<answers.size();++i)
    {
        if(answers[i].planTime>time)
        {
            cars[i].state==normal;
            continue;
        }
        vector<int> road_num=isroadempty(road_map,car_map,answers[i].road_id[0],answers[i].idx);//返回车道号，前车ID（注意一车道满，二车道首车的情况）
                                                                                                //车道号，前车id ，有没有空位（0/1）
        if(road_num[2]) //返回可进入的车道号；
        {
            //车入车道
            cars[i].state=finish;

            cars[i].road=answers[i].road_id[0];
            cars[i].r=road_map[(car_map.find(answers[i].idx)->second->road)]->speed;
            cars[i].speed=road_map[car_map[answers[i].idx]->road]->speed;
            cars[i].pre=road_num[1];

            //修改carline
            //更改车的R和V


        } else{
            cars[i].state=finish;

        }

    }
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<", "<<(*iter).road<<","<<(*iter).r<<","<<(*iter).pre<<endl;
//    system("pause");
    int Time=2;
//所有的车初始状态置wait
    for(int i=0;i<cars.size();++i){
        if (cars[i].state==terminal) //到终点的车
            continue;
        cars[i].state=wait;
    }
    vector<int> cross_wait={};
    vector<int> car_wait={};
    for(int i=0;i<answers.size();++i)//能finish的finish 不能就wait
    {

        Car* car=&cars[i];
        if (car->state==terminal || car->planTime<Time)
            continue;

        if (car->pre==-1)//无前车
            if (car->r+car->speed < road_map[car->road]->length){ //是<=还是<
                car->r=car->r+car->speed;
                car->state=finish;
                continue;
            }
            else//出路口的情况
            if (answers[i].road_id[-1]==car->road){//到达终点
                car->state=terminal;
                continue;
            }
            else
            {//出路口,置wait
                car->state=wait;
                vector<int>::iterator location_index=find(answers[i].road_id.begin(),answers[i].road_id.end(),car->road);  //find函数
                int roadid=*(location_index+1);
                cross_wait.push_back(road_map[roadid]->to);
                car_wait.push_back(car->idx);
                continue;
            }
        else //(car->pre!=-1)有前车
        if (car->r+car->speed<car_map[car->pre]->r){//前车和后车距离很远
            car->r=car->r+car->speed;
            car->state=finish;
            continue;
        }
        else //前车和后车距离很近
        if (car_map[car->pre]->state==wait){//前车wait状态,置wait
            car->state=wait;
            auto location_index=find(answers[i].road_id.begin(),answers[i].road_id.end(),car->road);  //find函数
            int roadid=*(location_index+1);
            cross_wait.push_back(road_map[roadid]->to);
            car_wait.push_back(car->idx);
            continue;
        }

        else {//前车finish状态
            car->state = finish;
            car->r = car_map[car->pre]->r - 1;
            continue;
        }

    }
    sort(cross_wait.begin(),cross_wait.end());
    while (!car_wait.empty()){
        for (int i = 0; i < cross_wait.size(); ++i) {
            Cross* cross=corss_map[cross_wait[i]];
            int flag=1;
            Car* car_left=nullptr;
            Car* car_down=nullptr;
            Car* car_right=nullptr;
            Car* car_up= nullptr;
//            if (!road_map[cross->left]->Carline.empty())
//                car_left=road_map[cross->left]->Carline;

        }


    }


//
//    ++time;
//    //void init_state();//所有的车初始状态置wait
//    for(int i=0;i<answers.size();++i)
////    {
//
////             //能finish的finish 能wait的wait
//               //存路口号
//               //
////             //
////    }
//        //路口号按ID升序
//        while (所有车状态都改) {
//            for (cross_id) {
//
//                for(road_cross)
//                {
//                    //保存四条道路第一优先级车的行驶方向
//                    int i=-1;
//                    while(有没有finish 的车)
//                    {
//                        ++i;
//                        i%=4;
//                        //判断是否冲突，是跳出，否走车，改变状态若车finish 后面所有车都finish，判断死循环
//
//
//                    }
//
//
//                }
//
//            }
//        }
}
