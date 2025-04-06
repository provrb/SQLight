#pragma once

// WX Components
#include <wx/wx.h> // wx Core
#include <wx/frame.h> // wxFrame

/**
 * @class MainFrame
 * @brief The main user interface window for the application.
 */
class MainFrame : public wxFrame {
public:
    explicit MainFrame(std::string_view title);
    ~MainFrame() override = default;

    void CreateMenuBar();
};