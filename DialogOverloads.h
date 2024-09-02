#pragma once
#include <wx/wx.h>
#include <wx/numdlg.h>

struct Values
{
    double valuesArr[10];
};

extern Values gValues;

class NumericInputDialog : public wxDialog {
public:
    NumericInputDialog(wxWindow* parent, const wxString& message, const wxString& caption, const wxString& value = "");

    wxString GetValue() const;
private:
    wxTextCtrl* textCtrl;

    void OnChar(wxKeyEvent& evt);
    void OnOK(wxCommandEvent& evt);
};

class MyNumberEntryDialog : public wxNumberEntryDialog {
public:
    MyNumberEntryDialog(wxWindow* parent, const wxString& message, const wxString& prompt, const wxString& caption, long value, long min, long max);
private:
    void SetupCustomLayout();

    void OnAdvancedButtonClicked(wxCommandEvent& evt);
    int processValues(const Values& values);



    wxButton* advancedButton;
    wxStaticText* advancedTip;
};
