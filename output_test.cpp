//
// Created by XDchen on 2019/3/24.
//

#include "output_test.h"
#include <fstream>
#include <algorithm>
void  output(vector<int> car_wait, vector<int> cross_wait,
             vector<Road> roads,map<int, Car *> car_map, map<int, Car_answer *> &car2answer_map){
    ofstream file("../config/output.txt");
    file<<"#  id      road"<<endl;
    for (int i = 0; i < car_wait.size(); ++i) {
        auto car = car_map[car_wait[i]];
        file << "(" << car->idx << ", " << car->road << ", ";
        for (int j = 0; j < car2answer_map[car->idx]->road_id.size(); ++j) {
            if (car2answer_map[car->idx]->road_id[j]!=car->road)
            file<<car2answer_map[car->idx]->road_id[j]<<", ";
            else
                file<<"**"<<car2answer_map[car->idx]->road_id[j]<<"**"<<", ";
        }
        file<<endl;
    }

    file<<"#  cross id"<<endl;
    for (int i = 0; i < cross_wait.size(); ++i) {
        file <<cross_wait[i]<<"   "<<endl;
    }
    file<<"#  road  "<<endl;
    for (int k = 0; k < roads.size(); ++k) {
        file << roads[k].idx<<"   ";
        int total = 0;
        for (int i = 0; i < roads[k].channel; ++i) {
            total += roads[k].Carline[i].size();
        }
        file <<roads[k].from<<"-->"<<roads[k].to<<"  :  "<< total;
        total = 0;
        for (int i = 0; i < roads[k].channel; ++i) {
            total += roads[k].Carline2[i].size();
        }
        file <<"   "<<roads[k].to<<"-->"<<roads[k].from<<"  :  "<< total<<endl;
    }


    file.close();
}