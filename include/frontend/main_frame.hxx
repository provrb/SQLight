#pragma once

// Backend
#include "backend/data_store.hxx"

// WX Components
#include <wx/wx.h> // wx Core
#include <wx/frame.h> // wxFrame
#include <wx/stc/stc.h> // wxStyledTextCtrl
#include <wx/dataview.h> // wxDataViewTreeCtrl
#include <wx/aui/aui.h>
#include <wx/grid.h>
#include <wx/splitter.h>
#include <wx/aui/auibook.h>

/**
 * @class MainFrame
 * @brief The main user interface window for the application.
 */
class MainFrame : public wxFrame {
public:
    explicit MainFrame(std::string_view title);
    ~MainFrame() override = default;

    wxString GetSQLWordList();
    void AppendTableColumn(const std::string& name);
private:
    // Events
    void OnCharAdded(wxStyledTextEvent& event);

    // Can the user close the aui page.
    // If its essential to the user program, bind a close event
    // and veto it.
    void PreventEssentialTabClosure(wxAuiNotebook* aui); 

    // Setup functions which setup core ui.
    void SetupMenuBar();
    void SetupWindowLeftPanel();
    void SetupWindowRightPanel();
    
    // Windows that will be put into the AUI view.
    void SetupTextEditor(wxAuiNotebook* parent);
    void SetupStructureView(wxAuiNotebook* aui);
    void SetupCommandOutput(wxAuiNotebook* aui);
    
    wxDataViewTreeCtrl* SetupTableTreeView(wxPanel* parent); // Table view on the left panel

    // UI Components
    wxStyledTextCtrl* m_textEditor; // styledTextCtrl IDE-like text editor
    wxGrid* m_tableDataView;
    wxPanel* m_windowLeftPanel;
    wxPanel* m_windowRightPanel;
    wxSplitterWindow* m_windowSplitterPanel;

    DataStore m_backend; // Backend data base
};