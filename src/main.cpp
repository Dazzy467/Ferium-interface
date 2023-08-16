#include "wx/wx.h"
#include "wxCustomButton.h"

class ProfilePanel : public wxPanel
{
public:
    enum class ElementID
    {
        E_PROFILELIST = wxID_HIGHEST + 3000,

    };
public:
    ProfilePanel(wxWindow* _parent,wxWindowID _winID) : wxPanel(_parent,_winID)
    {

        E_ProfileList = new wxListBox(this,static_cast<int>(ElementID::E_PROFILELIST));
        

        S_MainSizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(S_MainSizer);
    }
    ~ProfilePanel(){}
public: // Element
    wxListBox* E_ProfileList;
public: // Sizer
    wxBoxSizer* S_MainSizer;

};
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
        E_BtnAddMod = new wxButton(this,static_cast<int>(ElementID::E_BTNADDMOD),"Add mod",wxDefaultPosition,wxDefaultSize,wxBORDER_NONE);
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

class HeaderPanel : public wxPanel
{
public: 
    HeaderPanel(wxWindow* _parent,wxWindowID _winID) : wxPanel(_parent,_winID)
    {
        wxString FeriumText = "FERIUM";

        wxFont FeriumTextFont(wxFontInfo(this->Font_Size).Bold());
        
        E_FeriumText = new wxStaticText(this,wxID_ANY,FeriumText);

        E_FeriumText->SetForegroundColour(wxColour(255,255,255));
        E_FeriumText->SetFont(FeriumTextFont);

        S_MainSizer = new wxBoxSizer(wxHORIZONTAL);
        S_MainSizer->Add(E_FeriumText,0, wxLEFT | wxTOP | wxBOTTOM,10);
        this->SetSizer(S_MainSizer);
        
    }
    ~HeaderPanel(){}
public: // element
    wxStaticText* E_FeriumText;
public: // Sizer

    wxBoxSizer* S_MainSizer;
public: // Variables
    double Font_Size = 14;
};

class main : public wxFrame
{
public:
    enum class ElementID
    {
        PAGE_MODMANAGER = wxID_HIGHEST + 1000,
        PAGE_PROFILE,
        BTN_VIEWMODMANAGER
    };
public:
    main(const wxString& title) : wxFrame(nullptr,wxID_ANY,title)
    {
        this->SetMinSize(wxSize(600,500));
        this->SetIcon(wxIcon("IDI_FILE"));
        
        // Menu
        E_MenuBar = new wxMenuBar();
        Ec_FileMenu = new wxMenu();
        Ec_PageMenu = new wxMenu();

        Ec_FileMenu->Append(wxID_OPEN,"&Open");
        Ec_FileMenu->Append(wxID_SAVE,"&Save");
        Ec_FileMenu->Append(wxID_SAVEAS,"&Save as");

        Ec_PageMenu->Append(static_cast<int>(ElementID::PAGE_MODMANAGER),"&Manage Mod");
        Ec_PageMenu->Append(static_cast<int>(ElementID::PAGE_PROFILE),"&Profiles");

        E_MenuBar->Append(Ec_FileMenu,"&File");
        E_MenuBar->Append(Ec_PageMenu,"&Page");
        E_MenuBar->Bind(wxEVT_COMMAND_MENU_SELECTED,&main::OnMenuSelected,this);
        // this->SetMenuBar(E_MenuBar);



        // Panels
        P_Header = new HeaderPanel(this,wxID_ANY);
        P_Header->SetBackgroundColour(wxColour(55,55,55));

        P_SidePanel = new wxPanel(this,wxID_ANY);
        P_SidePanel->SetBackgroundColour(wxColour(70,70,70));
        P_SidePanel->SetMinSize(wxSize(200,500));
        

        // Button
        E_BtnViewModManager = new wxCustomButton(P_SidePanel,static_cast<int>(ElementID::BTN_VIEWMODMANAGER),"Mod Manager");
        // E_BtnViewModManager->SetSize(wxSize(200,50));
        E_BtnViewModManager->SetBackgroundColour(wxColour(55,55,55));
        E_BtnViewModManager->SetForegroundColour(wxColour(255,255,255));
        E_BtnViewModManager->SetPressedColour(wxColour(45,45,45));
        E_BtnViewModManager->Bind(wxEVT_BUTTON,&main::OnBtnModManage,this);

        wxFont FeriumTextFont(wxFontInfo(12).Bold());
        E_BtnViewModManager->SetFont(FeriumTextFont);


        P_ModManager = new ModManagerPanel(this,wxID_ANY);
        P_ModManager->Hide();
        // P_ModManager->Show();

        // Sizer
        wxBoxSizer* S_SidePanelSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* S_SidePanelRow1 = new wxBoxSizer(wxHORIZONTAL);
        S_SidePanelRow1->Add(E_BtnViewModManager,1);
        S_SidePanelSizer->Add(S_SidePanelRow1,0,wxEXPAND);


        P_SidePanel->SetSizer(S_SidePanelSizer);

        S_MainSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* S_ContentRow1 = new wxBoxSizer(wxHORIZONTAL);

        S_ContentRow1->Add(P_SidePanel,0,wxEXPAND);
        S_ContentRow1->Add(P_ModManager,1,wxALL | wxEXPAND,5);

        S_MainSizer->Add(P_Header,0,wxEXPAND);
        S_MainSizer->Add(S_ContentRow1,1,wxEXPAND);

        this->SetSizer(S_MainSizer);
        this->Layout();
    }
public: // Panel
    HeaderPanel* P_Header;
    ModManagerPanel* P_ModManager;
    wxPanel* P_SidePanel;
public: // Element

    wxMenuBar* E_MenuBar;
    wxMenu* Ec_FileMenu;
    wxMenu* Ec_PageMenu;

    wxCustomButton* E_BtnViewModManager;

public: // Sizer

    wxBoxSizer* S_MainSizer;

public: // Event handler

    void OnMenuSelected(wxCommandEvent& e)
    {
        switch (e.GetId())
        {
        case static_cast<int>(ElementID::PAGE_MODMANAGER):
            std::cout << "Element ID: " << e.GetId() << std::endl;
            if(!P_ModManager->IsShown())
            {
                P_ModManager->Show();
                std::cout << "Mod manager popped up!\n";
            }    
            else {
                P_ModManager->Hide();
                std::cout << "Mod manager hidden!\n";
            }
            this->Layout();
            
            
            break;
        
        default:
            break;
        }
    }
    void OnBtnModManage(wxCommandEvent& e)
    {
        if(!P_ModManager->IsShown())
        {
            P_ModManager->Show();
            std::cout << "Mod manager popped up!\n";
        }    
        else {
            P_ModManager->Hide();
            std::cout << "Mod manager hidden!\n";
        }
        this->Layout();
        
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
        this->MainFrame = new main("Ferium GUI");
        this->MainFrame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(app);