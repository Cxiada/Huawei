//
// Created by XDchen on 2019/3/10.
//

#include "input.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stack>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <cstdio>

void Car::Car_init(int idx, int from, int to, int speed_max,
                   int planTime) {
    this->idx=idx;
    this->from=from;
    this->to=to;
    this->speed_max=speed_max;
    this->planTime=planTime;
};


void Road::Road_init(int idx, int length, int speed, int channel,
                     int from, int to,bool isDuplex) {
    this->idx=idx;
    this->length=length;
    this->speed=speed;
    this->channel=channel;
    this->from=from;
    this->to=to;
    this->isDuplex=isDuplex;
};

void Cross::Cross_init(int idx, int up, int right, int down, int left) {
    this->idx=idx;
    this->up=up;
    this->right=right;
    this->down=down;
    this->left=left;
};

vector<Road> Road_input(string path){
    char buffer[256];
    vector<Road> roads;
    Road temp(501,2,1,1);
    ifstream examplefile(path);
    if (! examplefile.is_open())
    {
        cout << "Error opening file"; exit (1);
    }
    while (!examplefile.eof())
    {
        examplefile.getline(buffer,100);
        if (buffer[0] == '#') continue;
        int flag=0;
        string id={},length={},speed={},channel={},from={},to={},isDuplex={};
        for (char ch:buffer){
            if (ch==')') break;
            if (ch==',') flag+=1;
            if (isdigit(ch)==0) continue;
            switch (flag){
                default: break;
                case 0: id+=ch;
                    break;
                case 1: length+=ch;
                    break;
                case 2: speed+=ch;
                    break;
                case 3: channel+=ch;
                    break;
                case 4: from+=ch;
                    break;
                case 5: to+=ch;
                    break;
                case 6: isDuplex+=ch;
                    break;
            }
        }
        const char* p1 = id.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p2 = length.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p3 = speed.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p4 = channel.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p5 = from.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p6 = to.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p7 = isDuplex.data();//加const  或者用char * p=(char*)str.data();的形式
        temp.Road_init(atoi(p1),atoi(p2),atoi(p3),atoi(p4),atoi(p5),atoi(p6),atoi(p7));
        roads.push_back(temp);
    }
    return roads;
};

vector<Car> Car_input(string path){
    char buffer[256];
    vector<Car> cars;
    Car temp(1, 10, 6, 5, 1);
    ifstream examplefile(path);
    if (! examplefile.is_open())
    {
        cout << "Error opening file"; exit (1);
    }
    while (!examplefile.eof())
    {
        examplefile.getline(buffer,100);
        if (buffer[0] == '#') continue;
        int flag=0;
        string id={},from={},to={},speed_max={},planTime={};
        for (char ch:buffer){
            if (ch==')') break;
            if (ch==',') flag+=1;
            if (isdigit(ch)==0) continue;
            switch (flag){
                default: break;
                case 0: id+=ch;
                    break;
                case 1: from+=ch;
                    break;
                case 2: to+=ch;
                    break;
                case 3: speed_max+=ch;
                    break;
                case 4: planTime+=ch;
                    break;
            }
        }
        const char* p1 = id.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p2 = from.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p3 = to.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p4 = speed_max.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p5 = planTime.data();//加const  或者用char * p=(char*)str.data();的形式
        temp.Car_init(atoi(p1),atoi(p2),atoi(p3),atoi(p4),atoi(p5));
        cars.push_back(temp);
    }
    return cars;
};

vector<Cross> Cross_input(string path){
    char buffer[256];
    vector<Cross> crosses;
    Cross temp(1, 10, 6, 5, 1);
    ifstream examplefile(path);
    if (! examplefile.is_open())
    {
        cout << "Error opening file"; exit (1);
    }
    while (!examplefile.eof())
    {
        examplefile.getline(buffer,100);
        if (buffer[0] == '#') continue;
        int flag=0;
        string id={},up={},right={},down={},left={};
        for (char ch:buffer){
            if (ch==')') break;
            if (ch==',') flag+=1;
            if (isdigit(ch)==0 and ch !='-') continue;
            switch (flag){
                default: break;
                case 0: id+=ch;
                    break;
                case 1: up+=ch;
                    break;
                case 2: right+=ch;
                    break;
                case 3: down+=ch;
                    break;
                case 4: left+=ch;
                    break;
            }
        }
        const char* p1 = id.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p2 = up.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p3 = right.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p4 = down.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p5 = left.data();//加const  或者用char * p=(char*)str.data();的形式
        temp.Cross_init(atoi(p1),atoi(p2),atoi(p3),atoi(p4),atoi(p5));
        crosses.push_back(temp);
    }
    return crosses;
};

//----------------------------------------------------test_cpp---------------------
void Car_answer::Car_answer_init(int idx, int planTime, vector<int> road_id) {
    this->idx=idx;
    this->planTime=planTime;
    this->road_id=road_id;
};

vector<Car_answer> Car_answer_input(string path){
    char buffer[256];
    vector<Car_answer> Car_answers;
    vector<int> road_id;
    Car_answer temp(1, 1, road_id);
    ifstream examplefile(path);
    if (! examplefile.is_open())
    {
        cout << "Error opening file"; exit (1);
    }
    while (!examplefile.eof())
    {
        examplefile.getline(buffer,100);
        if (buffer[0] == '#') continue;
        int flag=0;
        string id={},planTime={},road={};
        road_id.clear();
        for (char ch:buffer){
            if (ch==')') break;
            if (ch==',') flag+=1;
            if (isdigit(ch)==0 and ch !='-') continue;
            switch (flag){
                default: break;
                case 0: id+=ch;
                    break;
                case 1: planTime+=ch;
                    break;
                case 2: road+=ch;
                    break;
                case 3: const char* p3 = road.data();
                    road_id.push_back(atoi(p3));
                    road={};
                    road+=ch;
                    flag=2;
                    break;
            }
        }
        const char* p1 = id.data();//加const  或者用char * p=(char*)str.data();的形式
        const char* p2 = planTime.data();//加const  或者用char * p=(char*)str.data();的形式
        temp.Car_answer_init(atoi(p1),atoi(p2),road_id);
        Car_answers.push_back(temp);
    }
    return Car_answers;
};