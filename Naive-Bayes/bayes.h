//
// Created by 陈玄 on 2022/4/23.
// Modified by 陈玄 on 2022/4/24.
//

#ifndef UNTITLED127_BAYES_H
#define UNTITLED127_BAYES_H

#include <cmath>
#include "pprint.h"
#include "warning.h"

using namespace std;

vector<int> bubble_sort(vector<double> *);

vector<int> item_count(vector<vector<int>> *, vector<int> *);

vector<vector<int>> tuple_count(vector<vector<int>> *, vector<int> *, vector<int> *);

vector<double> calc_prob(vector<vector<int>> *, vector<int> *, vector<vector<int>> *);

vector<int> naive_bayes(vector<vector<int>> *, vector<int> *, vector<int> *);

vector<int> bubble_sort(vector<double> *list) {
    int n = int(list->size());
    vector<int> ind(n, 0);
    for (int i = 0; i < n; i++) { ind[i] = i; }
    bool flag;
    for (int i = 0; i < n; i++) {
        flag = true;
        for (int j = 0; j < n - i; j++) {
            if ((*list)[j] < (*list)[j + 1]) {
                flag = false;
                double temp1 = (*list)[j];
                (*list)[j] = (*list)[j + 1];
                (*list)[j + 1] = temp1;
                int temp2 = ind[j];
                ind[j] = ind[j + 1];
                ind[j + 1] = temp2;
            }
        }
        if (flag) { break; }
    }
    return ind;
}

vector<int> item_count(vector<vector<int>> *data, vector<int> *attr) {
    int num = attr->back();
    // num: int (the number of classes)
    vector<int> res(num, 0);
    for (auto i: *data) { res[i.back()]++; }
    return res;
}

vector<vector<int>> tuple_count(vector<vector<int>> *data, vector<int> *attr, vector<int> *which) {
    int n_class = attr->back();
    // n_class: int (the number of classes)
    int n_attr = int(attr->size()) - 1;
    // n_attr: int (the number of attributes)
    vector<vector<int>> res(n_class, vector<int>(n_attr));
    for (auto i: *data) {
        int cls = i.back();
        // cls: int (which class)
        for (int j = 0; j < n_attr; j++) {
            res[cls][j] += ((*which)[j] == i[j]);
        }
    }
    return res;
}

vector<double> calc_prob(vector<vector<int>> *data, vector<int> *attr, vector<vector<int>> *tuple) {
    int n_class = attr->back();
    // n_class: int (the number of classes)
    int n_attr = int(attr->size()) - 1;
    // n_attr: int (the number of attributes)
    int n_transaction = int(data->size()) - 1;
    // n_transaction: int (the number of transactions)
    vector<int> prod;
    vector<double> prob;

    for (const auto &i: *tuple) {
        int product = 1;
        for (auto j: i) {
            if (j) { product *= j; }
            else { show_warning(1); }
        }
        prod.push_back(product);
    }

    vector<int> item = item_count(data, attr);
    for (int i = 0; i < n_class; i++) {
        prob.push_back(prod[i] / pow(double(item[i]), n_attr - 1) / n_transaction);
    }
    return prob;
}

vector<int> naive_bayes(vector<vector<int>> *data, vector<int> *attr, vector<int> *which) {
    vector<vector<int>> tuple = tuple_count(data, attr, which);
    vector<double> res = calc_prob(data, attr, &tuple);
    vector<int> ind = bubble_sort(&res);
    cout << "[Result] By sorting the conditional probability, we can get:\n";
    pprint(&res);
    return ind;
}

#endif // UNTITLED127_BAYES_H
