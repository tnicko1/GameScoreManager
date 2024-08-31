#pragma once
#include <wx/wx.h>
#include <wx/textentry.h>

class MyTextEntryDialog : public wxTextEntryDialog
{
public:
    MyTextEntryDialog(wxWindow* parent, const wxString& message, const wxString& caption = wxEmptyString, const wxString& value = wxEmptyString, long style = wxOK | wxCANCEL)
        : wxTextEntryDialog(parent, message, caption, value, style)
    {
        // Create a custom button
        m_customButton = new wxButton(this, wxID_ANY, "Custom Button");

        // Bind the button event
        Bind(wxEVT_BUTTON, &MyTextEntryDialog::OnCustomButtonClicked, this, m_customButton->GetId());
    }

    int ShowModal() override
    {
        int result = wxTextEntryDialog::ShowModal(); // Call the base class implementation

        // Perform customization
        CustomizeDialog();

        return result;
    }

private:
    void CustomizeDialog()
    {
        // Retrieve the button sizers and add the custom button
        wxSizer* sizer = GetSizer();
        wxSizer* buttonSizer = FindWindowById(wxID_OK)->GetParent()->GetSizer();

        if (buttonSizer)
        {
            buttonSizer->Add(m_customButton, 0, wxALL, 5);
            Layout(); // Layout the dialog to include the custom button
        }
    }

    void OnCustomButtonClicked(wxCommandEvent& event)
    {
        wxLogMessage("Custom Button Clicked");
    }

    wxButton* m_customButton;
};