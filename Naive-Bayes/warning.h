//
// Created by 陈玄 on 2022/4/24.
//

#ifndef UNTITLED127_WARNING_H
#define UNTITLED127_WARNING_H

#include <iostream>

using namespace std;

void show_warning(int);

void show_warning(int which) {
    cout << "[Warning " << which << "] ";
    if (which == 0) {
        cout << "Unknown.\n";
    } else if (which == 1) {
        cout << "Possibly data loss.\n";
    }
}

#endif //UNTITLED127_WARNING_H
