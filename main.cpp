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
using namespace std;



int main() {
//    string path1="road.txt";
//    vector<Road> roads;
//    roads=Road_input(path1);
//    for (auto iter=roads.cbegin();iter !=roads.cend();iter++)
//        cout<<(*iter).idx<<(*iter).length<<(*iter).speed<<(*iter).channel<<(*iter).from
//        <<(*iter).to<<(*iter).isDuplex<<endl;

//    string path2="car.txt";
//    vector<Car> cars;
//    cars=Car_input(path2);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).from<<(*iter).to<<(*iter).speed_max<<(*iter).planTime<<endl;

//    string path3="cross.txt";
//    vector<Cross> crosses;
//    crosses=Cross_input(path3);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).up<<(*iter).right<<(*iter).down<<(*iter).left<<endl;

    string path4="answer.txt";
    vector<Car_answer> answers;
    answers=Car_answer_input(path4);
    for (auto iter=answers.cbegin();iter !=answers.cend();iter++)
        cout<<(*iter).idx<<(*iter).planTime<<(*iter).road_id.front()<<endl;

}
