#include "frontend/mainframe.hxx"

#include <wx/treectrl.h>
#include <wx/splitter.h>

MainFrame::MainFrame(std::string_view title)
    : wxFrame(nullptr, wxID_ANY, title.data(), wxDefaultPosition, wxSize(800, 600))
{
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

    wxTextCtrl* textCtrl = new wxTextCtrl(rightPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    rightSizer->Add(textCtrl, 1, wxEXPAND | wxALL, 5);
    rightPanel->SetSizer(rightSizer);

    splitter->SplitVertically(leftPanel, rightPanel);
    splitter->SetSashGravity(0);
    splitter->SetSashPosition(splitter->GetMinimumPaneSize());
}