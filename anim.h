#ifndef ANIM_H
#define ANIM_H

#include <windows.h>
#include <chrono>
#include <thread>
#include <vector>

void clear_screen(char);
void wait(int);

class Change{
    public:
        double* attRef = nullptr;
        double changeAmt = 0;
        int frames = 1;
        int delay = 0;
        int frameProgress = 0;

        Change(double*, double, int, int);
};

int animate(std::vector<Change> &);
bool animStep(std::vector<Change> &);

#endif