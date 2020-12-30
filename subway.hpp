#ifndef _SUBWAY_HPP_
#define _SUBWAY_HPP_

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

#define my_max 214748364

// string linename[16]= { "地铁一号线","地铁二号线", "地铁三号线", "地铁三号线北延段","地铁四号线" ,"地铁五号线", "地铁六号线", "地铁七号线", "地铁八号线",
//      "地铁九号线", "地铁十三号线", "地铁十四号线", "地铁二十一号线", "地铁广佛线","地铁APM线", "地铁十四号线支线" };

// 1. 每个站和线路用不用有它自己的类，看起来不用。
// 2.
class Subway_info {
    vector<vector<int>> stations;    // adjRect of stsations
    vector<vector<int>> lines;       // adjRect of lines
    vector<string> chinese_name_sta; // 这里用map映射会不会比较好
    vector<string> chinese_name_lines;
    vector<vector<int>> lines_of_sta;     // 站所在线路
    vector<vector<int>> stations_of_line; // 线路上的站

public:
    Subway_info() { //读文件

        ifstream in;

        in.open("guangzhou.txt");
        // in.open("C:\\Users\\cortex\\Documents\\code\\cpp\\metro\\guangzhou.txt");

        string s, station_name, line_name;
        stringstream stream;
        int station_index, lineNum, line, staNum, adjcent_station_index, adjcent_station_weight;

        // read meta data including number of lines and number of stations
        // also read the lines' chinese name in the first line of the file
        getline(in, s);
        stream << s;
        stream >> lineNum >> staNum;
        // cout << lineNum << staNum;
        cout << "Initializing " << staNum << " stations on " << lineNum << " lines..." << endl;
        for (int i = 0; i < lineNum; i++) {
            stream >> line_name;
            chinese_name_lines.push_back(line_name);
            // cout << chinese_name_lines[i] << " " << i << endl;
        }

        stations.resize(staNum);
        for (int i = 0; i < staNum; i++) stations[i].resize(staNum, my_max);

        lines_of_sta.resize(staNum);

        lines.resize(lineNum);
        for (int i = 0; i < lineNum; i++) lines[i].resize(lineNum);

        // loop to read data of every station
        while (getline(in, s)) {
            stream.str(""); // must clear the stream
            stream << s;
            // cout << s << endl;
            stream >> station_index >> station_name >> lineNum;
            // cout << station_index << " " << station_name << " " << lineNum << " ";
            chinese_name_sta.push_back(station_name);

            vector<int> adjLines;
            for (int i = 0; i < lineNum; i++) {
                stream >> line;
                adjLines.push_back(line);
                lines_of_sta[station_index].push_back(line);
                // cout << lines_of_sta[station_index][i] << " ";
            }
            for (int i = 0; i < lineNum; i++)
                for (int j = i + 1; j < lineNum; j++) lines[adjLines[i]][adjLines[j]] = lines[adjLines[j]][adjLines[i]] = 1;

            stream >> staNum;
            // cout << staNum << " ";
            for (int i = 0; i < staNum; i++) {
                stream >> adjcent_station_index >> adjcent_station_weight;
                stations[station_index][adjcent_station_index] = stations[adjcent_station_index][station_index] = adjcent_station_weight;
                // cout << adjcent_station_index << " " << stations[station_index][adjcent_station_index]<<endl;
            }
            // cout << endl;
        }

        in.close();

        // cout << lines_of_sta[44].size() << endl;

        for (int i = 0; i < lines.size(); i++)
            stations_of_line.push_back(entire_line(i));

        for(int i = 0;i<lines.size();i++){
            for(int j =0;j<lines[i].size();j++){
                cout << lines[i][j]<<" ";
            }
            cout << endl;
        }
    }

    //中文站名到索引
    int name_to_index(string sta_name) {
        for (int i = 0; i < chinese_name_sta.size(); i++)
            if (sta_name == chinese_name_sta[i])
                return i;
    }

    string UTF8ToGB(const char *str) { //乱码转中文
        string result;
        WCHAR *strSrc;
        LPSTR szRes;

        //获得临时变量的大小
        int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
        strSrc = new WCHAR[i + 1];
        MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

        //获得临时变量的大小
        i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
        szRes = new CHAR[i + 1];
        WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

        result = szRes;
        delete[] strSrc;
        delete[] szRes;

        return result;
    }

    //显示站所在线路
    void show_line_station_locates(const int sta) {
        for (int i = 0; i < lines_of_sta[sta].size(); i++) {
            //cout << UTF8ToGB(chinese_name_lines[lines_of_sta[sta][i]].c_str()).c_str() << " ";
            cout << chinese_name_lines[lines_of_sta[sta][i]] << " ";
        }
        cout << endl;
    }

    void show_line_station_locates(const string sta) {
        for (int i = 0; i < chinese_name_sta.size(); i++) {
            if (chinese_name_sta[i] == sta) {
                show_line_station_locates(i);
                break;
            }
        }
    }

    // 获取整条线路上的站
    vector<int> entire_line(const int line) {
        int l = line;
        //cout << "line is " << l << endl;
        vector<int> unordered_line;
        //找起始站，然后把一串儿都显示出来
        for (int i = 0; i < lines_of_sta.size(); i++) {
            for (int j = 0; j < lines_of_sta[i].size(); j++) {
                if (lines_of_sta[i][j] == l) {
                    unordered_line.push_back(i); // i is on Line l
                    // cout << i << endl;
                    break;
                }
            }
        }
        //cout << "unordered_line complete" << endl;
        //找起始点
        int cnt, start;
        for (int i = 0; i < unordered_line.size(); i++) {
            cnt = 0;
            for (int j = 0; j < unordered_line.size(); j++) {
                if (stations[unordered_line[i]][unordered_line[j]] < my_max)
                    cnt++;
            }
            if (cnt == 1) {
                start = unordered_line[i];
                // cout << start;
                break;
            }
        }

        //cout << chinese_name_sta[start] << endl;
        //找到起点站了，串糖葫芦
        vector<int> correct_line;
        map<int, int> used;
        for (int i = 0; i < unordered_line.size(); i++) {
            used.insert(pair<int, int>(unordered_line[i], 0));
        }
        correct_line.push_back(start);
        used[start] = 1;
        while (true) {
            int to_push_back = -1;
            for (int i = 0; i < unordered_line.size(); i++) {
                int flag = 0;
                if (!used[unordered_line[i]] && stations[unordered_line[i]][start] < my_max) {
                    to_push_back = unordered_line[i];
                    break;
                }
            }
            if (to_push_back == -1) {
                break;
            } else {
                correct_line.push_back(to_push_back);
                used[to_push_back] = 1;
                start = to_push_back;
            }
        }
        // for (int j = 0;j<correct_line.size();j++){
        //         cout << chinese_name_sta[correct_line[j]]<<" ";
        //     }
        //     cout << endl;

        return correct_line;
    }

    void show_entire_line(const string line) {
        int l;
        for (int i = 0; i < chinese_name_lines.size(); i++) {
            if (line == chinese_name_lines[i]) {
                l = i;
                break;
            }
        }
        // show_entire_line(entire_line(l),l);
        show_entire_line(l);
    }

    void show_entire_line(const int line) {
        // show_entire_line(entire_line(line),line);
        cout << chinese_name_lines[line] << ":" << endl;
        int i;
        for (i = 0; i < stations_of_line[line].size() - 1; i++) {
            cout << chinese_name_sta[stations_of_line[line][i]] << " <- " << stations[stations_of_line[line][i + 1]][stations_of_line[line][i]] << " min"
                 << " -> ";
        }
        cout << chinese_name_sta[stations_of_line[line][i]] << endl;
    }

    // void show_entire_line(const vector<int> correct_line, const int line) {
    //     cout << "The stations on " << line << " are:" << endl;
    //     for (int i = 0; i < correct_line.size() - 1; i++) {
    //         cout << chinese_name_sta[correct_line[i]] << "<-->";
    //     }
    //     cout << chinese_name_sta[correct_line[correct_line.size() - 1]] << endl;
    // }

    //最少换乘
    void show_LTS(vector<vector<int>> fault_solutions, int start, int end) {
        int flag;
        vector<int> toDelete(fault_solutions.size(), 0);
        vector<vector<int>> solutions;
        for (int i = 0; i < fault_solutions.size() - 1; i++) {
            for (int j = i + 1; j < fault_solutions.size(); j++) {
                flag = 1;
                for (int k = 0; k < fault_solutions[i].size(); k++) {
                    if (fault_solutions[i][k] != fault_solutions[j][k]) {
                        flag = 0;
                        break;
                    }
                }
                if (flag)
                    toDelete[j] = 1;
            }
        }
        for (int i = 0; i < fault_solutions.size(); i++) {
            if (!toDelete[i])
                solutions.push_back(fault_solutions[i]);
        }

        if (solutions.size() == 1) {
            cout << "Here is the only solution for you to take least transfers:" << endl;
        } else {
            cout << "Here are " << solutions.size() << " solutions for you to take least transfers:" << endl;
        }
        for (int i = 0; i < solutions.size(); i++) {
            cout << chinese_name_sta[start];
            if (solutions[i].size() == 1)
                cout << "-" << chinese_name_lines[solutions[i][0]] << "->";
            for (int j = 0; j < solutions[i].size() - 1; j++) {
                cout << "-" << chinese_name_lines[solutions[i][j]] << "->";
                int index, flag1, flag2;
                for (int ii = 0; ii < lines_of_sta.size(); ii++) {
                    flag1 = 0;
                    flag2 = 0;
                    for (int iii = 0; iii < lines_of_sta[ii].size(); iii++) {
                        if (lines_of_sta[ii][iii] == solutions[i][j])
                            flag1 = 1;
                        if (lines_of_sta[ii][iii] == solutions[i][j + 1])
                            flag2 = 1;
                    }
                    if (flag1 && flag2) {
                        cout << chinese_name_sta[ii];
                        break;
                    }
                }
            }
            if (!(solutions[i].size() - 1))
                cout << chinese_name_sta[end] << endl;
            else
                cout << "-" << chinese_name_lines[solutions[i][solutions[i].size() - 1]] << "->" << chinese_name_sta[end] << endl;
        }
    }

    void Least_transfer_solution(string curr_station, string destination) {
        int start = name_to_index(curr_station);
        int end = name_to_index(destination);

        //cout << start << " " << end << endl;
        int min = my_max;
        vector<vector<int>> solutions;
        vector<vector<int>> tmp_solution;
        for (int i = 0; i < lines_of_sta[start].size(); i++) {
            for (int j = 0; j < lines_of_sta[end].size(); j++) {
                //cout << "1" << endl;
                tmp_solution = BFS_LTS(lines_of_sta[start][i], lines_of_sta[end][j], lines[0].size());
                //cout << "2" << endl;
                for (int ii = 0; ii < tmp_solution.size(); ii++) {
                    if (tmp_solution[ii].size() < min) {
                        solutions.clear();
                        solutions.push_back(tmp_solution[ii]);
                        min = tmp_solution[ii].size();
                    }
                    if (tmp_solution[ii].size() == min) {
                        solutions.push_back(tmp_solution[ii]);
                    }
                }
            }
        }

        show_LTS(solutions, start, end);
        return;
    }

    vector<vector<int>> BFS_LTS(int start, int end, int line_num) {
        vector<vector<int>> ret;
        vector<pair<int, int>> record;
        vector<int> visited(line_num, 0);
        int sentinel = 1;
        int iter = 0;
        int flag = 0;

        pair<int, int> head(-1, start);
        record.push_back(head);
        while (iter <= sentinel) {
            if (iter == sentinel) {
                if (flag) break;
                sentinel = record.size();
            }
            pair<int, int> tmp = record[iter];
            if (tmp.second == end) // 到了终点
                flag = 1;
            visited[tmp.second] = 1;

            for (int i = 0; i < line_num; i++) {
                if (!visited[i] && lines[tmp.second][i]) {
                    pair<int, int> new_rec(tmp.second, i);
                    record.push_back(new_rec);
                    //if (!flag) sentinel++;
                }
            }

            iter++;
        }

        for (int i = sentinel; 0 <= i; i--) {
            if (record[i].second == end) {
                vector<int> ans;
                ans.push_back(end);
                int flag = record[i].first;
                while (flag != -1) {
                    for (int j = i - 1; 0 <= j; j--) {
                        if (record[j].second == flag) {
                            ans.push_back(flag);
                            flag = record[j].first;
                            break;
                        }
                    }
                }
                for (int j = 0; j < ans.size() / 2; j++) {
                    int tmpInt = ans[j];
                    ans[j] = ans[ans.size() - 1 - j];
                    ans[ans.size() - 1 - j] = tmpInt;
                }
                ret.push_back(ans);
            }
        }

        // for(int i = 0;i<ret.size();i++){
        //     for(int j =0;j<ret[i].size();j++){
        //         cout << ret[i][j]<<" ";
        //     }
        //     cout << endl;
        // }

        return ret;
    }

    //最少站点
    void Least_stations_solution(string curr_station, string destination) {
        int start = name_to_index(curr_station);
        int end = name_to_index(destination);

        vector<vector<int>> fault_solutions = BFS_LSS(start, end);

        show_LSS(fault_solutions);

        return;
    }

    vector<vector<int>> BFS_LSS(int start, int end) {
        vector<vector<int>> ret;
        vector<pair<int, int>> record;
        vector<int> visited(stations.size(), 0);
        int sentinel = 1;
        int iter = 0;
        int flag = 0;

        pair<int, int> head(-1, start);
        record.push_back(head);
        while (iter <= sentinel) {
            if (iter == sentinel) {
                if (flag)
                    break;
                sentinel = record.size();
            }
            pair<int, int> tmp = record[iter];
            if (tmp.second == end)
                flag = 1;
            visited[tmp.second] = 1;

            for (int i = 0; i < stations.size(); i++) {
                if (!visited[i] && stations[tmp.second][i]) {
                    pair<int, int> new_rec(tmp.second, i);
                    record.push_back(new_rec);
                    if (!flag)
                        sentinel++;
                }
            }

            iter++;
        }

        for (int i = record.size() - 1; 0 <= i; i--) {
            if (record[i].second == end) {
                vector<int> ans;
                ans.push_back(end);
                int flag = record[i].first;
                while (flag != -1) {
                    for (int j = i - 1; 0 <= j; j--) {
                        if (record[j].second == flag) {
                            ans.push_back(flag);
                            flag = record[j].first;
                            break;
                        }
                    }
                }
                for (int j = 0; j < ans.size() / 2; j++) {
                    int tmpInt = ans[j];
                    ans[j] = ans[ans.size() - 1 - j];
                    ans[ans.size() - 1 - j] = tmpInt;
                }
                ret.push_back(ans);
            }
        }

        return ret;
    }

    void show_LSS(vector<vector<int>> fault_solutions) {
        int flag;
        vector<int> toDelete(fault_solutions.size(), 0);
        vector<vector<int>> solutions;
        for (int i = 0; i < fault_solutions.size() - 1; i++) {
            for (int j = i + 1; j < fault_solutions.size(); j++) {
                flag = 1;
                for (int k = 0; k < fault_solutions[i].size(); k++) {
                    if (fault_solutions[i][k] != fault_solutions[j][k]) {
                        flag = 0;
                        break;
                    }
                }
                if (flag)
                    toDelete[j] = 1;
            }
        }
        for (int i = 0; i < fault_solutions.size(); i++) {
            if (!toDelete[i])
                solutions.push_back(fault_solutions[i]);
        }

        if (solutions.size() == 1) {
            cout << "Here is the only solution to the shortest route: " << endl;
        } else {
            cout << "Here are " << solutions.size() << " solutions to the shortest route: " << endl;
        }
        for (int i = 0; i < solutions.size(); i++) {
            for (int j = 0; j < solutions[i].size() - 1; j++) {
                cout << chinese_name_sta[solutions[i][j]] << "->";
            }
            cout << chinese_name_sta[solutions[i][solutions[i].size() - 1]] << endl;
        }

        return;
    }

    // vector<vector<int>> Floyd_LeastTimeSolutions(int start, int end) {
    //     vector<vector<int>> ret, D;
    //     D.resize(stations.size(), vector<int>(stations.size(), 0));
    //     vector<vector<vector<int>>> P;
    //     P.resize(stations.size(), D);
    //     int i, u, v, w;

    //     for (v = 0; v < stations.size(); v++) {
    //         for (w = 0; w < stations.size(); w++) {
    //             D[v][w] = stations[v][w];
    //             for (u = 0; u < stations.size(); u++) P[v][w][u] = 0;
    //             if (D[v][w] < my_max) P[v][w][u] = 1;
    //         }
    //     }

    //     for (u = 0; u < stations.size(); u++) {
    //         for (v = 0; v < stations.size(); v++) {
    //             for (w = 0; w < stations.size(); w++) {
    //                 if (D[v][u] + D[u][w] < D[v][w]) //从v经u到w的一条路径更短
    //                 {
    //                     D[v][w] = D[v][u] + D[u][w];
    //                     for (i = 0; i < stations.size(); i++) {
    //                         P[v][w][i] = P[v][u][i] || P[u][w][i]; //更新路径
    //                     }
    //                 }
    //             }
    //         }
    //     }

    //     cout << D[start][end] << endl;

    //     for (int i = 0; i < stations.size(); i++) cout<<P[start][end][i];
    //     // if (P[start][end][i] == 1) cout << i << " ";
    //     cout << "1" << endl;
    //     return ret;
    // }

    //最少时间
    void Least_time_solution(string curr_station, string destination) {
        int start = name_to_index(curr_station);
        int end = name_to_index(destination);

        // vector<vector<int>> lt_solutions = Floyd_LeastTimeSolutions(start, end);

        vector<vector<int>> lt_solutions = Dijkstra_LeastTimeSolutions(start, end);

        printPath(lt_solutions);

        return;
    }

    vector<vector<int>> Dijkstra_LeastTimeSolutions(int start, int end) {
        int n = stations.size();

        int totalTime = 0;

        vector<vector<int>> res(n, vector<int>(0, 0));

        vector<int> dist(n, my_max);
        vector<vector<int>> path(n, vector<int>(0, 0));
        vector<bool> known(n, false);

        for (size_t i = 0; i < n; i++) dist[i] = stations[start][i];

        dist[start] = 0;
        known[start] = true;

        for (size_t i = 0; i < n; i++) {
            int v = -1;
            int m = my_max;

            for (size_t i = 0; i < n; i++) {
                if (!known[i] && dist[i] < m) {
                    v = i;
                    m = dist[i];
                }
            }

            if (v == -1) break; // cannot find shorter unkown path

            known[v] = true;
            for (size_t i = 0; i < n; i++) {
                if (!known[i] && (stations[v][i] < my_max)) {
                    if (dist[v] + stations[v][i] < dist[i]) {
                        dist[i] = dist[v] + stations[v][i];
                        path[i].push_back(v);
                    }
                }
            }
        }

        stack<int> s;
        int p = end;

        /* -------

        void myDFS_iter(Graph g, vector<bool> & visited, Vertex s) {
            size_t k = g[s].size();
            visited[s] = true;

            stack<Vertex> sta;
            sta.push(s);

            while (!sta.empty()) {
                Vertex v = sta.top();

                size_t j = 0;

                for (; j < g[v].size(); j++) {
                    Vertex w = g[v][j];

                    if (!visited[w]) {
                        visited[w] = true;
                        sta.push(w);
                        break;
                    }
                }

                if (j == g[v].size()) sta.pop();
            }
        }

        int myDFS(Graph g, Vertex s) {
            size_t n = g.size();
            int count = 0;
            vector<bool> visited(n, false);

            for (size_t i = 0; i < n; i++) {
                if (!visited[i]) {
                    myDFS_iter(g, visited, i);
                    count++;
                }
            }

            return count;
        }

        ------- */

        while (!path[p].empty()) {
            s.push(path[p].back());
            totalTime += stations[p][path[p].back()];
            // cout << totalTime;
            p = path[p].back();
        }

        res[0].push_back(start);
        while (!s.empty()) {
            res[0].push_back(s.top());
            // cout << s.top() << " ";
            s.pop();
        }
        res[0].push_back(end);

        // for (int i = 0; i < res[0].size(); i++) cout << chinese_name_sta[res[0][i]] << " ";

        return res;
    }

    void printPath(vector<vector<int>> path) {
        int currline, nextStation, flag, time, totalTime = 0;

        cout << "Here is the best choice:" << endl;

        for (int i = 0; i < path.size(); i++) {
            currline = -1;
            for (int j = 0; j < path[i].size(); j++) {
                flag = 0;
                for (int k = 0; k < lines_of_sta[path[i][j]].size(); k++) {
                    // cout << currline << " " << lines_of_sta[path[i][j]][k] << endl;
                    if (lines_of_sta[path[i][j]][k] == currline) {
                        flag = 1;
                        break;
                    }
                }
                // cout << flag;
                if (!flag) { // 换乘
                    time = 0;
                    totalTime += 5;
                    if (currline != -1) {
                        if (j) j--;
                        cout << endl
                             << "Transfer:";
                    }
                    cout << endl
                         << "Take the ";
                    vector<int> nextlines;
                    for (int k = 0; k < lines_of_sta[path[i][j]].size(); k++) {
                        for (int l = 0; l < lines_of_sta[path[i][j + 1]].size(); l++) {
                            if (lines_of_sta[path[i][j]][k] == lines_of_sta[path[i][j + 1]][l])
                                nextlines.push_back(lines_of_sta[path[i][j]][k]);
                        }
                    }
                    for (int k = 0; k < nextlines.size(); k++) {
                        currline = nextlines[k];
                        cout << chinese_name_lines[nextlines[k]] << " train";
                        cout << " Toward ";
                        for (int l = 0; l < stations_of_line[nextlines[k]].size(); l++) {
                            if (stations_of_line[nextlines[k]][l] == path[i][j]) {
                                cout << chinese_name_sta[stations_of_line[nextlines[k]].back()];
                                break;
                            }
                            if (stations_of_line[nextlines[k]][l] == path[i][j + 1]) {
                                cout << chinese_name_sta[stations_of_line[nextlines[k]][0]];
                                break;
                            }
                        }
                        if (k < nextlines.size() - 1) cout << " or ";
                    }

                    cout << endl;
                } else {
                    if (j) totalTime += stations[path[i][j]][path[i][j - 1]];
                    // cout << totalTime << endl;
                }

                cout << left << setw(16) << chinese_name_sta[path[i][j]] << time << " min" << endl;
                if (j < path[i].size() - 1) time += stations[path[i][j]][path[i][j + 1]];
            }
        }
        cout << endl
             << "Totally takes " << totalTime << " min" << endl
             << endl;
        // cout << totalTime << endl;
    }
};

#endif