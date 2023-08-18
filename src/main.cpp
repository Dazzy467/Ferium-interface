#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wxCustomButton.h"
#include "json.hpp"
#include "fstream"
#include "curlModrinth.h"
#include <map>


class ProfilePanel : public wxPanel
{
public:
    enum class ElementID
    {
        E_PROFILELIST = wxID_HIGHEST + 3000,
        E_MODLIST

    };
public:
    ProfilePanel(wxWindow* _parent,wxWindowID _winID) : wxPanel(_parent,_winID)
    {
        
        E_ProfileList = new wxListCtrl(this,static_cast<int>(ElementID::E_PROFILELIST),wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
        E_ModList = new wxListCtrl(this,static_cast<int>(ElementID::E_MODLIST),wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

        E_ProfileList->Bind(wxEVT_LIST_ITEM_ACTIVATED,&ProfilePanel::OnItemActivated,this);
        E_ProfileList->Bind(wxEVT_LIST_ITEM_SELECTED,&ProfilePanel::OnItemSelected,this);

        S_MainSizer = new wxBoxSizer(wxVERTICAL);
        S_ListBoxSizer = new wxBoxSizer(wxHORIZONTAL);

        S_ListBoxSizer->Add(E_ProfileList,1,wxEXPAND);
        S_ListBoxSizer->Add(E_ModList,1,wxEXPAND | wxLEFT,10);

        S_MainSizer->Add(S_ListBoxSizer,1,wxEXPAND);



        // Code here

        // Open ferium config.json file
        FILE.open("../../Json/config.json",std::ios::in);
        // Json parser
        jsonData = nlohmann::json::parse(FILE);

        nlohmann::json profileArray = jsonData["profiles"];

        

        // Create columns for profile list and mod list
        E_ProfileList->InsertColumn(0, "Profile Name");
        E_ProfileList->InsertColumn(1, "Minecraft Version");
        E_ProfileList->InsertColumn(2, "Mod Loader");

        E_ModList->InsertColumn(0,"Mods");
        E_ModList->InsertColumn(1,"Version");

        // Read the profile name and append to the profile list
        for (const auto& profile : profileArray)
        {
            long itemIndex = E_ProfileList->InsertItem(E_ProfileList->GetItemCount(),
                                                       profile["name"].get<std::string>());
            E_ProfileList->SetItem(itemIndex, 1, profile["game_version"].get<std::string>());
            E_ProfileList->SetItem(itemIndex, 2, profile["mod_loader"].get<std::string>());
        }

        this->SetSizer(S_MainSizer);
    }
    ~ProfilePanel(){}
public: // Element
    wxListCtrl* E_ProfileList;
    wxListCtrl* E_ModList;
public: // Sizer
    wxBoxSizer* S_MainSizer;
    wxBoxSizer* S_ListBoxSizer;
public: // Member
    std::fstream FILE;
    nlohmann::json jsonData;
    std::map<std::string,std::string> modVersionMap;
public: // Function
    void FitListCtrlColumns(wxListCtrl* listCtrl)
    {
        for (int col = 0; col < listCtrl->GetColumnCount(); ++col)
        {
            listCtrl->SetColumnWidth(col, wxLIST_AUTOSIZE);
            int currentWidth = listCtrl->GetColumnWidth(col);
            listCtrl->SetColumnWidth(col, wxLIST_AUTOSIZE_USEHEADER);
            int headerWidth = listCtrl->GetColumnWidth(col);
            listCtrl->SetColumnWidth(col, wxMax(currentWidth, headerWidth));
        }
    }


public: // Event handler

    void OnItemActivated(wxListEvent& e)
    {
        switch (e.GetId())
        {
        case static_cast<int>(ElementID::E_PROFILELIST):{
            std::cout << "Activated index: " << e.GetIndex() << std::endl;
            std::cout << "Activated profile: " << e.GetText() << std::endl;
            
            std::string apiUrl = "https://api.modrinth.com/v2/project/";
            
            // std::string response = modrinth::HttpGet(apiUrl);
            // nlohmann::json responseJson = nlohmann::json::parse(response);
            // std::cout << "Response test (mod name): " << responseJson["title"] << std::endl;

            // Sets the active profile
            jsonData["active_profile"] = e.GetIndex();
            std::cout << "Active profile: " << jsonData["active_profile"] << std::endl;

            // Get the mod list of that profile and append it to the modlist
            nlohmann::json modList = jsonData["profiles"][e.GetIndex()]["mods"];
            if (E_ModList->GetItemCount() > 0)
                E_ModList->DeleteAllItems();

            for (const auto& mod : modList)
            {

                if (modVersionMap.find(mod["identifier"]["ModrinthProject"].get<std::string>()) != modVersionMap.end())
                {
                    long itemIndex = E_ModList->InsertItem(E_ModList->GetItemCount(),mod["name"].get<std::string>());
                    E_ModList->SetItem(itemIndex, 1, modVersionMap[mod["identifier"]["ModrinthProject"].get<std::string>()]);
                }
                else{
                    wxYield();
                    std::string modUrl = apiUrl + mod["identifier"]["ModrinthProject"].get<std::string>() + "/version";
                    std::string response = modrinth::HttpGet(modUrl);
                    nlohmann::json responseJson = nlohmann::json::parse(response);
                    modVersionMap[mod["identifier"]["ModrinthProject"].get<std::string>()] = responseJson[0]["version_number"].get<std::string>();

                    long itemIndex = E_ModList->InsertItem(E_ModList->GetItemCount(),mod["name"].get<std::string>());
                    E_ModList->SetItem(itemIndex, 1, responseJson[0]["version_number"].get<std::string>());
                }


                
                
            }
            
            break;
        }
        default:
            break;
        }
    }

    void OnItemSelected(wxListEvent& e)
    {
        switch (e.GetId())
        {
        case static_cast<int>(ElementID::E_PROFILELIST):
        {
            std::cout << "Selected index: " << e.GetIndex() << std::endl;
            std::cout << "Selected profile: " << e.GetText() << std::endl;
            break;
        }
            
        
        default:
            break;
        }
    }
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
        E_ModList = new wxCheckListBox(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
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
        BTN_DASHBOARD,
        BTN_VIEWMODMANAGER,
        BTN_PROFILE

    };
public:
    main(const wxString& title) : wxFrame(nullptr,wxID_ANY,title)
    {
        // JSON Parser

        // Temporary location
        std::ifstream jsonFile("../../Json/config.json");
        nlohmann::json jData = nlohmann::json::parse(jsonFile);
        std::cout << "Active profile: " << jData["profiles"][jData["active_profile"].get<unsigned int>()]["name"].get<std::string>() << std::endl;


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



        // Panels
        P_Header = new HeaderPanel(this,wxID_ANY);
        P_Header->SetBackgroundColour(wxColour(55,55,55));

        P_SidePanel = new wxPanel(this,wxID_ANY);
        P_SidePanel->SetBackgroundColour(wxColour(70,70,70));
        P_SidePanel->SetMinSize(wxSize(200,500));
        

        // Button
        // Font for side panel button        
        wxFont FeriumTextFont(wxFontInfo(12).Bold());
        // Side panel button
        E_SidePanelButton = new wxCustomButton* [SideBtnLabel.size()];

        for (size_t i = 0; i < SideBtnLabel.size(); i++)
        {
            E_SidePanelButton[i] = new wxCustomButton(P_SidePanel,static_cast<int>(SideBtnLabel[i].second),SideBtnLabel[i].first);
            E_SidePanelButton[i]->SetBackgroundColour(wxColour(55,55,55));
            E_SidePanelButton[i]->SetForegroundColour(wxColour(255,255,255));
            E_SidePanelButton[i]->SetPressedColour(wxColour(45,45,45));
            E_SidePanelButton[i]->SetFont(FeriumTextFont);
            E_SidePanelButton[i]->Bind(wxEVT_BUTTON,&main::OnSideBtnEvent,this);
        }


        P_ModManager = new ModManagerPanel(this,wxID_ANY);
        P_ModManager->Hide();
        P_ProfileManager = new ProfilePanel(this,wxID_ANY);
        P_ProfileManager->Hide();

        // Sizer
        wxBoxSizer* S_SidePanelSizer = new wxBoxSizer(wxVERTICAL);

        S_SidePanelRow = new wxBoxSizer*[SideBtnLabel.size()];


        // add side panel button to S_SidePanelRow sizer
        for(size_t i = 0; i < SideBtnLabel.size();i++)
        {
            S_SidePanelRow[i] = new wxBoxSizer(wxHORIZONTAL);
            S_SidePanelRow[i]->Add(E_SidePanelButton[i],1);
            S_SidePanelSizer->Add(S_SidePanelRow[i],0,wxEXPAND);
        }

        


        P_SidePanel->SetSizer(S_SidePanelSizer);

        S_MainSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* S_ContentRow1 = new wxBoxSizer(wxHORIZONTAL);

        S_ContentRow1->Add(P_SidePanel,0,wxEXPAND);
        S_ContentRow1->Add(P_ProfileManager,1,wxALL| wxEXPAND,5);
        S_ContentRow1->Add(P_ModManager,1,wxALL | wxEXPAND,5);

        S_MainSizer->Add(P_Header,0,wxEXPAND);
        S_MainSizer->Add(S_ContentRow1,1,wxEXPAND);

        this->SetSizer(S_MainSizer);
        this->Layout();
    }
    ~main()
    {
        delete[] E_SidePanelButton;
        delete[] S_SidePanelRow;
    }
public: // Panel
    HeaderPanel* P_Header;
    ModManagerPanel* P_ModManager;
    ProfilePanel* P_ProfileManager;
    wxPanel* P_SidePanel;
public: // Element

    wxMenuBar* E_MenuBar;
    wxMenu* Ec_FileMenu;
    wxMenu* Ec_PageMenu;

    // You can add Side panel button thru here. Just add Label and ElementID pairs 
    std::vector<std::pair<wxString,ElementID>> SideBtnLabel = {
        {"Dashboard",ElementID::BTN_DASHBOARD},
        {"Mod Manager",ElementID::BTN_VIEWMODMANAGER},
        {"Profile",ElementID::BTN_PROFILE}

    };

    wxCustomButton** E_SidePanelButton;


public: // Sizer

    wxBoxSizer* S_MainSizer;
    wxBoxSizer** S_SidePanelRow;

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
    void OnSideBtnEvent(wxCommandEvent& e)
    {
        std::cout << "Element ID: " << e.GetId() << std::endl;
        switch(e.GetId())
        {
            case static_cast<int>(ElementID::BTN_VIEWMODMANAGER):
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
                break;
            }

            case static_cast<int>(ElementID::BTN_PROFILE):
            {
                if(!P_ProfileManager->IsShown())
                {
                    P_ProfileManager->Show();
                    std::cout << "Profile Manager popped up!\n";
                }    
                else {
                    P_ProfileManager->Hide();
                    std::cout << "Profile Manager hidden!\n";
                }
                break;
            }
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