
#pragma once
#include "MyEditorModel.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#define DOWN 2
#define UP 3
#define LEFT 4
#define RIGHT 5
#define SET 1
#define REMOVE 2

class MyEditorController : public Observable
{
private:
	MyEditorModel* p_model_;
    WINDOW* control_win;
    enum mode_
    {
        NAVIGATION,
        COMMAND,
        INPUT,
        SEARCH,
        EXIT
    } mode_ = NAVIGATION;
    enum input_mode_
    {
        BEFORE_CURSOR,
        AFTER_CURSOR,
        CHANGE_SYMBOL,
        NO_MODE
    } input_mode_ = NO_MODE;
    bool file_is_open_ = false;
    int x_ = 0, y_ = 0, global_pos_ = 0;
    char search_str_[64];

    void NavigationMode();
    void CommandMode();
    void InputMode();
    void SearchMode();
    void ReadCommand(char* str);
    void ReadCommandByOne(char* str);
    int FindStrStart(int pos);
    int FindStrEnd(int pos);
    int FindPrevFrom(char c, int pos);
    int FindNextFrom(char c, int pos);
    void GoToStrStart();
    void GoToStrEnd();
    void FindForwardAndSetCursor(char* search_str, int length);
    void FindBackwordAndSetCursor(char* search_str, int length);
    void CursorUpdate(int mode);
    
public:
    MyEditorController(MyEditorModel* model);
    ~MyEditorController();
    void Start();
    const char* GetMode();
    int GetActualString();
};

