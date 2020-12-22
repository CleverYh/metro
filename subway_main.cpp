#include "subway.hpp"
#include <iostream>

using namespace std;

void menu(void) {
    cout << "----------------------------------------------" << endl;
    cout << "            GuangZhou Metro Map               " << endl;
    cout << "----------------------------------------------" << endl;
    cout << "-           1. show Metro line               -" << endl;
    //按顺序显示某一条线路的所有站
    cout << "- 2. show the line(s) where a station locates-" << endl;
    //显示某一站在哪些线路上
    cout << "-         3. check travelling routes         -" << endl;
    //查询路线
    cout << "-                0.quit                      -" << endl;
    cout << "----------------------------------------------" << endl;
}

int main(void) {
    int n;
    string sta, start, des, line;
    Subway_info Info;
    menu();
    while (true) {
        cin >> n;
        switch (n) {
            case 1:
                cout << "Please enter the line name (in Chinese)" << endl;
                cin >> line;
                Info.show_entire_line(line);
                break;
            case 2: // 显示某一站在哪些线路上
                cout << "Please enter the station name(in Chinese)" << endl;
                cin >> sta;
                Info.show_line_station_locates(sta);
                break;
            case 3:
                cout << "Please enter your starting and destination station(seperated by space or enter)" << endl;
                cin >> start >> des;
                Info.Least_transfer_solution(start, des);
                Info.Least_stations_solution(start, des);
                break;
            case 0:
                cout << "Bye!" << endl;
                return 0;
        }
    }
}