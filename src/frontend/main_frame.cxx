// Frontend
#include "frontend/main_frame.hxx"


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

// TODO
void MainFrame::AppendTableColumn(const std::string& name) {
    if ( !m_tableDataView )
        return;
}
