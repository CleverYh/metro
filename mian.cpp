#include <iostream>
#include "subway.hpp"
using namespace std;

int main(){
    Subway_info a;
    string sta;
    cin >> sta;
    a.show_line_station_locates(sta);
    return 0;
}    