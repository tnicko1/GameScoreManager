#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "SavedGames.h"


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	void CreateControls();
	void BindEventHandlers();
	void SetupSizers();
	void AddSavedGames();
	void ColorCodeItems();

	void OnAddButtonClick(wxCommandEvent& evt);
	void OnInputEnterClick(wxCommandEvent& evt);

	void OnListItemSelected(wxListEvent& evt);
	void OnListItemDeselected(wxMouseEvent& evt);
	long selectedItemIndex;
	void OnDelKeyButtonClick(wxCommandEvent& evt);

	void OnColumnFilter(wxListEvent& evt);
	int lastClickedColumn;
	int clickCount;


	bool CompareGameToList(const wxString& gameName);

	std::vector<Games> GetItems();
	void OnCloseWindow(wxCloseEvent& evt);

	wxPanel* panel;
	wxStaticText* headlineText;
	wxButton* addButton;
	wxTextCtrl* inputField;
	wxListCtrl* gamesListBox;
	wxButton* deleteButton;
};

