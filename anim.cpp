#include "anim.h"

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