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
    if(ac1.planTime<ac2.planTime)
        return 1;
    else
        return 0;
}
//int cmp(Car_answer ac1, Car_answer ac2){
//    if(ac1.road_id.size()<ac2.road_id.size())
//        return 1;
//    else
//        return 0;
//}


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
    for (int i = 0; i < cars.size(); ++i) {
        car_map.insert(pair<int, Car *>(cars[i].idx, &cars[i]));
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
    string path5 = "../config/answer_raw.txt";//最终输出文件
    vector<vector<int>> L=creat_map(cars, crosses, road_map, corss_map, path4);
    vector<Car_answer> answer_raw;
    answer_raw = Car_answer_input(path4);
    sort(answer_raw.begin(), answer_raw.end(),cmp);//出发时间
    int pr_strat=2000;//一共在地图上跑的车数量
    vector<Car_answer> answers(answer_raw.begin(),answer_raw.begin()+pr_strat);
        map<int, Car_answer *> car2answer_raw_map;//car2answer_map从汽车idx到answer_raw的映射
        for (int i = 0; i < answer_raw.size(); ++i) {
            car2answer_raw_map.insert(pair<int, Car_answer *>(answer_raw[i].idx, &answer_raw[i]));
        }
        int Time = 0;
        vector<int> car_terminal;
        while ( !answers.empty() ) {
            Time++;
            //answer里去掉达到终点的car
            //answer加入新车
            //car_terminal清空
            //answer为空<-结束表示
            if ( !car_terminal.empty() ){   //answer里去掉达到终点的car
                for (int i = 0; i < car_terminal.size(); ++i) {
                    for (auto it=answers.begin();it!=answers.end();it++){
                        if ((*it).idx==car_terminal[i]){
                            answers.erase(it);
                            break;
                        }

                    }
                }
                vector<vector<int>> L_temp=L_change(crosses, road_map,L);
                for (int i = 0; i < car_terminal.size(); ++i) {  //answer加入新车
                    if (pr_strat<answer_raw.size()){
                        answer_raw[pr_strat].planTime=Time;
                        answers.push_back(answer_raw[pr_strat]);
                        UpdateRoute_pre(crosses.size(), car_map[answer_raw[pr_strat].idx],
                                answers.back(),L_temp,corss_map,car2answer_raw_map);
                        pr_strat++;
                    } else break;

                }
                car_terminal.clear();
            }
            if (answers.empty()) break;
            map<int, Car_answer *> car2answer_map;//car2answer_map从汽车idx到answer的映射
            for (int i = 0; i < answers.size(); ++i) {
                car2answer_map.insert(pair<int, Car_answer *>(answers[i].idx, &answers[i]));
            }

//            car_map[answers[0].idx]->road=answers[0].road_id[0];
//            UpdateRoute(crosses.size(), answers[0].idx, car2answer_map, L,
//                        car_map,road_map,corss_map,car2answer_raw_map);
//        if (Time%100==0)
            cout<<Time<<"   "<<pr_strat<<endl;
//            cout<<Time<<"   "<<answers.size()-car_terminal.size()<<endl;
//        if (pr_strat==7114)
//            cout<<"在这里停顿"<<endl;

            vector<int> cross_wait = {};
            vector<int> car_wait = {};
            Wait2finish(answers, Time, car_terminal, cars, car_map, road_map, cross_wait, car_wait,car2answer_raw_map);
            sort(cross_wait.begin(), cross_wait.end());//路口号排序
            cross_wait.erase(unique(cross_wait.begin(), cross_wait.end()), cross_wait.end());//清除重复路口号
            //3.22新增
            vector<Car> cars_back=cars;
            vector<Road> roads_back=roads;
            vector<int> cross_wait_back = cross_wait;
            vector<int> car_wait_back = car_wait;
            vector<Car_answer> answers_back=answers;

            while (!car_wait.empty()) {
                int cars_num = car_wait.size();
                for (int i = 0; i < cross_wait.size(); ++i) {

                    Cross *cross = corss_map[cross_wait[i]];
                    vector<Car *> cars_prior;//up，right，down，left
                    vector<Road *> road_wait;//up，right，down，left
                    cycle_start(cars_prior, road_wait, cross, road_map, car_map);

                    int cnt = -1;
                    int flag = 1;
                    int flag_cnt=0;
                    int temp=flag;
                    while ( flag_cnt <4 ) {
                        ++cnt;
                        cnt%=4;
                        if (temp==flag) flag_cnt++;
                        else{
                            temp=flag;
                            flag_cnt=0;
                        }
                        while (cars_prior[cnt]) {  //车没走完
                            //前方出不了路口
                            if (cars_prior[cnt]->r + cars_prior[cnt]->speed <= road_wait[cnt]->length) {
                                auto index = find(car_wait.begin(), car_wait.end(), cars_prior[cnt]->idx);
                                car_wait.erase(index);
                                flag++;
                                cars_prior[cnt]->state = finish;
                                cars_prior[cnt]->r = cars_prior[cnt]->r + cars_prior[cnt]->speed;
                                Carline_move(road_wait[cnt], cars_prior[cnt]->idx, cross->idx, car_wait, car_map);
                                cars_prior[cnt] = (findfirstpriority(road_wait[cnt], cross->idx, car_map));
                                continue;
                            }
                            //前方到达终点
                            if (cars_prior[cnt]->to == cross->idx) {
                                cars_prior[cnt]->state = terminal;
                                car_terminal.push_back(cars_prior[cnt]->idx);
                                auto index = find(car_wait.begin(), car_wait.end(), cars_prior[cnt]->idx);
                                car_wait.erase(index);
                                flag++;
                                Carline_clean(road_wait[cnt], cars_prior[cnt]->idx, cross->idx, car_map);
                                cars_prior[cnt] = (findfirstpriority(road_wait[cnt], cross->idx, car_map));
                                continue;
                            }
                            //去下一车道
                            if (!is_car_can_go(cars_prior, cnt, car_map, cross, car2answer_map))
                                break;
                            //车的行驶优先级最高
                            /*car_wait变化/++flag;*/
                            /*车的状态改变;*/
                            /*改变Carline;*/
                            auto location_index = find(car2answer_map[cars_prior[cnt]->idx]->road_id.begin(),
                                                       car2answer_map[cars_prior[cnt]->idx]->road_id.end(),
                                                       road_wait[cnt]->idx);
                            Road*  road_next = road_map[*(location_index + 1)]; //下条道路
                            if (car_move(cars_prior[cnt], road_next, cross->idx, road_map, car_map)) {
                                Carline_clean(road_wait[cnt], cars_prior[cnt]->idx, cross->idx, car_map);
                            } else { //正好卡在路口(堵车或者停在路口)
                                if (road_wait[cnt]->length-cars_prior[cnt]->r < min(road_next->speed, cars_prior[cnt]->speed_max) ){
                                    //堵车情况
                                    if (road_next->from == cross->idx) //前车未走
                                        if (!road_next->Carline[road_next->channel - 1].empty())
                                            if (car_map[road_next->Carline[road_next->channel - 1].back()]->state == wait)
                                                break;
                                    if (road_next->to == cross->idx) //前车未走
                                        if (!road_next->Carline2[road_next->channel - 1].empty())
                                            if (car_map[road_next->Carline2[road_next->channel - 1].back()]->state == wait)
                                                break;
                                }
                                cars_prior[cnt]->state = finish;
                                cars_prior[cnt]->r = road_map[cars_prior[cnt]->road]->length;
                                Carline_move(road_wait[cnt], cars_prior[cnt]->idx, cross->idx, car_wait, car_map);
                            }
                            auto index = find(car_wait.begin(), car_wait.end(), cars_prior[cnt]->idx);
                            car_wait.erase(index);
                            flag++;
                            cars_prior[cnt] = (findfirstpriority(road_wait[cnt], cross->idx, car_map));
                        }

                    }
                    if (cars_prior[0] == nullptr and cars_prior[1] == nullptr and cars_prior[2] == nullptr and
                        cars_prior[3] == nullptr) {
                        auto index = find(cross_wait.begin(), cross_wait.end(), cross->idx);
                        cross_wait.erase(index);
                    }

                }

                if (cars_num == car_wait.size()) {
                    cout << "Time:   " << Time << "   fail   "<<endl;
                    for (int kk = 0; kk < car_wait.size(); ++kk) {
                        cout<<"car:   " << car_wait[kk]<<endl;
                    }
                    for (int kk = 0; kk < cross_wait.size(); ++kk) {
                        cout<<"cross:   " << cross_wait[kk]<<endl;
                    }

                    answers.clear();
                    answers.insert(answers.end(),answers_back.begin(),answers_back.end());

                    for (int i = 0; i < car_wait.size(); ++i) {
                        UpdateRoute(crosses.size(), car_wait[i], car2answer_map, L,
                                    car_map,road_map,corss_map,car2answer_raw_map);
                    }

                    cars.clear(); //3.22新增
                    cars.insert(cars.end(),cars_back.begin(),cars_back.end());
                    roads.clear();
                    roads.insert(roads.end(),roads_back.begin(),roads_back.end());
                    cross_wait.clear();
                    cross_wait.insert(cross_wait.end(),cross_wait_back.begin(),cross_wait_back.end());
                    car_wait.clear();
                    car_wait.insert(car_wait.end(),car_wait_back.begin(),car_wait_back.end());


                }

            }

        }
            cout << Time << endl;

    ofstream file(path5);
    for (int i = 0; i < answer_raw.size(); ++i) {
        auto answer=answer_raw[i];
        file<<"("<<answer.idx<<", "<<answer.planTime<<", ";
        for (int j = 0; j < answer.road_id.size()-1; ++j) {
            file<<answer.road_id[j]<<", ";
        }
        if (i==answer_raw.size()-1)
            file<<answer.road_id.back()<<")";
        else
            file<<answer.road_id.back()<<")"<<"\n";
    }
    file.close();
    return 0;
}

