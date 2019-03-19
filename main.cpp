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

#include "input.h"
#include "Floyd.h"
#include "Estimate.h"
using namespace std;




int cmp(Car_answer ac1, Car_answer ac2){
    if(ac1.road_id.size()<ac2.road_id.size())
        return 1;
    else
        return 0;
}

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

int main() {
//输入操作
    string path1 = "../config/road.txt";
    vector<Road> roads;
    roads = Road_input(path1);

    string path2 = "../config/car.txt";
    vector<Car> cars;
    cars = Car_input(path2);


    string path3 = "../config/cross.txt";
    vector<Cross> crosses;
    crosses = Cross_input(path3);


//    string path4 = "../config/answer.txt";
//    vector<Car_answer> answers;
//    answers = Car_answer_input(path4);

    map<int, Car *> car_map;//car_map从汽车标号到cars下标的映射
//    map<int, Car_answer *> car2answer_map;//car2answer_map从汽车idx到answer的映射
    for (int i = 0; i < cars.size(); ++i) {
        car_map.insert(pair<int, Car *>(cars[i].idx, &cars[i]));
//        car2answer_map.insert(pair<int, Car_answer *>(cars[i].idx, &answers[i]));
    }

    map<int, Road *> road_map;//road_map从road标号到roads下标的映射
    for (int i = 0; i < roads.size(); ++i)
        road_map.insert(pair<int, Road *>(roads[i].idx, &roads[i]));

    map<int, Cross *> corss_map;//corss_map从corss标号到corsses下标的映射
    for (int i = 0; i < crosses.size(); ++i)
        corss_map.insert(pair<int, Cross *>(crosses[i].idx, &crosses[i]));

    for (int i = 0; i < crosses.size(); ++i) {   //路口初始化
        crosses[i].set_tocrossidx(road_map);
    }

        string path4 = "../config/answer.txt";
    vector<Car_answer> answer_raw;
    answer_raw = Car_answer_input(path4);
    sort(answer_raw.begin(), answer_raw.end(),cmp);//路口号排序

    vector<Car_answer> answers(answer_raw.begin(),answer_raw.begin()+20);


    map<int, Car_answer *> car2answer_map;//car2answer_map从汽车idx到answer的映射
//    for (int i = 0; i < cars.size(); ++i) {
//        car2answer_map.insert(pair<int, Car_answer *>(cars[i].idx, &answers[i]));
//    }
    for (int i = 0; i < answers.size(); ++i) {
        car2answer_map.insert(pair<int, Car_answer *>(answers[i].idx, &answers[i]));
    }

    int Time = 0;
    vector<int> car_terminal;
    while (car_terminal.size() < answers.size()) {
        Time++;
        if (Time%1000==0)
            cout<<Time<<endl;
//        if (Time==448)
//            cout<<"在这里停顿"<<endl;

        vector<int> cross_wait = {};
        vector<int> car_wait = {};
        Wait2finish(answers, Time, car_terminal, cars, car_map, road_map, cross_wait, car_wait);
        sort(cross_wait.begin(), cross_wait.end());//路口号排序
        cross_wait.erase(unique(cross_wait.begin(), cross_wait.end()), cross_wait.end());//清除重复路口号

        while (!car_wait.empty()) {
            int cars_num=car_wait.size();
            for (int i = 0; i < cross_wait.size(); ++i) {

                Cross *cross = corss_map[cross_wait[i]];
                vector<Car *> cars_prior;//up，right，down，left
                vector<Road *> road_wait;//up，right，down，left
                cycle_start( cars_prior, road_wait ,cross, road_map, car_map);

                int cnt = -1;
                int flag = 1;
                while (flag != 0) {
                    ++cnt;
                    if (cnt == 4) {  //一次循环的标志位
                        flag = 0;
                        cnt = 0;
                    }

                    while ( cars_prior[cnt] ){  //车没走完
                        if ( ! is_car_can_go(cars_prior, cnt, car_map, cross, car2answer_map ))
                            break;
                        //车能走
                        /*car_wait变化/++flag;*/
                        /*车的状态改变;*/
                        /*改变Carline;*/

                        auto index = find(car_wait.begin(), car_wait.end(), cars_prior[cnt]->idx);
                        car_wait.erase(index);
                        flag++;
                        auto location_index = find(car2answer_map[cars_prior[cnt]->idx]->road_id.begin(),
                                                   car2answer_map[cars_prior[cnt]->idx]->road_id.end(),
                                                   road_wait[cnt]->idx);
                        int roadid = *(location_index + 1); //下条道路
                        if (car_move(cars_prior[cnt], road_map[roadid], cross->idx, road_map, car_map)) {
                            Carline_clean(road_wait[cnt], cars_prior[cnt]->idx, cross->idx, car_map);
                        } else{
                            cars_prior[cnt]->state=finish;
                            cars_prior[cnt]->r=road_map[cars_prior[cnt]->road]->length;
                        }
                        cars_prior[cnt] = (findfirstpriority(road_wait[cnt], cross->idx, car_map));
                    }

                }
                if (cars_prior[0] == nullptr and cars_prior[1] == nullptr and cars_prior[2] == nullptr and
                    cars_prior[3] == nullptr) {
                    auto index = find(cross_wait.begin(), cross_wait.end(), cross->idx);
                    cross_wait.erase(index);
                }

            }
            if (cars_num==car_wait.size()){
                cout<<"Time:   "<<Time<<"   fail"<<endl;
//                int  n;
//                cin>> n;
//                break;
            }

        }

    }

    cout << Time << endl;
    return 0;
}

