#pragma once
#include "wx/wx.h"

class ModManagerPanel : public wxPanel
{
public:
    enum class ElementID
    {
        E_MODINPUT = wxID_HIGHEST + 2000,
        E_BTNADDMOD,
        E_BTNDEPLOYMOD
    };

public:
    ModManagerPanel(wxWindow* _parent,wxWindowID _winID) : wxPanel(_parent,_winID)
    {
        E_ModList = new wxCheckListBox(this,wxID_ANY);
        E_ModIdInput = new wxTextCtrl(this,static_cast<int>(ElementID::E_MODINPUT));
        E_BtnAddMod = new wxButton(this,static_cast<int>(ElementID::E_BTNADDMOD),"Add mod");
        E_BtnDeployMod = new wxButton(this,static_cast<int>(ElementID::E_BTNDEPLOYMOD),"Deploy mod");

        // Init sizers
        S_MainSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* S_row_bottom = new wxBoxSizer(wxHORIZONTAL);
        
        S_row_bottom->Add(E_ModIdInput,2,wxTOP | wxLEFT | wxBOTTOM,10);
        S_row_bottom->Add(E_BtnAddMod,1,wxTOP | wxLEFT | wxBOTTOM,10);
        S_row_bottom->Add(E_BtnDeployMod,1, wxTOP | wxLEFT | wxBOTTOM | wxRIGHT,10);


        // Add element to sizer

        S_MainSizer->Add(E_ModList,1,wxEXPAND);
        S_MainSizer->Add(S_row_bottom,0,wxEXPAND);
        this->SetSizer(S_MainSizer);
        
    }
    ~ModManagerPanel(){}
public: // Element
    wxCheckListBox* E_ModList;
    
    wxTextCtrl* E_ModIdInput;

    wxButton* E_BtnAddMod;
    wxButton* E_BtnDeployMod;


public: //Sizer
    wxBoxSizer* S_MainSizer;

};