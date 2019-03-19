//
// Created by XDchen on 2019/3/19.
//

#ifndef HUAWEI_ESTIMATE_H
#define HUAWEI_ESTIMATE_H

#include "input.h"
vector<int> isroadempty(map<int, Car*>& m_car,Road* road,int from);

bool car_move(Car *car, Road *road, int id_cross,map<int, Road* >& m_road,map<int, Car*>& m_car);

void Carline_clean(Road* road,int car_id, int cross, map<int, Car *>& car_map);

Car* findfirstpriority(Road *road,int to,map<int, Car*>& m_car);

int road_jion(Road* road, Road* toroad);


void Wait2finish(vector<Car_answer> &answers,int Time, vector<int> &car_terminal,
                 vector<Car> &cars, map<int, Car *> &car_map,map<int, Road *> &road_map,
                 vector<int> &cross_wait, vector<int> &car_wait );

void cycle_start(vector<Car*> &car_prior, vector<Road *> &road_wait,Cross* cross,
                 map<int,Road *> &road_map,map<int, Car *> &car_map);

#endif //HUAWEI_ESTIMATE_H
