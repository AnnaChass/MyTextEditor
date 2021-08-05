#include "MyEditorController.h"

void MyEditorController::NavigationMode()
{
    CursorUpdate(SET);
    char str[5] = { 0 };
    ReadCommandByOne(str);
    if (!strcmp(str, ":"))
    {
        mode_ = COMMAND;
        NotifyModeUpdate();
    }
    else if (!strcmp(str, "o"))
    {
        mode_ = INPUT;
        input_mode_ = AFTER_CURSOR;
        NotifyModeUpdate();
    }
    else if (!strcmp(str, "i"))
    {
        mode_ = INPUT;
        input_mode_ = BEFORE_CURSOR;
        NotifyModeUpdate();
    }
    else if (!strcmp(str, "/"))
    {
        mode_ = SEARCH;
        NotifyModeUpdate();
    }
    else if (!strcmp(str, "?"))
    {
        mode_ = SEARCH;
        NotifyModeUpdate();
    }
    else if (!strcmp(str, "^") || !strcmp(str, "0"))
    {
        CursorUpdate(REMOVE);
        global_pos_ = FindStrStart(global_pos_);
        x_ = 0;
    }
    else if (!strcmp(str, "$"))
    {
        CursorUpdate(REMOVE);
        int old_global_pos_ = global_pos_;
        global_pos_ = FindStrEnd(global_pos_) - 1;
        x_ += global_pos_ - old_global_pos_;
    }
    else if (!strcmp(str, "gg"))
    {
        CursorUpdate(REMOVE);
        global_pos_ = 0;
        x_ = 0;
        y_ = 0;
        NotifyStringUpdate();
    }
    else if (!strcmp(str, "x"))
    {
        CursorUpdate(DELETE);
        p_model_->DeleteSymbol(global_pos_);
        NotifyFileUpdate();
    }

    else if ((int)str[0] == UP)
    {
        if (y_ == 0) return;
        else
        {
            CursorUpdate(REMOVE);
            y_--;
        }

        int prev_str_end = global_pos_ - x_ - ((p_model_->GetSymbolByPos(global_pos_ - x_ - 1) == '\n') ? 1 : 0),
            prev_str_start = (/*x_ == 0 || */p_model_->GetSymbolByPos(prev_str_end) != '\n') ? FindStrStart(prev_str_end - 1) : FindStrStart(prev_str_end);
        if (prev_str_end - prev_str_start == 0)
        {
            global_pos_ = prev_str_start;
            x_ = 0;
        }
        else if (x_ >= prev_str_end - prev_str_start)
        {
            global_pos_ = prev_str_end - 1;
            x_ = prev_str_end - prev_str_start - 1;
        }
        else global_pos_ = prev_str_start + x_;
        NotifyStringUpdate();
    }
    else if ((int)str[0] == DOWN)
    {
        int cur_str_end = FindStrEnd(global_pos_);
        if (cur_str_end == 0 || cur_str_end == p_model_->GetBufferSize()) return;

        if (y_ >= LINES - 8) return;
        else
        {
            CursorUpdate(REMOVE);
            y_++;
        }

        int next_str_start = cur_str_end + (p_model_->GetSymbolByPos(cur_str_end) == '\n' ? 1 : 0),
            next_str_end = (p_model_->GetSymbolByPos(next_str_start) == '\n') ? next_str_start : FindStrEnd(next_str_start + 1);
        
        if (next_str_end - next_str_start == 0)
        {
            global_pos_ = next_str_start;
            x_ = 0;
        }
        else if (x_ >= next_str_end - next_str_start)
        {
            global_pos_ = next_str_end - 1;
            x_ = next_str_end - next_str_start - 1;
        }
        else global_pos_ = next_str_start + x_;
        NotifyStringUpdate();
    }
    else if ((int)str[0] == RIGHT)
    {
        if (x_ == COLS - 1) return;
        char next = p_model_->GetSymbolByPos(global_pos_ + 1);
        if (next == -1 || next == '\n') return;

        CursorUpdate(REMOVE);
        x_++;
        global_pos_++;
    }
    else if ((int)str[0] == LEFT)
    {
        if (x_ == 0) return;

        CursorUpdate(REMOVE);
        x_--;
        global_pos_--;
    }
}
void MyEditorController::CommandMode()
{
    char str[30] = { 0 };
    ReadCommand(str);
    if (!strncmp(str, "o ", 2)) // "o filename" : open file
    {
        file_is_open_ = p_model_->OpenNewFile(str + 2);
    }
    else if (!strncmp(str, "x", 1) || !strncmp(str, "wq!", 3)) // write down and close file and quit 
    {
        if (!file_is_open_)
        {
            printw("\nError: no file is open.\n"); refresh();
            return;
        }
        p_model_->WriteDownCurrentFile();
        p_model_->CloseCurrentFile();
        file_is_open_ = false;
        mode_ = EXIT;
    }
    else if (!strncmp(str, "w ", 2) && sizeof(str) > 2) // write down not current file
    {
        p_model_->WriteDownOtherFile(str + 1);
    }
    else if (!strncmp(str, "w", 1)) // write down current file
    {
        if (!file_is_open_)
        {
            printw("\nError: no file is open.\n"); refresh();
            return;
        }
        p_model_->WriteDownCurrentFile();
    }
    else if (!strncmp(str, "q!", 2)) // close file and quit
    {
        if (file_is_open_)
        {
            p_model_->CloseCurrentFile();
        }
        file_is_open_ = false;
        mode_ = EXIT;
    }
    else if (!strncmp(str, "q", 1))
    {
        if (p_model_->FileIsChanged())
        {
            return;
        }
        if (file_is_open_)
        {
            p_model_->WriteDownCurrentFile();
            p_model_->CloseCurrentFile();
        }
        file_is_open_ = false;
        mode_ = EXIT;
    }
    else if ((int)str[0] == 27)
    {
        mode_ = NAVIGATION;
        NotifyModeUpdate();
    }

}
void MyEditorController::InputMode()
{
    CursorUpdate(SET);

    char str[100] = { 0 };
    ReadCommand(str);
    if (input_mode_ == NO_MODE)
    {
        if (!strcmp(str, "i")) // print before cursor
        {
            input_mode_ = BEFORE_CURSOR;
        }
        else if (!strcmp(str, "l"))
        {
            CursorUpdate(REMOVE);
            GoToStrStart();
            input_mode_ = AFTER_CURSOR;
        }
        else if (!strcmp(str, "A"))
        {
            CursorUpdate(REMOVE);
            GoToStrEnd();
            input_mode_ = AFTER_CURSOR;
        }
        else if (!strcmp(str, "r"))
        {
            input_mode_ = CHANGE_SYMBOL;
        }
        else if ((int)str[0] == 27)
        {
            mode_ = NAVIGATION;
            NotifyModeUpdate();
        }
    }
    else
    {
        CursorUpdate(REMOVE);
        if (input_mode_ == CHANGE_SYMBOL)
        {
            p_model_->SetSymbol(str[0], global_pos_);
        }
        else if (input_mode_ == BEFORE_CURSOR)
        {
            p_model_->BufferInsert(str, global_pos_);
            x_ += strlen(str);
            global_pos_ += strlen(str);
        }
        else
        {
            p_model_->BufferInsert(str, global_pos_ + 1);
        }
        input_mode_ = NO_MODE;
    }
}
void MyEditorController::SearchMode()
{

    CursorUpdate(SET);

    char str[100] = { 0 };
    ReadCommand(str);
    if (!strncmp(str, "/", 1))
    {
        if (global_pos_ == p_model_->GetBufferSize() - 1) return;
        memset(search_str_, 0, 64);

        int i = 0;
        for (; strncmp(str + i + 1, "<CR>", 4); ++i)
            search_str_[i] = str[i + 1];
        if (i == 0) return;
        search_str_[i + 1] = '\0';
        FindForwardAndSetCursor(search_str_, strlen(search_str_));
        NotifyStringUpdate();
    }
    else if (!strncmp(str, "?", 1))
    {
        if (global_pos_ == 0) return;
        memset(search_str_, 0, 64);

        int i = 0;
        for (; strlen(str) > i + 1, strncmp(str + i + 1, "<CR>", 4); ++i)
            search_str_[i] = str[i + 1];
        if (i == 0) return;
        search_str_[i + 1] = '\0';
        FindBackwordAndSetCursor(search_str_, strlen(search_str_));
        NotifyStringUpdate();
    }
    else if ((int)str[0] == 27)
    {
        memset(search_str_, 0, 64);
        mode_ = NAVIGATION;
        NotifyModeUpdate();
    }


}
void MyEditorController::ReadCommand(char* str)
{
    mvwgetstr(control_win, 1, 1, str);
    wclear(control_win);
    box(control_win, 0, 0);
    wrefresh(control_win);
}
void MyEditorController::ReadCommandByOne(char* str)
{
    wmove(control_win, 1, 1);
    wrefresh(control_win);
    char c;
    int i = 0;
    while (c = wgetch(control_win))
    {
        if (c == DOWN || c == UP || c == RIGHT || c == LEFT)
        {
            str[0] = c;
            return;
        }
        else if (c == '\r') // ENTER
            break;
        else
        {
            str[i] = c;
            mvwprintw(control_win, 1, 1, str); wrefresh(control_win);
            i++;
        }
    }
    wclear(control_win);
    box(control_win, 0, 0);
    wrefresh(control_win);
}
int MyEditorController::FindStrStart(int pos)
{
    int real_str_start = FindPrevFrom('\n', pos) + 1;
    if (pos - real_str_start >= COLS) return real_str_start + COLS * ((pos - real_str_start) / COLS);
    else return real_str_start;
}
int MyEditorController::FindStrEnd(int pos)
{
    int real_str_end = FindNextFrom('\n', pos);
    if (real_str_end - FindStrStart(pos) > COLS || pos - FindStrStart(pos) == COLS - 1) return COLS + FindStrStart(pos);
    else return real_str_end;
}
int MyEditorController::FindPrevFrom(char c, int pos) 
{
    int cur_pos = 0, prev_pos = -1;
    if (cur_pos < pos) cur_pos = p_model_->FindNextFrom(c, cur_pos);
    while (cur_pos < pos && cur_pos != -1)
    {
        prev_pos = cur_pos;
        cur_pos = p_model_->FindNextFrom(c, cur_pos + 1);
    }
    return prev_pos;
}
int MyEditorController::FindNextFrom(char c, int pos)
{
    int next = p_model_->FindNextFrom(c, pos);
    if (next == -1) return p_model_->GetBufferSize();
    else return next;
}
void MyEditorController::GoToStrStart()
{
    global_pos_ -= x_;
    x_ = 0;
}
void MyEditorController::GoToStrEnd()
{
    int old_global_pos_ = global_pos_;
    global_pos_ = FindStrEnd(global_pos_);
    x_ += global_pos_ - old_global_pos_;
}
void MyEditorController::FindForwardAndSetCursor(char* search_str, int length)
{
    int i = global_pos_, new_x = x_, new_y = y_;
    while (1)
    {
        if (i + length > p_model_->GetBufferSize())
            return;
        if (!strcmp(search_str, p_model_->GetSubBuffer(i, length))) break;
        if (p_model_->GetSubBuffer(i, length)[0] == '\n' || new_x == COLS - 1)
        {
            new_y++;
            new_x = -1;
        }
        i++;
        new_x++;
    }
    CursorUpdate(REMOVE);
    x_ = new_x;
    y_ = new_y;
    global_pos_ = i;
}
void MyEditorController::FindBackwordAndSetCursor(char* search_str, int length)
{
    int i = global_pos_, new_x = x_, new_y = y_;
    while (1)
    {
        if (i < 0) return;
        if (!strcmp(search_str, p_model_->GetSubBuffer(i, length))) break;
        if (new_x == 0)
        {
            new_y--;
            if (p_model_->GetSymbolByPos(i - 1) != '\n') new_x = COLS;
            else new_x = FindStrEnd(i - 1) - FindStrStart(i - 1) + 1;
        }
        i--;
        new_x--;
    }
    CursorUpdate(REMOVE);
    x_ = new_x;
    y_ = new_y;
    global_pos_ = i;
}
void MyEditorController::CursorUpdate(int mode)
{
    char c = p_model_->GetSymbolByPos(global_pos_);
    if (c == -1) return;
    if (c == '\n') c = ' ';
    if (mode == SET) mvaddch(y_, x_, c | A_REVERSE);
    else mvaddch(y_, x_, c | A_NORMAL);
    refresh();
}

MyEditorController::MyEditorController(MyEditorModel* model)
{
    p_model_ = model;
    mode_ = NAVIGATION;

    control_win = newwin(3, COLS, LINES - 3, 0);
    box(control_win, 0, 0);
    wrefresh(control_win);
}
MyEditorController::~MyEditorController()
{
    delwin(control_win);
}
void MyEditorController::Start()
{
    NotifyModeUpdate();

    raw();
    keypad(control_win, true);
    int exit_cond = 0;
    while (!exit_cond)
    {
        switch (mode_)
        {
        case NAVIGATION:
            NavigationMode();
            break;
        case COMMAND:
            CommandMode();
            break;
        case INPUT:
            InputMode();
            break;
        case SEARCH:
            SearchMode();
            break;
        case EXIT:
            exit_cond = 1;
            break;
        }

    }
    cbreak();
}
const char* MyEditorController::GetMode()
{
    if (mode_ == NAVIGATION) return "NAVIGATION MODE";
    else if (mode_ == INPUT) return "INPUT MODE";
    else if (mode_ == COMMAND) return "COMMAND MODE";
    else return "SEARCH MODE";
}
int MyEditorController::GetActualString()
{
    return y_ + 1;
}