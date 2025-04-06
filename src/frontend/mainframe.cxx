#include "frontend/mainframe.hxx"

#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/stc/stc.h>

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

    wxTreeCtrl* treeCtrl = new wxTreeCtrl(leftPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTR_TWIST_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_DEFAULT_STYLE);

    // Add roots for SQL tree
    wxTreeItemId root = treeCtrl->AddRoot("Tables");
    treeCtrl->AppendItem(root, "Dummy 1");
    treeCtrl->AppendItem(root, "Dummy 2");

    leftSizer->Add(treeCtrl, 1, wxEXPAND | wxALL, 5);
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

    wxStyledTextCtrl* stc = new wxStyledTextCtrl(rightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    stc->SetMarginType(0, wxSTC_MARGIN_NUMBER); 

    wxFont lineNumberFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    stc->StyleSetFont(wxSTC_STYLE_LINENUMBER, lineNumberFont);

    auto updateMarginWidth = [stc]() {
        int maxLines = stc->GetLineCount();
        int digitCount = std::to_string(maxLines).length();
        int marginWidth = digitCount * 11;  

        stc->SetMarginWidth(0, marginWidth);
    };

    updateMarginWidth();

    stc->Bind(wxEVT_STC_UPDATEUI, [updateMarginWidth, stc](wxStyledTextEvent& event) {
        updateMarginWidth();
    });

    rightSizer->Add(stc, 1, wxEXPAND | wxALL, 5);
    rightPanel->SetSizer(rightSizer);

    splitter->SplitVertically(leftPanel, rightPanel);
    splitter->SetSashGravity(0);
    splitter->SetSashPosition(splitter->GetMinimumPaneSize());

    CreateMenuBar();
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
