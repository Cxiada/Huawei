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
#define normal 1
#define wait 2
#define finish 3
#define terminal 4
using namespace std;



int main() {
    string path1="road.txt";
    vector<Road> roads;
    roads=Road_input(path1);
//    for (auto iter=roads.cbegin();iter !=roads.cend();iter++)
//        cout<<(*iter).idx<<(*iter).length<<(*iter).speed<<(*iter).channel<<(*iter).from
//        <<(*iter).to<<(*iter).isDuplex<<endl;

    string path2="car.txt";
    vector<Car> cars;
    cars=Car_input(path2);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).from<<(*iter).to<<(*iter).speed_max<<(*iter).planTime<<endl;

    string path3="cross.txt";
    vector<Cross> crosses;
    crosses=Cross_input(path3);
//    for (auto iter=cars.cbegin();iter !=cars.cend();iter++)
//        cout<<(*iter).idx<<(*iter).up<<(*iter).right<<(*iter).down<<(*iter).left<<endl;

    string path4="answer.txt";
    vector<Car_answer> answers;
    answers=Car_answer_input(path4);
//    for (auto iter=answers.cbegin();iter !=answers.cend();iter++)
//        cout<<(*iter).idx<<(*iter).planTime<<(*iter).road_id.front()<<endl;
int time=1;
//    for(int i=0;i<answers.size();++i)
//    {
//             if(answers[i].planTime>time)
//             {
//                 cars[i].state==finish;
//                 continue;
//             }
//        int road_num=isroadempty();//返回车道号，前车ID（注意一车道满，二车道首车的情况）
//
//             if(road_num) //返回可进入的车道号；
//             {
//                 //车入车道
//                 cars[i].state==finish;
                   //更改车的R和V

//
//             } else{
//                 cars[i].state==finish;
//
//             }
//
//    }
    ++time;
    //void init_state();//所有的车初始状态置wait
    for(int i=0;i<answers.size();++i)
//    {

//             //能finish的finish 能wait的wait
               //存路口号
               //
//             //
//    }
        //路口号按ID升序
        while (所有车状态都改) {
            for (cross_id) {

                for(road_cross)
                {
                    //保存四条道路第一优先级车的行驶方向
                    int i=-1;
                    while(有没有finish 的车)
                    {
                        ++i;
                        i%=4;
                        //判断是否冲突，是跳出，否走车，改变状态若车finish 后面所有车都finish，判断死循环 


                    }


                }

            }
        }
}
