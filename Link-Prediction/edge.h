//
// Created by 陈玄 on 2022/11/13.
//

#ifndef NETWORK_NOV_13_EDGE_H
#define NETWORK_NOV_13_EDGE_H

struct edge {
    int start = 0;
    int end = 0;
    int start_neighbor = 0;
    int end_neighbor = 0;
    int intersection_size = 0;
    int union_size = 0;
    double salton = 0.0;
    double jaccard = 0.0;
    double sorenson = 0.0;
    double HPI = 0.0;
    double HDI = 0.0;
    double LHN = 0.0;
    int PA = 0;
    double AA = 0.0; // 实验性功能
    double RA = 0.0; // 实验性功能
};

#endif //NETWORK_NOV_13_EDGE_H
