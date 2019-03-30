//
// Created by XDchen on 2019/3/17.
//

#ifndef HUAWEI_FLOYD_H
#define HUAWEI_FLOYD_H

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
#include <map>
#include <algorithm>
#include "Estimate.h"
const int inf = 99999;
vector<vector<int>> zuiduan(int n, vector<Car> &cars, vector<vector<int>> &L);

vector<vector<int>> Floyd_init(vector<Cross> &crosses, map<int, Road* > &road_map);

vector<vector<int>> L_change(vector<Cross> &crosses, map<int, Road* > &road_map,
                             vector<vector<int>> L);

vector<vector<int>> creat_map(vector<Car> &cars, vector<Cross> &crosses,
               map<int, Road *> &road_map, map<int, Cross *> &corss_map,string path);

void UpdateRoute(int n, const int car_wait, map<int, Car_answer *> &car2answer_map,  vector<vector<int>> L,
                 const map<int, Car *> car_map,const map<int, Road *> road_map,map<int, Cross *> &corss_map,
                 map<int, Car_answer *> car2answer_raw_map);

void UpdateRoute_pre(int n, int p_beg, int P_end,   vector<vector<int>> L,map<int, Cross *> &corss_map,
                     map<int, Car *> car_map, vector<Car_answer> &answer_raw);

//void UpdateRoute_pre(int n, const Car* car, Car_answer &answer,  vector<vector<int>> L,
//                     map<int, Cross *> &corss_map, map<int, Car_answer *> car2answer_raw_map);

#endif //HUAWEI_FLOYD_H
