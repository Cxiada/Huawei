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

const int inf = 999999;
vector<vector<int>> zuiduan(int n, vector<Car> &cars,
                            map<int, int > &corssid_map, vector<vector<int>> &L);

vector<vector<int>> Floyd_init(vector<Cross> &crosses, map<int, Road* > &road_map,
                               map<int, int > &corssid_map);

#endif //HUAWEI_FLOYD_H
