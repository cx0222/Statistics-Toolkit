//
// Created by 陈玄 on 2022/4/9.
// Modified by 陈玄 on 2022/4/24.
//

#ifndef UNTITLED121_PPRINT_H
#define UNTITLED121_PPRINT_H

using namespace std;

template<class T>
inline void pprint(vector<T> *);

template<class T>
inline void pprint(vector<vector<T>> *);

template<class T>
inline void pprint(vector<T> *v) {
    for (const auto &i: *v) {
        cout << "\t" << i;
    }
    cout << "\n";
}

template<class T>
inline void pprint(vector<vector<T>> *u) {
    cout << "[Count] " << u->size() << "\n";
    for (auto i: *u) {
        pprint(&i);
    }
}

#endif // UNTITLED121_PPRINT_H
