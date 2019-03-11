//
// Created by XDchen on 2019/3/10.
//

#ifndef HUAWEI_INPUT_H
#define HUAWEI_INPUT_H

#include <vector>
#include <queue>
#include <string>
using namespace std;

class Car{
public:
    int idx=1;
    int from=1;
    int to=1;
    int speed_max=5;
    int planTime=1;

    int speed=5;
    int r=1;
    int state=-1;  //'nromal','wait','finish','end'
    Car(int idx, int from, int to, int speed_max,
        int planTime):idx(idx),from(from),to(to),speed_max(speed_max),planTime(planTime){};

    void Car_init(int idx, int from, int to, int speed_max,
                  int planTime);
};

class Road{
public:
    int idx=1;
    int length=10;
    int speed=5;
    int channel=1;
    int from=1;
    int to=2;
    bool isDuplex= false;
    vector<vector<Car>> Carline;
    vector<vector<Car>> Carline2;
    Road(int idx, int length, int speed, int channel,int from, int to,bool isDuplex
    ):idx(idx),length(length),speed(speed),channel(channel),from(from),
      to(to), isDuplex(isDuplex){};

    void Road_init(int idx, int length, int speed, int channel,
                   int from, int to,bool isDuplex);
};

class Cross{
public:
    int idx=1;
    int up=-1;
    int right=-1;
    int down=-1;
    int left=-1;
    queue<Car> up_waitCar;
    queue<Car> right_waitCar;
    queue<Car> dowm_waitCar;
    queue<Car> left_waitCar;
    Cross(int idx, int up, int right, int down,int left):
            idx(idx), up(up), right(right), down(down), left(left){};
    void Cross_init(int idx, int up, int right, int down,int left);
};

vector<Road> Road_input(string path);

vector<Car> Car_input(string path);

vector<Cross> Cross_input(string path);

class Car_answer{
public:
    int idx=1;
    int planTime=1;
    vector<int> road_id;
    Car_answer(int idx,int planTime, vector<int> road_id):idx(idx),planTime(planTime),road_id(road_id){};
    void Car_answer_init(int idx,int planTime, vector<int> road_id);

};

vector<Car_answer> Car_answer_input(string path);
#endif //HUAWEI_INPUT_H
