#include "DialogOverloads.h"
#include <sstream>
#include <iomanip>

NumericInputDialog::NumericInputDialog(wxWindow* parent, const wxString& message, const wxString& caption, const wxString& value)
    : wxDialog(parent, wxID_ANY, caption, wxDefaultPosition, wxSize(300, 150), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Message
    wxStaticText* msg = new wxStaticText(this, wxID_ANY, message);
    sizer->Add(msg, 0, wxALL, 10);
    wxStaticText* range = new wxStaticText(this, wxID_ANY, "Number from 0 to 100");
    wxStaticText* NA = new wxStaticText(this, wxID_ANY, "-1 if this criteria doesn't apply to this game");
    sizer->Add(range, 0, wxALL, 10);
    sizer->Add(NA, 0, wxALL, 10);
    // Text entry control
    textCtrl = new wxTextCtrl(this, wxID_ANY, value, wxDefaultPosition, wxSize(200, -1));
    sizer->Add(textCtrl, 0, wxALL, 10);

    // Buttons
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(sizer);

    // Bind event
    textCtrl->Bind(wxEVT_CHAR, &NumericInputDialog::OnChar, this);
    textCtrl->SetFocus();
    Bind(wxEVT_BUTTON, &NumericInputDialog::OnOK, this, wxID_OK);

    CenterOnParent();
}

wxString NumericInputDialog::GetValue() const {
    return textCtrl->GetValue();
}


void NumericInputDialog::OnChar(wxKeyEvent& evt) {
    int keyCode = evt.GetKeyCode();

    // Allow control keys (backspace, delete, arrows, etc.)
    if (wxIsdigit(keyCode) || keyCode == WXK_BACK || keyCode == WXK_DELETE ||
        keyCode == WXK_LEFT || keyCode == WXK_RIGHT || keyCode == WXK_TAB ||
        keyCode == '.' || keyCode == WXK_NUMPAD_DECIMAL || keyCode == '-') {
        evt.Skip();  // Let the event pass through if it's a valid key
    }
    else {
        // Block the event if it's not a valid character
        wxBell();  // Optional: Give feedback sound
    }
}

void NumericInputDialog::OnOK(wxCommandEvent& evt) {
    double value;
    if (textCtrl->GetValue().ToDouble(&value)) {
        if (value >= -1 && value <= 100) {
            evt.Skip();  // Allow the dialog to close
        }
        else {
            wxMessageBox("Please enter a number between -1 and 100.", "Invalid Input", wxOK | wxICON_ERROR);
        }
    }
    else {
        wxMessageBox("Please enter a valid number.", "Invalid Input", wxOK | wxICON_ERROR);
    }
}


MyNumberEntryDialog::MyNumberEntryDialog(wxWindow* parent, const wxString& message, const wxString& prompt, const wxString& caption, long value, long min, long max)
    : wxNumberEntryDialog(parent, message, prompt, caption, value, min, max) {
    SetupCustomLayout();
}

Values gValues = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

void MyNumberEntryDialog::SetupCustomLayout() {
    wxSizer* sizer = GetSizer();

    advancedButton = new wxButton(this, wxID_ANY, "Advanced");
    advancedButton->Bind(wxEVT_BUTTON, &MyNumberEntryDialog::OnAdvancedButtonClicked, this);

    advancedTip = new wxStaticText(this, wxID_ANY, "Let the algorithm calculate for you");

    sizer->Add(advancedTip, wxSizerFlags().CenterHorizontal());
    sizer->Add(advancedButton, wxSizerFlags().CenterHorizontal().Border(wxDOWN, 15));

    sizer->Fit(this);
}

void MyNumberEntryDialog::OnAdvancedButtonClicked(wxCommandEvent& evt) {
    
    NumericInputDialog visualsDialog(this, "How good are the visuals?", "Visuals");
    if (visualsDialog.ShowModal() == wxID_OK) {
        double visuals = std::stod(visualsDialog.GetValue().ToStdString());
        visualsDialog.Hide();
        NumericInputDialog soundDialog(this, "How good is the sound/music design?", "Sound/Music");
        if (soundDialog.ShowModal() == wxID_OK) {
            double soundMusic = std::stod(soundDialog.GetValue().ToStdString());
            soundDialog.Hide();
            NumericInputDialog coreDialog(this, "How good is the core loop?", "Core Loop");
            if (coreDialog.ShowModal() == wxID_OK) {
                double coreLoop = std::stod(coreDialog.GetValue().ToStdString());
                coreDialog.Hide();
                NumericInputDialog storyDialog(this, "How good is the story/character design?", "Story/Character Design");
                if (storyDialog.ShowModal() == wxID_OK) {
                    double storyDesign = std::stod(storyDialog.GetValue().ToStdString());
                    storyDialog.Hide();
                    NumericInputDialog UIDialog(this, "How good is the UI?", "Friendly UI");
                    if (UIDialog.ShowModal() == wxID_OK) {
                        double friendlyUI = std::stod(UIDialog.GetValue().ToStdString());
                        UIDialog.Hide();
                        NumericInputDialog competitionDialog(this, "How good is the competition?", "Competition");
                        if (competitionDialog.ShowModal() == wxID_OK) {
                            double competition = std::stod(competitionDialog.GetValue().ToStdString());
                            competitionDialog.Hide();
                            NumericInputDialog simDialog(this, "How good is the real world simulation?", "Real World Simulation");
                            if (simDialog.ShowModal() == wxID_OK) {
                                double realWorldSim = std::stod(simDialog.GetValue().ToStdString());
                                simDialog.Hide();
                                NumericInputDialog rawPriceDialog(this, "How much does this game normally cost?", "Raw Price");
                                if (rawPriceDialog.ShowModal() == wxID_OK) {
                                    double rawPrice = std::stod(rawPriceDialog.GetValue().ToStdString());
                                    rawPriceDialog.Hide();
                                    NumericInputDialog paidDialog(this, "How much did you pay?", "I Paid");
                                    if (paidDialog.ShowModal() == wxID_OK) {
                                        double pricePaid = std::stod(paidDialog.GetValue().ToStdString());
                                        paidDialog.Hide();
                                        gValues = { visuals, soundMusic, coreLoop, storyDesign, friendlyUI, competition, realWorldSim, rawPrice, pricePaid};
                                        double avgScore = processValues(gValues);
                                        gValues.valuesArr[9] = avgScore;
                                        double value = avgScore;
                                        std::ostringstream stream;
                                        stream << std::fixed << std::setprecision(0) << value;
                                        std::string message = "The algorithm rated this game with a score of " + stream.str();
                                        wxMessageBox(message);
                                        EndModal(wxID_HIGHEST + 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
int countNAs(const Values& s) {
    int count = 0;
    const double epsilon = 1e-6;
    for (int i = 0; i < 7; i++) {
        if (fabs(s.valuesArr[i] - 1.0) < epsilon)
            count++;
    }
    return count;
}

int MyNumberEntryDialog::processValues(const Values& values) {
    double fullScore = 0.0;
    int zeroCount = countNAs(values);
    const double epsilon = 1e-6;
    for (int i = 0; i < 7; i++) {
        if (fabs(values.valuesArr[i] - 1.0) > epsilon) {
            fullScore += values.valuesArr[i];
        }
    }
    double scoreAvg = fullScore / (7 - zeroCount);
    
    return scoreAvg;
}

