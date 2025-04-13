// Frontend
#include "frontend/main_frame.hxx"

/**
 * @brief Handles character addition events to provide SQL keyword auto-completion.
 *
 * This function is triggered when a character is added to the editor. It checks if
 * the character is alphanumeric, then determines the current word being typed. The
 * function retrieves a list of SQL keywords and filters them based on the partially
 * typed word. If matches are found, an auto-completion list is displayed.
 *
 * The auto-completion is case-insensitive and only activates when typing alphanumeric
 * characters that could be part of a SQL keyword. Non-alphanumeric characters are
 * ignored.
 *
 * @param event The wxStyledTextEvent containing information about the character added.
 *              The event's key property is used to determine if auto-completion should trigger.
 *
 * @note The function uses GetSQLWordList() to obtain the list of SQL keywords for completion.
 * @note The event is always skipped at the end to allow further processing by other handlers.
 */
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