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


    string path3 = "../config/cross.txt";
    vector<Cross> crosses;
    crosses = Cross_input(path3);


    string path4 = "../config/answer.txt";
    vector<Car_answer> answers;
    answers = Car_answer_input(path4);

    map<int, Car *> car_map;//car_map从汽车标号到cars下标的映射
    map<int, Car_answer *> car2answer_map;//car2answer_map从汽车idx到answer的映射
    for (int i = 0; i < cars.size(); ++i) {
        car_map.insert(pair<int, Car *>(cars[i].idx, &cars[i]));
        car2answer_map.insert(pair<int, Car_answer *>(cars[i].idx, &answers[i]));
    }


    map<int, Road *> road_map;//road_map从road标号到roads下标的映射
    for (int i = 0; i < roads.size(); ++i)
        road_map.insert(pair<int, Road *>(roads[i].idx, &roads[i]));

    map<int, Cross *> corss_map;//corss_map从corss标号到corsses下标的映射
    for (int i = 0; i < crosses.size(); ++i)
        corss_map.insert(pair<int, Cross *>(crosses[i].idx, &crosses[i]));

    int Time = 1;

    for (int i = 0; i < answers.size(); ++i) {
        if (answers[i].planTime > Time) {
            cars[i].state == normal;
            continue;
        }
        vector<int> road_num = isroadempty(road_map, car_map, answers[i].road_id[0],
                                           answers[i].idx);//返回车道号，前车ID（注意一车道满，二车道首车的情况）
        //车道号，前车id ，有没有空位（0/1）
        if (road_num[2]) //返回可进入的车道号；
        {
            //车入车道
            cars[i].state = finish;

            cars[i].road = answers[i].road_id[0];
            cars[i].r = road_map[(car_map.find(answers[i].idx)->second->road)]->speed;
            cars[i].speed = road_map[car_map[answers[i].idx]->road]->speed;
            cars[i].pre = road_num[1];

            //修改carline
            //更改车的R和V


        } else {
            cars[i].state = finish;

        }

    }
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<", "<<(*iter).road<<","<<(*iter).r<<","<<(*iter).pre<<endl;
//    system("pause");
    vector<int> car_terminal;
    while (car_terminal.size() < answers.size()) {
        Time++;
//        cout<<Time<<"   road:"<<cars[0].road<<"    r:"<<cars[0].r<<endl;
//所有的车初始状态置wait
        for (int i = 0; i < cars.size(); ++i) {
            if (cars[i].state == terminal) //到终点的车
                continue;
            cars[i].state = wait;
        }
        vector<int> cross_wait = {};
        vector<int> car_wait = {};
        for (int i = 0; i < answers.size(); ++i)//能finish的finish 不能就wait
        {

            Car *car = &cars[i];
            if (car->state == terminal || car->planTime > Time)
                continue;

            if (car->pre == -1)//无前车
                if (car->r + car->speed <= road_map[car->road]->length) {
                    car->r = car->r + car->speed;
                    car->state = finish;
                    continue;
                } else//出路口的情况
                if (answers[i].road_id.back() == car->road) {//到达终点
                    car->state = terminal;
                    car_terminal.push_back(car->idx);
                    continue;
                } else {//出路口,置wait
                    car->state = wait;
                    vector<int>::iterator location_index = find(answers[i].road_id.begin(), answers[i].road_id.end(),
                                                                car->road);  //find函数
                    int roid = *(location_index);
                    int roid2 = *(location_index + 1);
                    int jionid = roid_jion(road_map[*(location_index)], road_map[*(location_index + 1)]);
                    cross_wait.push_back(jionid);
                    car_wait.push_back(car->idx);
                    continue;
                }
            else //(car->pre!=-1)有前车
            if (car->r + car->speed < car_map[car->pre]->r) {//前车和后车距离很远
                car->r = car->r + car->speed;
                car->state = finish;
                continue;
            } else //前车和后车距离很近
            if (car_map[car->pre]->state == wait) {//前车wait状态,置wait
                car->state = wait;
                auto location_index = find(answers[i].road_id.begin(), answers[i].road_id.end(), car->road);  //find函数
                int jionid = roid_jion(road_map[*(location_index)], road_map[*(location_index + 1)]);
                cross_wait.push_back(jionid);
                car_wait.push_back(car->idx);
                continue;
            } else {//前车finish状态
                car->state = finish;
                car->r = car_map[car->pre]->r - 1;
                continue;
            }

        }
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<", "<<(*iter).state<<","<<(*iter).r<<","<<(*iter).pre<<endl;

        sort(cross_wait.begin(), cross_wait.end());//路口号排序
        cross_wait.erase(unique(cross_wait.begin(), cross_wait.end()), cross_wait.end());//清除重复路口号

        while (!car_wait.empty()) {
            for (int i = 0; i < cross_wait.size(); ++i) {
                Cross *cross = corss_map[cross_wait[i]];
                vector<Car *> car_prior;
                vector<Road *> road_wait;
//            cycle_start(cross, &car_prior, &road_wait);
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
                int cnt = -1;
                int flag = 1;
                while (flag != 0) {
                    ++cnt;
                    if (cnt == 4) flag = 0;
                    cnt %= 4;
                    if (!car_prior[cnt]) continue; //车走完了
//                if (冲突)
//                    continue;
//                else{
                    auto location_index = find(car2answer_map[car_prior[cnt]->idx]->road_id.begin(), //find函数
                                               car2answer_map[car_prior[cnt]->idx]->road_id.end(), road_wait[cnt]->idx);
                    int roadid = *(location_index + 1);
                    int id = road_wait[cnt]->idx;
/*车入车道;*/    bool confict = car_move(car_prior[cnt], road_map[roadid], cross->idx, road_map, car_map);
/*车的状态改变;*/
/*改变后面的车的状态;*/
/*car_wait变化/++flag;*/
                    if (confict) {
                        auto index = find(car_wait.begin(), car_wait.end(), car_prior[cnt]->idx);
                        car_wait.erase(index);
                        flag++;
                        Car *temp = (findfirstpriority(road_map[car_prior[cnt]->road], cross->idx, car_map));
                        car_prior[cnt]->road = roadid;
                        car_prior[cnt] = temp;
                    }
//            }
//
                }
                if (car_prior[0] == nullptr and car_prior[1] == nullptr and car_prior[2] == nullptr and
                    car_prior[3] == nullptr) {
                    auto index = find(cross_wait.begin(), cross_wait.end(), cross->idx);
                    cross_wait.erase(index);
                }

//            if (!road_map[cross->left]->Carline.empty())
//                car_left=road_map[cross->left]->Carline;

            }


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
    cout << Time << endl;
    return 0;
}

