#include "wx/wx.h"


class ProfilePanel : public wxPanel
{
public:
    ProfilePanel(){}
    ~ProfilePanel(){}

};

class ModManagerPanel : public wxPanel
{
public:
    ModManagerPanel(wxWindow* _parent,wxWindowID _winID) : wxPanel(_parent,_winID)
    {
        E_ModList = new wxCheckListBox(this,wxID_ANY);
        E_ModIdInput = new wxTextCtrl(this,10);
        E_BtnAddMod = new wxButton(this,11,"Add mod");
        E_BtnDeployMod = new wxButton(this,12,"Deploy mod");

        // Init sizers
        S_MainSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* row_bottom = new wxBoxSizer(wxHORIZONTAL);
        
        row_bottom->Add(E_ModIdInput,2,wxTOP | wxLEFT | wxBOTTOM,10);
        row_bottom->Add(E_BtnAddMod,1,wxTOP | wxLEFT | wxBOTTOM,10);
        row_bottom->Add(E_BtnDeployMod,1, wxTOP | wxLEFT | wxBOTTOM | wxRIGHT,10);


        // Add element to sizer

        S_MainSizer->Add(E_ModList,1,wxEXPAND);
        S_MainSizer->Add(row_bottom,0,wxEXPAND);
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

class main : public wxFrame
{
public:
    main(const wxString& title) : wxFrame(nullptr,wxID_ANY,title)
    {
        // Menu
        E_MenuBar = new wxMenuBar();
        Ec_FileMenu = new wxMenu();
        Ec_PageMenu = new wxMenu();

        Ec_FileMenu->Append(wxID_OPEN,"&Open");
        Ec_FileMenu->Append(wxID_SAVE,"&Save");
        Ec_FileMenu->Append(wxID_SAVEAS,"&Save as");

        Ec_PageMenu->Append(50,"&Manage Mod");
        Ec_PageMenu->Append(51,"&Profiles");

        E_MenuBar->Append(Ec_FileMenu,"&File");
        E_MenuBar->Append(Ec_PageMenu,"&Page");
        E_MenuBar->Bind(wxEVT_COMMAND_MENU_SELECTED,&main::OnMenuSelected,this);
        this->SetMenuBar(E_MenuBar);

        // Panels
        P_ModManager = new ModManagerPanel(this,wxID_ANY);
        P_ModManager->Hide();
        // P_ModManager->Show();

        // Sizer
        S_MainSizer = new wxBoxSizer(wxHORIZONTAL);
        S_MainSizer->Add(P_ModManager,1,wxEXPAND);

        this->SetSizer(S_MainSizer);
        this->Layout();
    }
public: // Panel
    ModManagerPanel* P_ModManager;
public: // Element

    wxMenuBar* E_MenuBar;
    wxMenu* Ec_FileMenu;
    wxMenu* Ec_PageMenu;

public: // Sizer

    wxBoxSizer* S_MainSizer;

public: // Event handler

    void OnMenuSelected(wxCommandEvent& e)
    {
        switch (e.GetId())
        {
        case 50:
            P_ModManager->Show();
            this->Layout();
            break;
        
        default:
            break;
        }
    }


};

class app : public wxApp
{
public:
    app(){}
    ~app(){}
    main* MainFrame = nullptr;
public: // Method

    virtual bool OnInit()
    {
        this->MainFrame = new main("Ferium interface");
        this->MainFrame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(app);