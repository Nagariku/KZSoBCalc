#include "App.h"

std::vector<wxBitmap> MedalBitmaps;
wxFont Font;


wxIMPLEMENT_APP(App);


bool App::OnInit()
{
    this->PreFrame();
    this->frame = new MainFrame();
    this->PostFrame();
    return true;
}


void App::PreFrame() 
{
    wxPNGHandler* handler = new wxPNGHandler;
    wxImage::AddHandler(handler);

    MedalBitmaps = utils::GetMedalBitmaps();
    Font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    if (fs::exists(".\\settings.json"))
        ReadSettingsFile();
    else
        CreateSettingsFile();
}


void App::PostFrame()
{
    if (!this->frame->ternimated) 
    {
        this->frame->Show(true);
    }
}

// nicola is cool
// <3
