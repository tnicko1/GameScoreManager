#include "MainFrame.h"
#include "SavedGames.h"
#include <cstddef> //for std::size_t
#include <wx/numdlg.h>
#include "DialogOverloads.h"
#include <cstdlib> //for std::stoi

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	SetupSizers();
	BindEventHandlers();
	AddSavedGames();
	ColorCodeItems();
}

void MainFrame::CreateControls() {
	panel = new wxPanel(this);

	headlineText = new wxStaticText(panel, wxID_ANY, "Enter Game Scores");
	wxFont headlineFont(wxFontInfo(wxSize(0, 28)).Bold());
	headlineText->SetFont(headlineFont);

	addButton = new wxButton(panel, wxID_ANY, "Add a new game");
	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxTE_PROCESS_ENTER);
	inputField->Show(false);

	gamesListBox = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);

	gamesListBox->InsertColumn(0, "Games");
	gamesListBox->InsertColumn(1, "Scores");
	gamesListBox->InsertColumn(2, "Visuals");
	gamesListBox->InsertColumn(3, "Sound/Music");
	gamesListBox->InsertColumn(4, "Core Loop");
	gamesListBox->InsertColumn(5, "Story/Character Design");
	gamesListBox->InsertColumn(6, "Friendly UI");
	gamesListBox->InsertColumn(7, "Competition");
	gamesListBox->InsertColumn(8, "Real World Simulation");
	gamesListBox->InsertColumn(9, "Raw Price");
	gamesListBox->InsertColumn(10, "I Paid");
	gamesListBox->SetColumnWidth(0, 170);
	gamesListBox->SetColumnWidth(1, 55);
	gamesListBox->SetColumnWidth(2, 60);
	gamesListBox->SetColumnWidth(3, 100);
	gamesListBox->SetColumnWidth(4, 85);
	gamesListBox->SetColumnWidth(5, 170);
	gamesListBox->SetColumnWidth(6, 83);
	gamesListBox->SetColumnWidth(7, 95);
	gamesListBox->SetColumnWidth(8, 165);
	gamesListBox->SetColumnWidth(10, 68);

	filterButton = new wxButton(panel, wxID_ANY, "Filter");
	deleteButton = new wxButton(panel, wxID_DELETE, "Delete");
	deleteButton->Show(false);

}

void MainFrame::BindEventHandlers() {
	gamesListBox->Bind(wxEVT_LIST_ITEM_SELECTED, &MainFrame::OnListItemSelected, this);
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClick, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnterClick, this);
	deleteButton->Bind(wxEVT_BUTTON, &MainFrame::OnDelKeyButtonClick, this, wxID_DELETE);
	gamesListBox->Bind(wxEVT_LIST_COL_BEGIN_DRAG, &MainFrame::OnResize, this);
	filterButton->Bind(wxEVT_BUTTON, &MainFrame::OnFilterButtonClick, this);


	panel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnListItemDeselected, this);
	headlineText->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnListItemDeselected, this);

	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnCloseWindow, this);
}

void MainFrame::SetupSizers() {
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* headlineSizer = new wxBoxSizer(wxHORIZONTAL);
	headlineSizer->Add(headlineText, wxSizerFlags().Proportion(1));
	headlineSizer->AddSpacer(5);
	headlineSizer->Add(inputField);
	headlineSizer->Add(addButton);
	

	mainSizer->Add(headlineSizer, wxSizerFlags().Expand());
	mainSizer->AddSpacer(25);
	mainSizer->Add(gamesListBox, wxSizerFlags().Proportion(1).Expand());
	mainSizer->AddSpacer(5);

	wxBoxSizer* listButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
	listButtonsSizer->Add(filterButton);
	listButtonsSizer->AddSpacer(5);
	listButtonsSizer->Add(deleteButton);
	mainSizer->Add(listButtonsSizer);

	wxGridSizer* outerSizer = new wxGridSizer(1);
	outerSizer->Add(mainSizer, wxSizerFlags().Border(wxALL, 20).Expand());

	panel->SetSizer(outerSizer);
	outerSizer->SetSizeHints(this);
}



void MainFrame::AddSavedGames() {
	std::vector<Games> gamesVec = LoadGamesFromFile("savedgames.txt");
	std::sort(gamesVec.begin(), gamesVec.end(), [](Games& game1, Games& game2) {
		return game1.values[0] > game2.values[0];
		});

	for (int i = 0; i < gamesVec.size(); i++) {
		gamesListBox->InsertItem(i, gamesVec[i].gameName);
		gamesListBox->SetItem(i, 1, std::to_string(gamesVec[i].values[0]));
		for (int j = 2; j <= 10; j++) {
			std::string itemText;

			if (gamesVec[i].values[j - 1] == 0) {
				itemText = "Free";
			}
			else if (gamesVec[i].values[j - 1] == -1) {
				itemText = "NA";
			}
			else if (j == 9 || j == 10) {
				itemText = "$" + std::to_string(gamesVec[i].values[j - 1]);
			}
			else {
				itemText = std::to_string(gamesVec[i].values[j - 1]);
			}

			gamesListBox->SetItem(i, j, itemText);
		}
	}
}


void MainFrame::ColorCodeItems() {
	int itemCount = gamesListBox->GetItemCount();
	for (int i = 0; i < itemCount; i++) {
		int value = std::stoi(gamesListBox->GetItemText(i, 1).ToStdString());

		if (value < 20) {
			wxColour red(255, 0, 0);
			gamesListBox->SetItemBackgroundColour(i, red);
		}
		else if (value < 40) {
			wxColour orange(255, 167, 0);
			gamesListBox->SetItemBackgroundColour(i, orange);
		}
		else if (value < 60) {
			wxColour yellow(255, 244, 0);
			gamesListBox->SetItemBackgroundColour(i, yellow);
		}
		else if (value < 80) {
			wxColour lightGreen(163, 255, 0);
			gamesListBox->SetItemBackgroundColour(i, lightGreen);
		}
		else if (value <= 100) {
			wxColour green(44, 186, 0);
			gamesListBox->SetItemBackgroundColour(i, green);
		}
	}
}

void MainFrame::OnAddButtonClick(wxCommandEvent& evt) {
	addButton->Show(false);
	inputField->Show(true);
	inputField->Update();
	panel->Layout();
	inputField->SetFocus();
}

void CapitalizeAfterSpaces(std::string& str)
{
	bool capitalize = true;

	for (char& ch : str)
	{
		if (ch == ' ')
		{
			capitalize = true;
		}
		else if (capitalize && std::isalpha(ch))
		{
			ch = std::toupper(ch);
			capitalize = false;
		}
		else
		{
			capitalize = false;
		}
	}
}

void MainFrame::OnInputEnterClick(wxCommandEvent& evt) {
	wxString gameName = inputField->GetValue().MakeLower();
	if (gameName.IsEmpty()) {
		inputField->Show(false);
		addButton->Show(true);
		return;
	}
	else if (CompareGameToList(gameName)) {
		wxMessageBox("This game is already on the list!");
		inputField->Clear();
		return;
	}

	int userInput;
	MyNumberEntryDialog dialog(this, "", "Enter Score: ", "Scoring the game", 0, 0, 100);
	int modalID = dialog.ShowModal();
	if (modalID == wxID_OK) {
		userInput = dialog.GetValue();
		long itemCount = gamesListBox->GetItemCount();
		gamesListBox->InsertItem(itemCount, gameName);
		gamesListBox->SetItem(itemCount, 1, std::to_string(userInput));
		for (int j = 2; j <= 10; j++) {
			gamesListBox->SetItem(itemCount, j, "-1");
		}
		inputField->Clear();
		inputField->Show(false);
		addButton->Show(true);

		std::vector<Games> savedGames = GetItems();
		SaveGamesToFile(savedGames, "savedgames.txt");
		std::vector<Games> gamesVec = LoadGamesFromFile("savedgames.txt");
		std::sort(gamesVec.begin(), gamesVec.end(), [](Games& game1, Games& game2) {
			return game1.values[0] > game2.values[0];
			});
		gamesListBox->DeleteAllItems();

		for (int i = 0; i < gamesVec.size(); i++) {
			CapitalizeAfterSpaces(gamesVec[i].gameName);
			gamesListBox->InsertItem(i, gamesVec[i].gameName);
			gamesListBox->SetItem(i, 1, std::to_string(gamesVec[i].values[0]));


			for (int j = 2; j <= 10; j++) {
				std::string itemText;

				if (gamesVec[i].values[j - 1] == 0) {
					itemText = "Free";
				}
				else if (gamesVec[i].values[j - 1] == -1) {
					itemText = "NA";
				}
				else if (j == 9 || j == 10) {
					itemText = "$" + std::to_string(gamesVec[i].values[j - 1]);
				}
				else {
					itemText = std::to_string(gamesVec[i].values[j - 1]);
				}

				gamesListBox->SetItem(i, j, itemText);
			}


		}
		ColorCodeItems();
	}
	else if (modalID == wxID_CANCEL) {
		inputField->Clear();
		inputField->Show(false);
		addButton->Show(true);
	}
	else if (modalID == (wxID_HIGHEST + 1)) {
		long itemCount = gamesListBox->GetItemCount();
		gamesListBox->InsertItem(itemCount, gameName);
		gamesListBox->SetItem(itemCount, 1, std::to_string(gValues.valuesArr[9]));
		for (int j = 2; j <= 10; j++) {
			gamesListBox->SetItem(itemCount, j, std::to_string(gValues.valuesArr[j - 2]));
		}
		inputField->Clear();
		inputField->Show(false);
		addButton->Show(true);

		std::vector<Games> savedGames = GetItems();
		SaveGamesToFile(savedGames, "savedgames.txt");
		std::vector<Games> gamesVec = LoadGamesFromFile("savedgames.txt");
		std::sort(gamesVec.begin(), gamesVec.end(), [](Games& game1, Games& game2) {
			return game1.values[0] > game2.values[0];
			});
		gamesListBox->DeleteAllItems();

		for (int i = 0; i < gamesVec.size(); i++) {
			CapitalizeAfterSpaces(gamesVec[i].gameName);
			gamesListBox->InsertItem(i, gamesVec[i].gameName);
			gamesListBox->SetItem(i, 1, std::to_string(gamesVec[i].values[0]));


			for (int j = 2; j <= 10; j++) {
				std::string itemText;

				if (gamesVec[i].values[j - 1] == 0) {
					itemText = "Free";
				}
				else if (gamesVec[i].values[j - 1] == -1) {
					itemText = "NA";
				}
				else if (j == 9 || j == 10) {
					itemText = "$" + std::to_string(gamesVec[i].values[j - 1]);
				}
				else {
					itemText = std::to_string(gamesVec[i].values[j - 1]);
				}

				gamesListBox->SetItem(i, j, itemText);
			}


		}
		ColorCodeItems();
	}
	
	
}

void MainFrame::OnListItemSelected(wxListEvent& evt) {
	selectedItemIndex = evt.GetIndex();
	deleteButton->Show(true);
	panel->Layout();
}

void MainFrame::OnListItemDeselected(wxMouseEvent& evt) {
	gamesListBox->SetItemState(selectedItemIndex, 0, wxLIST_STATE_SELECTED);
}

void MainFrame::OnDelKeyButtonClick(wxCommandEvent& evt) {
	if (selectedItemIndex != wxNOT_FOUND) {
		int response = wxMessageBox("Are you sure you want to delete the selected item? this action is irreversible", "Confirm Deletion", wxYES_NO | wxICON_WARNING);
		if (response == wxYES) {
			gamesListBox->DeleteItem(selectedItemIndex);
			selectedItemIndex = wxNOT_FOUND;
		}
	}
}

void MainFrame::OnResize(wxListEvent& evt) {
	evt.Veto();
	wxMessageBox("Please resize from settings menu", "Caution", wxOK);
}

void MainFrame::OnFilterButtonClick(wxCommandEvent& evt)
{
	wxArrayString choices;
	choices.Add("Score: High to Low");
	choices.Add("Score: Low to High");
	choices.Add("Alphabetically");
	wxSingleChoiceDialog dialog(this, "Choose the filter option: ", "Filter", choices);
	if (dialog.ShowModal() == wxID_OK) {
		std::string userFilter = dialog.GetStringSelection().ToStdString();
		std::vector<Games> gamesVec = LoadGamesFromFile("savedgames.txt");
		if (userFilter == "Score: Low to High") {
			std::sort(gamesVec.begin(), gamesVec.end(), [](Games& game1, Games& game2) {
				return game1.values[0] < game2.values[0];
				});
		}
		else if (userFilter == "Score: High to Low") {
			std::sort(gamesVec.begin(), gamesVec.end(), [](Games& game1, Games& game2) {
				return game1.values[0] > game2.values[0];
				});
		}
		else if (userFilter == "Alphabetically") {
			std::sort(gamesVec.begin(), gamesVec.end(), [](Games& game1, Games& game2) {
				return game1.gameName < game2.gameName;
				});
		}
		gamesListBox->DeleteAllItems();

		for (int i = 0; i < gamesVec.size(); i++) {
			CapitalizeAfterSpaces(gamesVec[i].gameName);
			gamesListBox->InsertItem(i, gamesVec[i].gameName);
			gamesListBox->SetItem(i, 1, std::to_string(gamesVec[i].values[0]));
			for (int j = 2; j <= 10; j++) {
				std::string itemText;

				if (gamesVec[i].values[j - 1] == 0) {
					itemText = "Free";
				}
				else if (gamesVec[i].values[j - 1] == -1) {
					itemText = "NA";
				}
				else if (j == 9 || j == 10) {
					itemText = "$" + std::to_string(gamesVec[i].values[j - 1]);
				}
				else {
					itemText = std::to_string(gamesVec[i].values[j - 1]);
				}

				gamesListBox->SetItem(i, j, itemText);
			}
			
		}
		ColorCodeItems();
	}
}

bool MainFrame::CompareGameToList(const wxString& gameName) {
	long itemCount = gamesListBox->GetItemCount();
	
	for (int i = 0; i < itemCount; i++) {
		wxString gameOnList = gamesListBox->GetItemText(i).MakeLower();
		
		if (gameName == gameOnList) {
			return true; //game found
		}
	}
	return false;
}

std::vector<Games> MainFrame::GetItems() {
	std::vector<Games> gamesVec;

	int itemCount = gamesListBox->GetItemCount();

	for (int i = 0; i < itemCount; i++) {
		Games game;
		std::string col1Value = gamesListBox->GetItemText(i, 0).ToStdString();
		game.gameName = col1Value;

		wxListItem item;
		item.SetId(i);
		for (int j = 0; j < 10; j++) {
			item.SetColumn(j + 1);
			gamesListBox->GetItem(item);
			if (item.GetText() == "Free") {
				game.values[j] = 0;
			}
			else if (item.GetText() == "NA") {
				game.values[j] = -1;
			}
			else if (j == 8 || j == 9) {
				std::string str = item.GetText().ToStdString();
				if (!str.empty() && str[0] == '$') {
					str = str.substr(1); // Remove the first character
				}
				try {
					game.values[j] = std::stoi(str);
				}
				catch (const std::invalid_argument& e) {
					wxMessageBox("invalid argument"); // Or handle the error as appropriate
				}
			}
			else {
				game.values[j] = std::stoi(item.GetText().ToStdString());
			}
		}
		gamesVec.push_back(game);
	}
	return gamesVec;
}

void MainFrame::OnCloseWindow(wxCloseEvent& evt) {
	std::vector<Games> gamesVec = GetItems();

	SaveGamesToFile(gamesVec, "savedgames.txt");
	evt.Skip();
}

