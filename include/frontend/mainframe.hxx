#pragma once

// WX Components
#include <wx/wx.h> // wx Core
#include <wx/frame.h> // wxFrame
#include <wx/stc/stc.h> // wxStyledTextCtrl
#include <wx/dataview.h> // wxDataViewTreeCtrl

/**
 * @class MainFrame
 * @brief The main user interface window for the application.
 */
class MainFrame : public wxFrame {
public:
    explicit MainFrame(std::string_view title);
    ~MainFrame() override = default;

    void CreateMenuBar();
    wxString GetSQLWordList();
    void SetupTextEditor(wxWindow* parent);
    wxDataViewTreeCtrl* SetupTableTreeView(wxPanel* parent);
private:
    void OnCharAdded(wxStyledTextEvent& event);

    wxStyledTextCtrl* m_textEditor;
};