#include "App.h"
#include "MainFrame.h"

bool App::OnInit()
{
    MainFrame* mainFrame = new MainFrame("Game Score Manager");
    mainFrame->SetClientSize(1200, 600);
    mainFrame->Center();
    mainFrame->Show(true);
    return true;
}
