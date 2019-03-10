#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stack>
#include <queue>
#include <fstream>
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
                  int planTime) {
        this->idx=idx;
        this->from=from;
        this->to=to;
        this->speed_max=speed_max;
        this->planTime=planTime;
    };
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
                   int from, int to,bool isDuplex) {
        this->idx=idx;
        this->length=length;
        this->speed=speed;
        this->channel=channel;
        this->from=from;
        this->to=to;
        this->isDuplex=isDuplex;
    };
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
};

int main() {
    char buffer[256];
    ifstream examplefile("road.txt");
    if (! examplefile.is_open())
    {
        cout << "Error opening file"; exit (1);
    }
    while (!examplefile.eof())
    {
        examplefile.getline(buffer,100);
        cout<<buffer<< endl;
    }
    cout<<00<<endl;
    return 0;


}