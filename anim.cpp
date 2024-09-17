#include "anim.h"
#include "textRender.h"

//from hsUtil

/*
    Name:       clear_screen
    Arguments:  char fill
    Purpose:    Clear screen when called. Optionally, fill screen with input char
*/
void clear_screen(char fill = ' ') { //stolen from stackoverflow. Uses windows.h
    COORD tl = {0,0};
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);   
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}

/*
    Name:       wait
    Args:       int s
    Purpose:    Wait for the specified number of milliseconds
*/
void wait(int s){//Also a stackoverflow borrow. Uses chrono and thread
    std::this_thread::sleep_for(std::chrono::milliseconds(s));
}

Change::Change(double* attRef, double changeAmt, int frames, int delay = 0){
    attRef = attRef;
    changeAmt = changeAmt;
    frames = frames;
    delay = delay;
    frameProgress = 0;
}


int animate(std::vector<Change> & changeList){
    bool done;
    int frameCount = 0;
    std::vector<FlatSurface> flatSurfaces;

    do{
        flatSurfaces.clear();
        clear_screen(' ');
        
        done = animStep(changeList);
        frameCount++;

        drawFrame(&flatSurfaces); //fix
    }while(!done);

    return frameCount;
}

bool animStep(std::vector<Change> & changeList){
    bool exitCond = true;
    for(auto& c : changeList){
        if(c.delay > 0){
            c.delay--;
            exitCond = false;
        }else if(c.frameProgress != c.frames){
            *(c.attRef) += (c.changeAmt/c.frames);
            c.frameProgress++;
            exitCond = false;
        }
    }
    return exitCond;
}

void drawFrame(std::vector<FlatSurface>* flatSurfaces){ //add vector for "bodies" here?
    for(auto& s : testSphere.surfaces){
            *flatSurfaces.push_back(s.cameraTransform(c));
        }

        std::cout << Draw(*flatSurfaces, {-40, 40}, {-20, 20});

        wait(frameTime); //something to make this the same between large and small render jobs?
}