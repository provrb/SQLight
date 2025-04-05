#include "frontend/app.hxx"
#include "frontend/mainframe.hxx"

/**
 * @brief Initializes the wxWidgets application.
 *
 * This function is called when the application starts. It creates the
 * main application window, sets the icon, adjusts the window size
 * to fit the display, and enforces a minimum size constraint.
 *
 * @return true Always returns true to indicate successful initialization.
 */
bool App::OnInit() {
    // Create the main application window
    MainFrame* mainFrame = new MainFrame(APP_NAME);
    mainFrame->Show(true);

    mainFrame->SetClientSize(800, 500);

    // Get display information for positioning and sizing
    //wxDisplay display(wxDisplay::GetFromWindow(mainFrame));
    //wxRect dimensions = display.GetClientArea();

    //mainFrame->SetMinClientSize(wxSize(800, 600));

    return true;
}