#include "frontend/mainframe.hxx"
#include "frontend/colours.hxx"

#include <wx/splitter.h>
#include <wx/clipbrd.h>
#include <wx/aui/auibook.h>

MainFrame::MainFrame(std::string_view title)
    : wxFrame(nullptr, wxID_ANY, title.data(), wxDefaultPosition, wxSize(800, 600))
{
    wxInitAllImageHandlers();
    SetTitle("SQLight - No file open");

    // Splitter between left and right
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
    splitter->SetMinimumPaneSize(200);

    // Left panel
    wxPanel* leftPanel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    wxDataViewTreeCtrl* treeCtrl = SetupTableTreeView(leftPanel);
    
    leftSizer->Add(treeCtrl, 1, wxEXPAND | wxALL, 8);
    leftPanel->SetSizer(leftSizer);

    // Right panel
    wxPanel* rightPanel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    // Add buttons above right panel
    //wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    //wxImage runIcon("C:\\Users\\ethan\\Desktop\\SQLight\\assets\\run.png", wxBITMAP_TYPE_PNG);
    //runIcon = runIcon.Scale(22, 22, wxIMAGE_QUALITY_HIGH);
    //wxBitmap runBitmap(runIcon);

    //wxBitmapButton* runButton = new wxBitmapButton(rightPanel, wxID_ANY, runBitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
    //runButton->SetBackgroundColour(rightPanel->GetBackgroundColour());

    // Add buttons to sizer
    //buttonSizer->Add(runButton, 0, wxALL);
    //rightSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);

    wxAuiNotebook* aui = new wxAuiNotebook(
        rightPanel, 
        wxID_ANY, 
        wxDefaultPosition, wxDefaultSize, 
        wxAUI_NB_DEFAULT_STYLE | wxNO_BORDER
    );

    wxStyledTextCtrl* stc = SetupTextEditor(aui);
    aui->AddPage(stc, "SQL1");
    aui->SetControlMargin(0);

    rightSizer->Add(aui, 1, wxEXPAND | wxALL, 4);
    rightPanel->SetSizer(rightSizer);

    splitter->SplitVertically(leftPanel, rightPanel);
    splitter->SetSashGravity(0);
    splitter->SetSashPosition(splitter->GetMinimumPaneSize());

    CreateMenuBar();
}

wxStyledTextCtrl* MainFrame::SetupTextEditor(wxWindow* parent) {
    wxStyledTextCtrl* stc = new wxStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    stc->SetMarginType(0, wxSTC_MARGIN_NUMBER);

    wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    stc->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    stc->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(0, 0, 0));

    auto updateMarginWidth = [stc]() {
        int maxLines = stc->GetLineCount();
        int digitCount = std::to_string(maxLines).length();
        int marginWidth = digitCount * 12;

        stc->SetMarginWidth(0, marginWidth);
        };

    updateMarginWidth();

    stc->Bind(wxEVT_STC_UPDATEUI, [updateMarginWidth, stc](wxStyledTextEvent& event) {
        updateMarginWidth();
        });

    stc->SetCaretLineVisible(true);
    stc->SetCaretStyle(wxSTC_CARETSTYLE_BLOCK);
    stc->SetCaretForeground(wxColour(150, 150, 150));
    stc->SetCaretLineBackground(wxColour(235, 235, 235));
    stc->SetIndentationGuides(wxSTC_IV_LOOKBOTH);
    stc->SetIndent(4);

    // Set lexer for stc
    stc->SetLexer(wxSTC_LEX_SQL);

    // Keywords for SQL to highlight
    const char* sqlKeywords =
        "select from where insert into update delete create drop alter table "
        "index view trigger begin commit rollback values primary key foreign "
        "not null default as and or like in is between join inner outer left right on";

    stc->SetKeyWords(0, sqlKeywords);

    // Syntax highlighting for SQL
    stc->StyleSetForeground(wxSTC_SQL_IDENTIFIER, AppColours::SQLSyntax::IDENTIFIER);
    stc->StyleSetForeground(wxSTC_SQL_NUMBER, AppColours::SQLSyntax::NUMBER);
    stc->StyleSetForeground(wxSTC_SQL_WORD, AppColours::SQLSyntax::WORD);
    stc->StyleSetForeground(wxSTC_SQL_COMMENT, AppColours::SQLSyntax::COMMENT);
    stc->StyleSetForeground(wxSTC_SQL_COMMENTLINE, AppColours::SQLSyntax::COMMENT);
    stc->StyleSetForeground(wxSTC_SQL_STRING, AppColours::SQLSyntax::STRING);

    // Setup keybinds
    
    wxAcceleratorEntry entries[6];
    entries[0].Set(wxACCEL_CTRL, ( int ) 'C', wxID_COPY);
    entries[1].Set(wxACCEL_CTRL, ( int ) 'V', wxID_PASTE);
    entries[2].Set(wxACCEL_CTRL, ( int ) 'X', wxID_CUT);
    entries[3].Set(wxACCEL_CTRL, ( int ) 'A', wxID_SELECTALL);
    entries[4].Set(wxACCEL_CTRL, ( int ) 'Z', wxID_UNDO);
    entries[5].Set(wxACCEL_CTRL, ( int ) 'Y', wxID_REDO);
    wxAcceleratorTable accel(6, entries);
    stc->SetAcceleratorTable(accel);

    stc->Bind(wxEVT_MENU, [stc](wxCommandEvent&) { stc->Copy(); }, wxID_COPY);
    stc->Bind(wxEVT_MENU, [stc](wxCommandEvent&) { stc->Paste(); }, wxID_PASTE);
    stc->Bind(wxEVT_MENU, [stc](wxCommandEvent&) { stc->Cut(); }, wxID_CUT);
    stc->Bind(wxEVT_MENU, [stc](wxCommandEvent&) { stc->SelectAll(); }, wxID_SELECTALL);
    stc->Bind(wxEVT_MENU, [stc](wxCommandEvent&) { stc->Undo(); }, wxID_UNDO);
    stc->Bind(wxEVT_MENU, [stc](wxCommandEvent&) { stc->Redo(); }, wxID_REDO);

    return stc;
}

wxDataViewTreeCtrl* MainFrame::SetupTableTreeView(wxPanel* parent) {
    wxDataViewTreeCtrl* treeCtrl = new wxDataViewTreeCtrl(
        parent, 
        wxID_ANY, 
        wxDefaultPosition, wxDefaultSize,
        wxDV_ROW_LINES | wxDV_VERT_RULES | wxDV_MULTIPLE | wxDV_HORIZ_RULES | wxBORDER_STATIC | wxDV_NO_HEADER
    );

    treeCtrl->SetRowHeight(18);

    // Disable editing of tree ctrl items
    treeCtrl->Bind(wxEVT_DATAVIEW_ITEM_START_EDITING, [](wxDataViewEvent& event) {
        event.Veto();
    });

    // Root node
    wxDataViewItem tables = treeCtrl->AppendContainer(wxDataViewItem(nullptr), "Tables (0)");
    treeCtrl->AppendItem(tables, "Table 1");
    treeCtrl->AppendItem(tables, "Table 2");
    treeCtrl->AppendItem(tables, "Table 3");
    treeCtrl->AppendItem(tables, "Table 4");
    
    wxDataViewItem views = treeCtrl->AppendContainer(wxDataViewItem(nullptr), "Views (0)");
    treeCtrl->AppendItem(views, "View 1");
    treeCtrl->AppendItem(views, "View 2");

    wxDataViewItem indexes = treeCtrl->AppendContainer(wxDataViewItem(nullptr), "Indexes (0)");
    treeCtrl->AppendItem(indexes, "Index 1");
    treeCtrl->AppendItem(indexes, "Index 2");
    treeCtrl->AppendItem(indexes, "Index 3");

    wxDataViewItem triggers = treeCtrl->AppendContainer(wxDataViewItem(nullptr), "Triggers (0)");
    treeCtrl->AppendItem(triggers, "Trigger 1");

    return treeCtrl;
}

void MainFrame::CreateMenuBar() {
    wxMenu* fileMenu = new wxMenu;
    wxMenu* editMenu = new wxMenu;
    wxMenu* selectionMenu = new wxMenu;
    wxMenu* viewMenu = new wxMenu;
    wxMenu* runMenu = new wxMenu;
    wxMenu* helpMenu = new wxMenu;

    // File menu
    fileMenu->Append(wxID_NEW, "&New Database\tCtrl+N", "Create a new database");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_OPEN, "Open &Database\tCtrl+K Ctrl+D", "Open an existing database");
    fileMenu->Append(wxID_OPEN, "Open &Database Read-only\tCtrl+K Ctrl+R", "Open an existing database in read-only mode");
    fileMenu->Append(wxID_CLOSE, "&Close Database\tCtrl+K Ctrl+L", "Close an open database");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_SAVE, "&Save\tCtrl+S", "Save the current file");
    fileMenu->Append(wxID_SAVEAS, "Save &As\tCtrl+Shift+S", "Save the current file as");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tCtrl+Q", "Exit the application");

    // Edit menu
    editMenu->Append(wxID_ADD, "&Create Table", "Create a new SQL table");
    editMenu->Append(wxID_EDIT, "&Modify Table", "Edit the selected SQL table");
    editMenu->Append(wxID_DELETE, "&Delete Table", "Delete the selected SQL table");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_UNDO, "&Undo\tCtrl+Z", "Undo the last action");
    editMenu->Append(wxID_REDO, "&Redo\tCtrl+Y", "Redo the last undone action");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_CUT, "Cu&t\tCtrl+X", "Cut the selected text");
    editMenu->Append(wxID_COPY, "&Copy\tCtrl+C", "Copy the selected text");
    editMenu->Append(wxID_PASTE, "&Paste\tCtrl+V", "Paste the copied text");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_FIND, "&Find\tCtrl+F", "Find text in the current file");
    editMenu->Append(wxID_REPLACE, "&Replace\tCtrl+H", "Replace text in the current file");
    editMenu->AppendSeparator();

    // Selection menu
    selectionMenu->Append(wxID_SELECTALL, "Select &All\tCtrl+A", "Select all text in the current file");
    selectionMenu->Append(wxID_SELECTALL, "Select &None\tCtrl+Shift+A", "Deselect all text in the current file");
    selectionMenu->Append(wxID_DUPLICATE, "Duplicate\tCtrl+D", "Duplicate the selected text");
    selectionMenu->Append(wxID_DELETE, "&Delete\tDel", "Delete the selected text");


    // Append and set menu bar
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(editMenu, "&Edit");
    menuBar->Append(selectionMenu, "&Selection");
    menuBar->Append(viewMenu, "&View");
    menuBar->Append(runMenu, "&Run");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

}
