#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wfstream.h>

#include "MainFrame.h"

#include "utils.h"
#include "globals.h"
#include "json.h"


class App : public wxApp
{
public:
    virtual bool OnInit();

private:
    MainFrame* frame;

    void PreFrame();
    void PostFrame();
};
