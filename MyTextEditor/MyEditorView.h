#pragma once
#include "Observer.h"
#include "MyEditorModel.h"
#include "MyEditorController.h"

class MyEditorView : public Observer
{
private:
    MyEditorModel* p_model_;
    MyEditorController* p_controller_;
    WINDOW *file_win, * mode_win, *filename_win, *string_win;

public:
    MyEditorView(MyEditorModel* model, MyEditorController* controller);
    ~MyEditorView();

    virtual void FileUpdate()
    {
        wclear(file_win);
        int pos = 0, fix_length = 100;
        while (p_model_->GetBufferSize() - pos > fix_length)
        {
            wprintw(file_win, p_model_->GetSubBuffer(pos, fix_length));
            wrefresh(file_win);
            pos += fix_length;
        }
        wprintw(file_win, p_model_->GetSubBuffer(pos, p_model_->GetBufferSize() - pos));
        wrefresh(file_win);
    }
    virtual void ModeUpdate()
    {
        wclear(mode_win);
        box(mode_win, 0, 0);
        mvwprintw(mode_win, 1, 1, p_controller_->GetMode());
        wrefresh(mode_win);
    }
    virtual void FilenameUpdate()
    {
        wclear(filename_win);
        box(filename_win, 0, 0);
        mvwprintw(filename_win, 1, 1, p_model_->GetFileName());
        wrefresh(filename_win);
    }
    virtual void StringUpdate()
    {
        wclear(string_win);
        box(string_win, 0, 0);
        mvwprintw(string_win, 1, 1, "%d / %d", p_controller_->GetActualString(), p_model_->GetStringNumber());
        wrefresh(string_win);
    }
};

