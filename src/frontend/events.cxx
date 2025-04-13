#include "frontend/main_frame.hxx"

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