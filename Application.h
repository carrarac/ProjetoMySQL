#pragma once
#include <wx/wx.h>
#include "Frame.h"
#include "MySQLDatabase.h"
class Application :
    public wxApp
{
public:
    virtual bool OnInit();
};

