#include <curses.h>
#include "MyEditorModel.h"
#include "MyEditorView.h"
#include "MyEditorController.h"

int main()
{
    initscr();
    refresh();
    MyEditorModel model;
    MyEditorController controller(&model);
    MyEditorView view(&model, &controller);
    controller.Start();
    endwin();
    return 0;
}

