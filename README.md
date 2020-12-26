# Coding Proj. 5

This is a co-op homework from DCS211-Data Structure and Algorithm by Qiao Haiyan.

Build together with Liu Haoqin.


Version 1.1

fastest route

Version 1.0

1. show Metro line
2. show the line(s) where a station locates
3. check travelling routes
   1. least transfer
   2. shortest route

### CO-OP CODING PROJECT REPORT:

# Guangzhou Metro Guide Map

_chenyh369@mail2.sysu.edu.cn_

2020 年 12 月 21 日

## 摘要

本实验主要参照[广州地铁路线查询](http://www.gzmtr.com/)设计一个地铁路线查询程序，输出最佳乘车信息方案等信息。

## 引言

### 问题

设计并实现一个地铁线路查询程序，最低要求给出一种最佳乘车方案。

### 解决问题的思想

输入地铁线路和站点信息，建图，然后运用图论算法来求解。

## 数据结构和算法设计细节

### 数据结构

1. 首先是一个类，私有成员包括：
   1. 两个 `vector<vector<int> >` 邻接矩阵，一个用来存放站点，一个用来存放线路；
   2. 所有站点和线路使用数字标记，因此会有两个  `vector<string>`  存中文名称；
   3. 还有两个个 `vector<vector<int> >` 邻接表，用来存每个站所在的线路和每条线路上的车站；
2. 然后是定义输入的文件格式：
   1. 第一行记录线路数量m和站点数量n，然后是m个线路的中文名称，按编号排序；
   2. 接下来是n行，每行有车站编号，车站中文名称，车站所在线路，相邻车站编号，相邻车站距离（时间）；

### 算法

类的函数：

1. 构造函数读取地铁线路站点文件并初始化私有成员；
2. 显示站点信息的函数，输入一个站点名称显示站点所在线路等信息；
3. 显示线路信息的函数，输入一个线路名称显示线路的信息；
4. 输入起始站和终点站，显示最少换乘的方法
   1. 其实最少换乘就是经过最少的线路；
   2. 通过对起始站所在线路和终点站所在线路作为广度优先搜索的起点和终点进行地铁线路图的广度优先搜索，得出所有可能路径，存入邻接表；
   3. 如果得到的某个路径size小于min（换乘次数少于min），则清空结果的邻接表，将新的路径压入结果；
   4. 如果某个路径size等于min，则将它压入结果；
   5. 最后得到的结果是一条或多条最少换乘的方案；
5. 输入起始站和终点站，显示最快路线
   1. 使用dijkstra算法算出最短时间路线；
   2. 逐条打印乘车路线；

## 编译运行

```powershell
g++ main.cpp -o main
.\main
```

## 总结

本次实验实现了地铁路线规划，求出了最短时间路线。加深了对图的理解。也加深了对广州地铁的理解。

花在建立广州市地铁路线图上的时间和 debug 的时间很多，就没有做 GUI 。

应该考虑直接爬取数据。

总之基本实现了地铁路径查询的功能，接下来想学习 QT 做一个 GUI 。

txt 文件格式：
第一行行首两个数：线路数量m，站点数量n，之后是m个线路中文名
接下来n行： 序号 站名 所在线的数量 线路 邻接站数 站序号 相邻车站距离（时间）

