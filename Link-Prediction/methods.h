//
// Created by 陈玄 on 2022/11/13.
//

#ifndef NETWORK_NOV_13_METHODS_H
#define NETWORK_NOV_13_METHODS_H

#include <algorithm>
#include <cmath>
#include "edge.h"

class predict_methods {
public:
    static void calc_salton(struct edge &);

    static void calc_jaccard(struct edge &);

    static void calc_sorenson(struct edge &);

    static void calc_HPI(struct edge &);

    static void calc_HDI(struct edge &);

    static void calc_LHN(struct edge &);

    static void calc_PA(struct edge &);
};

void predict_methods::calc_salton(struct edge &edge) {
    edge.salton = edge.intersection_size / sqrt(edge.start_neighbor * edge.end_neighbor);
}

void predict_methods::calc_jaccard(struct edge &edge) {
    edge.jaccard = edge.intersection_size / double(edge.union_size);
}

void predict_methods::calc_sorenson(struct edge &edge) {
    edge.sorenson = 2.0 * edge.intersection_size / (edge.start_neighbor + edge.end_neighbor);
}

void predict_methods::calc_HPI(struct edge &edge) {
    edge.HPI = edge.intersection_size / double(std::min(edge.start_neighbor, edge.end_neighbor));
}

void predict_methods::calc_HDI(struct edge &edge) {
    edge.HDI = edge.intersection_size / double(std::max(edge.start_neighbor, edge.end_neighbor));
}

void predict_methods::calc_LHN(struct edge &edge) {
    edge.LHN = edge.intersection_size / double(edge.start_neighbor * edge.end_neighbor);
}

void predict_methods::calc_PA(struct edge &edge) {
    edge.PA = edge.start_neighbor * edge.end_neighbor;
}

std::string process_text(const std::string &method) {
    // 处理字符串 将字符串非法字符去除 并且转化为小写形式
    std::string res;
    for (char ch: method) {
        if (ch >= 'A' && ch <= 'Z') {
            res += char(ch + 32);
        } else if (ch >= 'a' && ch <= 'z') {
            res += ch;
        }
    }
    return res;
}


#endif //NETWORK_NOV_13_METHODS_H
