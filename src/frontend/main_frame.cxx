// Frontend
#include "frontend/main_frame.hxx"
#include "frontend/colours.hxx"
#include "frontend/file_paths.hxx"

// WX
#include <wx/splitter.h>
#include <wx/listctrl.h>
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
    
    leftSizer->Add(treeCtrl, 1, wxEXPAND | wxLEFT | wxTOP | wxBOTTOM, 8);
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
        wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxNO_BORDER
    );

    SetupTextEditor(aui);
    aui->AddPage(m_textEditor, "*SQL1");
    SetupStructureView(aui);
    SetupCommandOutput(aui);
    AppendTableColumn("hello");
    
    aui->SetControlMargin(0);

    rightSizer->Add(aui, 1, wxEXPAND | wxTOP | wxLEFT | wxBOTTOM, 7);
    rightPanel->SetSizer(rightSizer);

    splitter->SplitVertically(leftPanel, rightPanel);
    splitter->SetSashGravity(0);
    splitter->SetSashPosition(splitter->GetMinimumPaneSize());

    SetupMenuBar();
}

void MainFrame::OnCharAdded(wxStyledTextEvent& event) {
    char c = event.GetKey();
    if ( !std::isalnum(c) ) {
        return; 
    }

    int pos = m_textEditor->GetCurrentPos();
    int start = m_textEditor->WordStartPosition(pos, true);
    int wordLen = pos - start;

    wxString wordList = GetSQLWordList();

    wxArrayString words = wxSplit(wordList, ' ');

    wxArrayString filteredWords;
    wxString typedWord = m_textEditor->GetTextRange(start, pos);

    for ( const auto& word : words ) {
        if ( word.Lower().StartsWith(typedWord.Lower()) ) {
            filteredWords.Add(word);
        }
    }

    if ( filteredWords.IsEmpty() )
        return;

    wxString filteredWordList = wxJoin(filteredWords, ' ');

    m_textEditor->AutoCompShow(wordLen, filteredWordList);

    event.Skip();
}

void MainFrame::CheckNotCloseableTab(wxAuiNotebook* aui) {
    // the tabs with the following names cannot be closed as they
    // are essential to the program.
    // when a close event is fired and the page name is one of these,
    // the event should be veto'd to prevent the tab from closing.
    aui->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, [aui, this](wxAuiNotebookEvent& event) {
        wxString pageName = aui->GetPageText(event.GetSelection());
        if ( pageName == "Records" || pageName == "Output" )
            event.Veto();
    });
}

wxString MainFrame::GetSQLWordList() {
    wxString kwds =
        "from select where insert into update delete create drop alter table "
        "index view trigger begin commit rollback values primary key foreign "
        "not null default as and or like in is between join inner outer left right on";
    return kwds;
}

void MainFrame::AppendTableColumn(const std::string& name) {
    if ( !m_tableDataView )
        return;
}

void MainFrame::SetupTextEditor(wxWindow* parent) {
    // Create text editor
    m_textEditor = new wxStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxNO_BORDER);
    m_textEditor->SetMarginType(0, wxSTC_MARGIN_NUMBER);

    // Set the text editor font
    wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_textEditor->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    m_textEditor->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(0, 0, 0));

    // Auto completion settings
    m_textEditor->AutoCompSetIgnoreCase(true);

    // Set background and foreground colors. Default settings
    m_textEditor->SetCaretLineVisible(true);
    m_textEditor->SetCaretStyle(wxSTC_CARETSTYLE_BLOCK);
    m_textEditor->SetCaretForeground(wxColour(150, 150, 150));
    m_textEditor->SetCaretLineBackground(parent->GetBackgroundColour());
    m_textEditor->SetIndentationGuides(wxSTC_IV_LOOKBOTH);
    m_textEditor->SetIndent(4);

    // Set lexer for stc
    m_textEditor->SetLexer(wxSTC_LEX_SQL);

    // Set keywords for SQL
    m_textEditor->SetKeyWords(0, GetSQLWordList());

    // Syntax highlighting for SQL
    m_textEditor->StyleSetForeground(wxSTC_SQL_IDENTIFIER, AppColours::SQLSyntax::IDENTIFIER);
    m_textEditor->StyleSetForeground(wxSTC_SQL_NUMBER, AppColours::SQLSyntax::NUMBER);
    m_textEditor->StyleSetForeground(wxSTC_SQL_WORD, AppColours::SQLSyntax::WORD);
    m_textEditor->StyleSetForeground(wxSTC_SQL_COMMENT, AppColours::SQLSyntax::COMMENT);
    m_textEditor->StyleSetForeground(wxSTC_SQL_COMMENTLINE, AppColours::SQLSyntax::COMMENT);
    m_textEditor->StyleSetForeground(wxSTC_SQL_STRING, AppColours::SQLSyntax::STRING);

    // Setup keybinds
    wxAcceleratorEntry entries[6];
    entries[0].Set(wxACCEL_CTRL, ( int ) 'C', wxID_COPY);
    entries[1].Set(wxACCEL_CTRL, ( int ) 'V', wxID_PASTE);
    entries[2].Set(wxACCEL_CTRL, ( int ) 'X', wxID_CUT);
    entries[3].Set(wxACCEL_CTRL, ( int ) 'A', wxID_SELECTALL);
    entries[4].Set(wxACCEL_CTRL, ( int ) 'Z', wxID_UNDO);
    entries[5].Set(wxACCEL_CTRL, ( int ) 'Y', wxID_REDO);
    wxAcceleratorTable accel(6, entries);
    m_textEditor->SetAcceleratorTable(accel);

    m_textEditor->Bind(wxEVT_MENU, [this](wxCommandEvent&) { m_textEditor->Copy(); }, wxID_COPY);
    m_textEditor->Bind(wxEVT_MENU, [this](wxCommandEvent&) { m_textEditor->Paste(); }, wxID_PASTE);
    m_textEditor->Bind(wxEVT_MENU, [this](wxCommandEvent&) { m_textEditor->Cut(); }, wxID_CUT);
    m_textEditor->Bind(wxEVT_MENU, [this](wxCommandEvent&) { m_textEditor->SelectAll(); }, wxID_SELECTALL);
    m_textEditor->Bind(wxEVT_MENU, [this](wxCommandEvent&) { m_textEditor->Undo(); }, wxID_UNDO);
    m_textEditor->Bind(wxEVT_MENU, [this](wxCommandEvent&) { m_textEditor->Redo(); }, wxID_REDO);

    /*
        Bind events for features like auto complete,
        or essential resizing
    */

    // Whenever a character is added, show auto complete suggestions
    m_textEditor->Bind(wxEVT_STC_CHARADDED, &MainFrame::OnCharAdded, this);

    // Resize the margin containing line numbers when a digit is over 100
    m_textEditor->Bind(wxEVT_STC_UPDATEUI, [this](wxStyledTextEvent& event) {
        int maxLines = m_textEditor->GetLineCount();
        int digitCount = std::to_string(maxLines).length();
        int marginWidth = digitCount * 12;

        m_textEditor->SetMarginWidth(0, marginWidth);
        });
}

wxDataViewTreeCtrl* MainFrame::SetupTableTreeView(wxPanel* parent) {
    wxDataViewTreeCtrl* treeCtrl = new wxDataViewTreeCtrl(
        parent,
        wxID_ANY,
        wxDefaultPosition, wxDefaultSize,
        wxDV_ROW_LINES | wxDV_VERT_RULES | wxDV_MULTIPLE | wxDV_HORIZ_RULES | wxBORDER_STATIC
    );

    // Row icons
    wxImage tableIcon(std::string(ASSET_DIR) + "table.png", wxBITMAP_TYPE_PNG);
    wxImage viewsIcon(std::string(ASSET_DIR) + "views.png", wxBITMAP_TYPE_PNG);
    wxImage indicesIcon(std::string(ASSET_DIR) + "indices.png", wxBITMAP_TYPE_PNG);
    wxImage triggersIcon(std::string(ASSET_DIR) + "triggers.png", wxBITMAP_TYPE_PNG);
    tableIcon = tableIcon.Scale(15, 16, wxIMAGE_QUALITY_HIGH);
    viewsIcon = viewsIcon.Scale(15, 16, wxIMAGE_QUALITY_HIGH);
    indicesIcon = indicesIcon.Scale(15, 16, wxIMAGE_QUALITY_HIGH);
    triggersIcon = triggersIcon.Scale(15, 16, wxIMAGE_QUALITY_HIGH);

    wxDataViewColumn* nameCol = treeCtrl->GetColumn(0);
    nameCol->SetResizeable(true);
    nameCol->SetMinWidth(60);
    nameCol->SetWidth(wxCOL_WIDTH_DEFAULT);
    nameCol->SetWidth(wxCOL_WIDTH_AUTOSIZE);
    nameCol->SetTitle("Name");

    wxDataViewColumn* typeCol = treeCtrl->AppendTextColumn("Type", 1);
    typeCol->SetMinWidth(40);

    treeCtrl->SetRowHeight(19);

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

    // Set row icons
    treeCtrl->SetItemIcon(tables, wxBitmap(tableIcon));
    treeCtrl->SetItemIcon(views, wxBitmap(viewsIcon));
    treeCtrl->SetItemIcon(indexes, wxBitmap(indicesIcon));
    treeCtrl->SetItemIcon(triggers, wxBitmap(triggersIcon));

    treeCtrl->SetAlternateRowColour(wxColour(240, 240, 240));

    return treeCtrl;
}

void MainFrame::SetupStructureView(wxAuiNotebook* aui) {
    if ( m_tableDataView ) // already been setup
        return;

    wxPanel* structureView = new wxPanel(aui);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    // create editing grid
    m_tableDataView = new wxGrid(structureView, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_STATIC);
    m_tableDataView->CreateGrid(16, 5);
    m_tableDataView->SetLabelFont(wxFontInfo(9).Weight(wxFONTWEIGHT_NORMAL));
    m_tableDataView->SetColLabelSize(20);
    m_tableDataView->DisableOverlaySelection();
    m_tableDataView->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
    m_tableDataView->SetLabelBackgroundColour(*wxWHITE);
    m_tableDataView->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);

    for ( int i = 1; i <= m_tableDataView->GetNumberRows(); i++ ) {
        int digitCount = std::to_string(i).length();
        int width = digitCount * 11;
        m_tableDataView->SetRowLabelSize(width);
    }

    // add choices to select a table
    wxArrayString options;
    options.Add("O1");
    options.Add("O2");
    options.Add("O3");

    wxStaticText* selectLabel = new wxStaticText(structureView, wxID_ANY, "&Table: ");
    wxChoice* dropdown = new wxChoice(structureView, wxID_ANY, wxDefaultPosition, wxSize(150, 20), options);
    structureView->SetBackgroundColour(wxColour(255, 255, 255));

    wxTextCtrl* cellInfo = new wxTextCtrl(structureView, wxID_ANY, wxEmptyString,  wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC | wxTE_READONLY);
    // cellInfo->SetBackgroundColour(*wxWHITE);
    cellInfo->SetForegroundColour(wxColour(120, 120, 120));
    cellInfo->AppendText("Cell information will appear here...");

    wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
    hSizer->Add(selectLabel, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxLEFT, 5); 
    hSizer->Add(dropdown, 0, wxTOP, 5);

    wxStaticText* infoLabel = new wxStaticText(structureView, wxID_ANY, "&Cell Info: Row 0, Column 0");
    wxBoxSizer* hSizerBottom = new wxBoxSizer(wxHORIZONTAL);
    hSizerBottom->Add(infoLabel, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxLEFT, 5);

    sizer->Add(hSizer);
    m_tableDataView->SetMaxSize(wxSize(INT_MAX, 200));
    sizer->Add(m_tableDataView, 0, wxEXPAND | wxTOP, 5);
    sizer->Add(hSizerBottom);
    sizer->Add(cellInfo, 1, wxEXPAND | wxTOP, 5);
    sizer->SetItemMinSize(cellInfo, 100, 100);
    structureView->SetSizer(sizer);

    aui->AddPage(structureView, "Records"); // add page
    CheckNotCloseableTab(aui);
}

void MainFrame::SetupCommandOutput(wxAuiNotebook* aui) {
    wxPanel* output = new wxPanel(aui);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* textCtrl = new wxTextCtrl(output, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_READONLY);

    textCtrl->SetForegroundColour(wxColour(120, 120, 120));
    output->SetBackgroundColour(*wxWHITE);

    textCtrl->AppendText("Command output will appear here...");

    sizer->Add(textCtrl, 1, wxEXPAND | wxTOP, 5);
    output->SetSizer(sizer);
    aui->AddPage(output, "Output");
    CheckNotCloseableTab(aui);
}

void MainFrame::SetupMenuBar() {
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