//
// Created by 陈玄 on 2022/4/23.
// Modified by 陈玄 on 2022/4/24.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "pprint.h"
#include "bayes.h"

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "[Welcome] Naive-Bayes version 2.1\n";
    cout << "[Author] *cx2020 [2022 Apr 24]\n\n";

    cout << "[Input] Please input file name...\n";
    string filename;
    cin >> filename;

    cout << "[Input] Please input the numbers of rows...\n";
    int dim;
    cin >> dim;
    vector<int> attribute(dim, 0);

    vector<vector<int>> import;
    ifstream file1(filename);
    if (file1.is_open()) {
        string line;
        string each;
        while (getline(file1, line)) {
            vector<int> tmp;
            istringstream transaction(line);
            for (int i = 0; i < dim; i++) {
                getline(transaction, each, ',');
                int tag = atoi(each.c_str());
                tmp.push_back(tag);
                if (tag + 1 > attribute[i]) { attribute[i] = tag + 1; }
            }
            import.push_back(tmp);
        }
    } else {
        cout << "[Exception] File cannot be open.\n";
        return 1;
    }

    cout << "[Note] Here are the imported data:\n";
    pprint(&import);

    cout << "[Note] Each rows of data have the following number of kinds:\n";
    pprint(&attribute);

    cout << "[Input] Please input the sample point, seperated with ' ', ended with '\\n'...\n";
    vector<int> which;
    for (int i = 0; i < dim - 1; i++) {
        int each;
        cin >> each;
        which.push_back(each);
    }
    cout << "[Note] Here is the sample point:\n";
    pprint(&which);

    vector<int> res = naive_bayes(&import, &attribute, &which);

    cout << "[Note] The following list is decreasingly sorted by the probability in which the sample point lie.\n";
    pprint(&res);

    return 0;
}
