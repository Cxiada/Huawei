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
//    string path="road.txt";
//    vector<Road> roads;
//    roads=Road_input(path);
//    for (auto iter=roads.cbegin();iter !=roads.cend();iter++)
//        cout<<(*iter).idx<<(*iter).length<<(*iter).speed<<(*iter).channel<<(*iter).from
//        <<(*iter).to<<(*iter).isDuplex<<endl;

//    string path="car.txt";
//    vector<Car> cars;
//    cars=Car_input(path);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).from<<(*iter).to<<(*iter).speed_max<<(*iter).planTime<<endl;

    string path="cross.txt";
    vector<Cross> cars;
    cars=Cross_input(path);
    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
        cout<<(*iter).idx<<(*iter).up<<(*iter).right<<(*iter).down<<(*iter).left<<endl;
}