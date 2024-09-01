#include "DialogOverloads.h"

MyNumberEntryDialog::MyNumberEntryDialog(wxWindow* parent, const wxString& message, const wxString& prompt, const wxString& caption, long value, long min, long max)
    : wxNumberEntryDialog(parent, message, prompt, caption, value, min, max) {
    SetupCustomLayout();
}

Values gValues = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
    
    wxNumberEntryDialog visualsDialog(this, "How good are the visuals?", "-1 for not applicable", "Visuals", 0, -1, 100);
    if (visualsDialog.ShowModal() == wxID_OK) {
        int visuals = visualsDialog.GetValue();
        visualsDialog.Hide();
        wxNumberEntryDialog soundDialog(this, "How good is the sound/music design?", "-1 for not applicable", "Sound/Music", 0, -1, 100);
        if (soundDialog.ShowModal() == wxID_OK) {
            int soundMusic = soundDialog.GetValue();
            soundDialog.Hide();
            wxNumberEntryDialog coreDialog(this, "How good is the core loop?", "-1 for not applicable", "Core Loop", 0, -1, 100);
            if (coreDialog.ShowModal() == wxID_OK) {
                int coreLoop = coreDialog.GetValue();
                coreDialog.Hide();
                wxNumberEntryDialog storyDialog(this, "How good is the story/character design?", "-1 for not applicable", "Story/Character Design", 0, -1, 100);
                if (storyDialog.ShowModal() == wxID_OK) {
                    int storyDesign = storyDialog.GetValue();
                    storyDialog.Hide();
                    wxNumberEntryDialog UIDialog(this, "How good is the UI?", "-1 for not applicable", "Friendly UI", 0, -1, 100);
                    if (UIDialog.ShowModal() == wxID_OK) {
                        int friendlyUI = UIDialog.GetValue();
                        UIDialog.Hide();
                        wxNumberEntryDialog competitionDialog(this, "How good is the competition?", "-1 for not applicable", "Competition", 0, -1, 100);
                        if (competitionDialog.ShowModal() == wxID_OK) {
                            int competition = competitionDialog.GetValue();
                            competitionDialog.Hide();
                            wxNumberEntryDialog simDialog(this, "How good is the real world simulation?", "-1 for not applicable", "Real World Simulation", 0, -1, 100);
                            if (simDialog.ShowModal() == wxID_OK) {
                                int realWorldSim = simDialog.GetValue();
                                simDialog.Hide();
                                wxNumberEntryDialog rawPriceDialog(this, "How much does this game normally cost?", "0 for free", "Raw Price", 0, -1, 1000);
                                if (rawPriceDialog.ShowModal() == wxID_OK) {
                                    int rawPrice = rawPriceDialog.GetValue();
                                    rawPriceDialog.Hide();
                                    wxNumberEntryDialog paidDialog(this, "How much did you pay?", "0 if you got it for free", "Price Paid", 0, -1, 1000);
                                    if (paidDialog.ShowModal() == wxID_OK) {
                                        int pricePaid = paidDialog.GetValue();
                                        paidDialog.Hide();
                                        gValues = { visuals, soundMusic, coreLoop, storyDesign, friendlyUI, competition, realWorldSim, rawPrice, pricePaid};
                                        int avgScore = processValues(gValues);
                                        gValues.valuesArr[9] = avgScore;
                                        std::string message = "The algorithm rated this game with a score of " + std::to_string(avgScore);
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
    for (int i = 0; i < 7; i++) {
        if (s.valuesArr[i] == -1)
            count++;
    }
    return count;
}

int MyNumberEntryDialog::processValues(const Values& values) {
    int fullScore = 0;
    int zeroCount = countNAs(values);
    for (int i = 0; i < 7; i++) {
        if (values.valuesArr[i] != -1) {
            fullScore += values.valuesArr[i];
        }
    }
    int scoreAvg = fullScore / (7 - zeroCount);
    return scoreAvg;
}

