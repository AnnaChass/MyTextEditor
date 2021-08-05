#include "MyEditorView.h"

MyEditorView::MyEditorView(MyEditorModel* model, MyEditorController* controller)
{
    p_model_ = model;
    p_model_->AddObserver(this);
    p_controller_ = controller;
    p_controller_->AddObserver(this);

    file_win = newwin(LINES - 8, COLS, 0, 0);
    wrefresh(file_win);

    mode_win = newwin(3, COLS / 3, LINES - 5, 0);
    box(mode_win, 0, 0);
    wrefresh(mode_win);

    filename_win = newwin(3, COLS / 3, LINES - 5, COLS / 3);
    box(filename_win, 0, 0);
    wrefresh(filename_win);

    string_win = newwin(3, COLS / 3, LINES - 5, 2 * COLS / 3);
    box(string_win, 0, 0);
    wrefresh(string_win);
}
MyEditorView::~MyEditorView()
{
    delwin(file_win);
}
