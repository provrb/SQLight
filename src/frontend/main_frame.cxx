// Frontend
#include "frontend/main_frame.hxx"

// WX
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/aui/auibook.h>

/**
 * @brief Constructs the main application window and initializes all UI components.
 *
 * This constructor creates the primary application interface with:
 * - A splitter window dividing left and right panels
 * - Left panel containing a tree control for database navigation
 * - Right panel with an AUI notebook containing:
 *   * SQL editor tab
 *   * Table structure view
 *   * Command output console
 *
 * The window is initialized with default size (800x600) and sets up:
 * - Image handlers for wxWidgets
 * - Default window title ("SQLight - No file open")
 * - Minimum pane size constraints
 * - Menu bar functionality
 *
 * @param title The window title to display (converted from std::string_view).
 *              This is initially overridden by "SQLight - No file open".
 */
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

/**
 * @brief Prevents closure of essential application tabs.
 *
 * This function binds an event handler to veto close events for specific tabs
 * that are mandatory to the application's functionality. Currently protects:
 * - "Records" tab
 * - "Output" tab
 *
 * When a user attempts to close one of these protected tabs, the close operation
 * will be cancelled while allowing other tabs to close normally.
 *
 * @param aui Pointer to the wxAuiNotebook control containing the tabs to protect.
 *            Must be a valid notebook control. The function binds directly to
 *            this notebook's close events.
 */
void MainFrame::PreventEssentialTabClosure(wxAuiNotebook* aui) {
    if ( !aui )
        return;

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

/**
 * @brief Retrieves a space-separated string of SQL keywords for auto-completion.
 *
 * This function provides a comprehensive list of common SQL keywords that are used
 * for auto-completion in the code editor. The keywords include basic SQL commands,
 * clauses, operators, and constraints.
 *
 *
 * @return wxString A space-delimited string containing SQL keywords in lowercase.
 */
wxString MainFrame::GetSQLWordList() {
    wxString kwds =
        "from select where insert into update delete create drop alter table "
        "index view trigger begin commit rollback values primary key foreign "
        "not null default as and or like in is between join inner outer left right on";
    return kwds;
}

// TODO
void MainFrame::AppendTableColumn(const std::string& name) {
    if ( !m_tableDataView )
        return;
}
