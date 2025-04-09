#pragma once

// WX Components
#include <wx/wx.h> // wx Core
#include <wx/frame.h> // wxFrame
#include <wx/stc/stc.h> // wxStyledTextCtrl
#include <wx/dataview.h> // wxDataViewTreeCtrl
#include <wx/aui/aui.h>

/**
 * @class MainFrame
 * @brief The main user interface window for the application.
 */
class MainFrame : public wxFrame {
public:
    explicit MainFrame(std::string_view title);
    ~MainFrame() override = default;

    wxString GetSQLWordList();
private:
    // Events
    void OnCharAdded(wxStyledTextEvent& event);

    // Can the user close the aui page.
    // If its essential to the user program, bind a close event
    // and veto it.
    void CheckNotCloseableTab(wxAuiNotebook* aui); 

    // Setup functions which setup core ui.
    // See 'setup.hxx' for function implementations
    void SetupMenuBar();
    void SetupTextEditor(wxWindow* parent);
    void SetupStructureView(wxAuiNotebook* aui);
    void SetupCommandOutput(wxAuiNotebook* aui);
    wxDataViewTreeCtrl* SetupTableTreeView(wxPanel* parent);

    wxStyledTextCtrl* m_textEditor; // styledTextCtrl IDE-like text editor
};