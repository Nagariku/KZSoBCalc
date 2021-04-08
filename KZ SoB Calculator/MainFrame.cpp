#include "MainFrame.h"

using utils::GetColor;
using utils::GetSteamId;
using utils::GetGalaxyId;
using utils::CreateBackupDirectory;
using utils::ShowErrorMessage;
using utils::CreateTimeString;
using utils::GetCurrentDatetimeString;

// ----- Event table -----

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(IDs::Open_Settings, MainFrame::ShowSettings)
    EVT_BUTTON(IDs::Reload_Times, MainFrame::UpdateTimesEvent)
    EVT_BUTTON(IDs::Save_Settings, MainFrame::SaveSettingsEvent)
    EVT_BUTTON(IDs::Save_Backup, MainFrame::SaveBackupFile)
    EVT_BUTTON(IDs::Load_Backup, MainFrame::LoadBackupFile)

    EVT_COMBOBOX(IDs::Difficulty_Box, MainFrame::UpdateDifficultyEvent)
    EVT_COMBOBOX(IDs::RunType_Box, MainFrame::UpdateRunTypeEvent)
    EVT_COMBOBOX(IDs::Category_Box, MainFrame::UpdateCategoryEvent)
    EVT_COMBOBOX(IDs::Game_Store_Box, MainFrame::UpdateGameStoreEvent)
wxEND_EVENT_TABLE()


// ----- Class declaration -----

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Cherry's KZ SoB Calculator", wxPoint(100, 100), wxSize(295, 514), wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER)
{
    this->PreSizer();
    this->CreateSizer();
    this->PostSizer();
}

void MainFrame::PreSizer()
{
    this->SetBackgroundColour(GetColor("White"));
    this->SetFont(Font);

    this->is_settings_shown = settings_json["show_settings"].get<bool>();
    this->difficulty = Difficulty::NORMAL;
    this->run_type = RunType::ACTION_ONLY;

    this->SetupIcon();
}

void MainFrame::PostSizer()
{
    wxWindow* open_settings = wxWindow::FindWindowById(IDs::Open_Settings);
    open_settings->SetMinSize(wxSize(-1, 38));

    wxWindow::FindWindowById(IDs::Save_Settings)->SetMinSize(wxSize(-1, 38));
    wxWindow::FindWindowById(IDs::Reload_Times)->SetMinSize(wxSize(-1, 38));

    this->settings_sizer->ShowItems(this->is_settings_shown);

    this->SetupGameStore();
    this->SetupSettings();

    this->UpdateTimes();
    this->UpdateRunDescription();

    this->SetSize(this->GetBestSize());
}


// ----- Misc functions -----

void MainFrame::Terminate()
{
    this->ternimated = true;
    this->Destroy();
}

void MainFrame::SaveSettings() {
    settings_json["run_type"] = this->settings->run_type->GetValue();
    settings_json["category"] = this->settings->category->GetValue();
    settings_json["difficulty"] = this->settings->difficulty->GetValue();
    settings_json["show_settings"] = this->settings->settings_visibility->GetValue();
    if (this->settings->game_store->GetStrings().GetCount() == 2) {
        settings_json["game_store"] = this->settings->game_store->GetValue();
    }
    WriteSettingsFile();
}


// ----- Setup functions -----

void MainFrame::SetupIcon() 
{
    wxIcon icon = wxIcon(wxString("images\\icon.ico"), wxBITMAP_TYPE_ICO);
    this->SetIcon(icon);
}


void MainFrame::SetupSettings() 
{
    this->difficulty = this->settings->difficulty->FindString(settings_json["difficulty"].get<std::string>());
    this->settings->difficulty->SetSelection(this->difficulty);

    this->run_type = this->settings->run_type->FindString(settings_json["run_type"].get<std::string>());
    this->settings->run_type->SetSelection(this->run_type);

    int category = this->settings->category->FindString(settings_json["category"].get<std::string>());
    this->settings->category->SetSelection(category);
    this->UpdateCategory();

    if (this->settings->game_store->GetStrings().GetCount() == 2) {
        int game_store = this->settings->game_store->FindString(settings_json["game_store"].get<std::string>());
        this->settings->game_store->SetSelection(game_store);
    }
}


void MainFrame::SetupGameStore() 
{
    // Clearing old IDs and Game Store Box
    this->steam_id = -1;
    this->galaxy_id = "";
    this->settings->game_store->Set(std::vector<wxString>());

    GetSteamId(this->steam_id);
    bool steam_found = this->steam_id != -1 && fs::exists(LocalAppdata + "\\Katana_ZERO\\KatanaSpeedrunOpt" + std::to_string(this->steam_id) + ".zero");
    if (steam_found) {
        this->settings->game_store->Insert("Steam", 0);
    }

    GetGalaxyId(this->galaxy_id);
    bool galaxy_found = this->galaxy_id != "" && fs::exists(LocalAppdata + "\\Katana_ZERO\\KatanaSpeedrunOpt" + this->galaxy_id + ".zero");
    if (galaxy_found) {
        this->settings->game_store->Insert("GOG", 0);
    }

    if (!steam_found && !galaxy_found) {
        ShowErrorMessage("Katana Zero savefiles for your current Steam/GOG accounts is not found. Log-in the account with the game bought and launch the game.");
        this->Terminate();
    }
    else {
        this->settings->game_store->Select(0);
        this->UpdateGameStore();
    }
    this->settings->Layout();
}


// ----- Create funcs -----

void MainFrame::CreateSizer() 
{
    wxBoxSizer* sizer = this->CreateTableSizer();

    this->CreateSettingsSizer();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);

    main_sizer->Add(sizer, 0, wxEXPAND);
    main_sizer->Add(this->settings_sizer, 0, wxEXPAND);

    this->SetSizer(main_sizer);
    this->Layout();
    this->Fit();
}

wxBoxSizer* MainFrame::CreateTableSizer() 
{
    this->table = new Table(this, wxID_ANY);

    // --- Buttons sizer ---
    wxImage image = wxImage("images\\settings.png", wxBITMAP_TYPE_PNG).Scale(SETTINGS_IMAGE_SIZE, SETTINGS_IMAGE_SIZE, wxIMAGE_QUALITY_HIGH);

    wxButton* reload_times = new wxButton(this, IDs::Reload_Times, "Reload level times");
    wxBitmapButton* settings_image_button = new wxBitmapButton(this, IDs::Open_Settings, wxBitmap(image));
    settings_image_button->SetMinSize(settings_image_button->GetDefaultSize());

    wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);
    buttons->Add(reload_times, 1, wxEXPAND);
    buttons->Add(settings_image_button, 0, wxEXPAND);

    // --- Run description ---
    this->run_description = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    wxBoxSizer* rd_horizontal = new wxBoxSizer(wxHORIZONTAL);
    rd_horizontal->Add(this->run_description, 0, wxALIGN_CENTER_VERTICAL);

    this->run_description_sizer = new wxBoxSizer(wxVERTICAL);
    this->run_description_sizer->Add(rd_horizontal, 0, wxALIGN_CENTER_HORIZONTAL);

    // --- Main sizer ---
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    sizer->Add(this->table);
    sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    sizer->Add(this->run_description_sizer, 0, wxEXPAND);
    sizer->Add(buttons, 0, wxEXPAND);

    return sizer;
}


void MainFrame::CreateSettingsSizer() 
{
    this->settings = new Settings(this, wxID_ANY), 0, wxEXPAND;
    this->settings_sizer = new wxBoxSizer(wxHORIZONTAL);

    this->settings_sizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxEXPAND | wxALL, 5);
    this->settings_sizer->Add(this->settings, 0, wxEXPAND);
}


// ----- Get Funcs -----

void MainFrame::GetTimes() 
{
    this->level_times = time_reader::GetTimes(this->current_id);
}

float MainFrame::GetTotalTime() 
{
    int levels;
    float time = 0.0;

    if (this->table->category == Categories::ALL_STAGES) levels = 11;
    else if (this->table->category == Categories::GOLDEN_ENDING) levels = 7;
    else levels = 12;

    for (int i = 0; i < levels; i++) {
        float level_time = std::any_cast<float>(this->level_times[i][this->difficulty][this->run_type]["time"]);
        if (level_time != (float)-1) {
            time += level_time;
        }
    }
    return time;
}

int MainFrame::GetTotalRank() 
{
    int levels;
    int total_rank = 0;

    if (this->table->category == Categories::ALL_STAGES) levels = 11;
    else if (this->table->category == Categories::GOLDEN_ENDING) levels = 7;
    else levels = 12;

    for (int i = 0; i < levels; i++) {
        int level_rank = std::any_cast<int>(this->level_times[i][this->difficulty][this->run_type]["rank"]);
        if (level_rank == -1) {
            return 0;
        }
        else {
            total_rank += level_rank;
        }
    }
    return floor(total_rank / levels);
}

// ----- Updaters -----

void MainFrame::UpdateTimes()
{
    this->GetTimes();

    for (int i = 0; i < 12; i++) {
        LevelRow* level_row = this->table->level_rows[i];
        level_row->update_widgets(this->level_times[i][this->difficulty][this->run_type]);
    }
    this->UpdateTotalTime();
}

void MainFrame::UpdateTotalTime()
{
    this->table->TotalTime->SetLabel(
        CreateTimeString(this->GetTotalTime())
    );
    this->table->TotalMedal->SetBitmap(MedalBitmaps[this->GetTotalRank()]);
    this->table->total_time_sizer->Layout();
}

void MainFrame::UpdateRunDescription()
{
    this->run_description->SetLabel(
        this->settings->difficulty->GetValue() + ", " + this->settings->run_type->GetValue() + ", " + this->settings->category->GetValue()
    );
    this->run_description_sizer->Layout();
}

void MainFrame::UpdateCategory()
{
    int selection = this->settings->category->GetSelection();

    if (selection == Categories::GOLDEN_ENDING) {
        run_type = this->settings->run_type->GetSelection();

        // Golden Ending can't be achieved with Action Only activated, 
        // because of that Action Only option of Run Type Combobox is removed
        this->settings->run_type->Set(wxArrayString(2, GE_RUN_TYPE_OPTIONS));

        if (run_type == RunType::FULL_RUN) {
            this->settings->run_type->SetSelection(RunType::FULL_RUN - 1);
            this->run_type = RunType::FULL_RUN;
        }
        else {
            this->settings->run_type->SetSelection(RunType::NO_CUTSCENES - 1);
            this->run_type = RunType::NO_CUTSCENES;
        }
    }
    else {
        // Return Action Only option to Run Type combobox if category is not Golden Ending
        this->settings->run_type->Set(wxArrayString(3, RUN_TYPE_OPTIONS));
        this->run_type = RunType::ACTION_ONLY;
        this->settings->run_type->SetSelection(this->run_type);
    }
    this->table->ChangeCategory(this->settings->category->GetSelection());
}

void MainFrame::UpdateGameStore()
{
    wxString name = this->settings->game_store->GetValue();

    if (name == "Steam") {
        this->current_id = std::to_string(this->steam_id);
    }
    else if (name == "GOG") {
        this->current_id = this->galaxy_id;
    }
}


// ----- Backup saving/loading -----

void MainFrame::SaveBackupFile(wxCommandEvent& _event)
{
    bool result = CreateBackupDirectory();
    if (!result) {
        wxLogError("Can't create 'KZ SoB Backups' folder");
        return;
    }

    std::string filename = GetCurrentDatetimeString();

    // If file with this name exists, we need to add (number) to the filename
    if (fs::exists(BackupsFolder + filename)) {
        int i;
        for (i = 2; fs::exists(BackupsFolder + "\\" + filename + " (" + std::to_string(i) + ")"); i++) {}
        filename = filename + " (" + std::to_string(i) + ")";
    }

    wxFileDialog SaveDialog = wxFileDialog(
        this,
        "Save Sum of Best",
        BackupsFolder,
        filename,
        ".zero files|*.zero",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    // If user canceled file saving, quit the function
    if (SaveDialog.ShowModal() == wxID_CANCEL) return;

    std::ofstream output_stream(SaveDialog.GetPath().ToStdString());
    if (!output_stream.is_open())
    {
        wxLogError("Cannot save current contents in file '%s'.", SaveDialog.GetPath());
        return;
    }
    std::ifstream input_stream(LocalAppdata + std::string("\\Katana_ZERO\\KatanaSpeedrunOpt.zero"));
    if (!input_stream.is_open()) {
        wxLogError("Cannot save current contents in file '%s'.", SaveDialog.GetPath());
        return;
    }

    // stolen from here https://stackoverflow.com/questions/26302949/how-can-i-copy-a-text-file-into-another
    std::string content = "";
    int i;

    for (i = 0; input_stream.eof() != true; i++)
        content += input_stream.get();

    i--;
    content.erase(content.end() - 1);

    input_stream.close();

    output_stream << content;
    output_stream.close();
}


void MainFrame::LoadBackupFile(wxCommandEvent& _event)
{
    bool result = CreateBackupDirectory();
    if (!result) {
        wxLogError("Can't create 'KZ SoB Backups' folder");
        return;
    }

    wxFileDialog LoadDialog = wxFileDialog(
        this,
        "Load Sum of Best",
        BackupsFolder,
        "",
        ".zero files|*.zero",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );
    if (LoadDialog.ShowModal() == wxID_CANCEL) return;

    this->CreateTempBackup();

    std::string lines;
    {
        std::ifstream backup_file(LoadDialog.GetPath().ToStdString());
        if (!backup_file.is_open()) {
            wxLogError("Failed to open backup file");
            return;
        }

        std::string line;
        while (std::getline(backup_file, line)) {
            lines += line;
            lines.push_back('\n');
        }
        backup_file.close();
    }

    {
        std::ofstream speedrunopt_file(LocalAppdata + "\\Katana_ZERO\\KatanaSpeedrunOpt" + this->current_id + ".zero");
        if (!speedrunopt_file.is_open()) {
            wxLogError("Failed to open KatanaSpeedrunOpt" + wxString(this->current_id) + ".zero file");
            return;
        }
        speedrunopt_file << lines;
        speedrunopt_file.close();
    }

    {
        std::ofstream speedrunopt_file(LocalAppdata + "\\Katana_ZERO\\KatanaSpeedrunOpt.zero");
        if (!speedrunopt_file.is_open()) {
            wxLogError("Failed to open KatanaSpeedrunOpt.zero file");
            return;
        }
        speedrunopt_file << lines;
        speedrunopt_file.close();
    }

    try {
        this->UpdateTimes();
    }
    catch (...) {
        ShowErrorMessage("Backup corrupted or invalid file selected. Reverting back.");
        this->RevertTempBackup();
    }
}


// ----- Temporary Backup functions -----

void MainFrame::CreateTempBackup() 
{
    std::string lines;
    {
        std::ifstream speedrunopt_file(LocalAppdata + "\\Katana_ZERO\\KatanaSpeedrunOpt" + this->current_id + ".zero");
        if (!speedrunopt_file.is_open()) {
            wxLogError("Failed to open KatanaSpeedrunOpt" + wxString(this->current_id) + ".zero file");
            return;
        }

        std::string line;
        while (std::getline(speedrunopt_file, line)) {
            lines += line;
            lines.push_back('\n');
        }
        speedrunopt_file.close();
    }

    {
        std::ofstream speedrunopt_file(BackupsFolder + "\\TempBackup.zero");
        if (!speedrunopt_file.is_open()) {
            wxLogError("Failed to open KatanaSpeedrunOpt" + wxString(this->current_id) + ".zero file");
            return;
        }
        speedrunopt_file << lines;
        speedrunopt_file.close();
    }
}

void MainFrame::RevertTempBackup() 
{
    std::string lines;
    {
        std::ifstream speedrunopt_file(BackupsFolder + "\\TempBackup.zero");
        if (!speedrunopt_file.is_open()) {
            wxLogError("Failed to open KatanaSpeedrunOpt" + wxString(this->current_id) + ".zero file");
            return;
        }

        std::string line;
        while (std::getline(speedrunopt_file, line)) {
            lines += line;
            lines.push_back('\n');
        }
        speedrunopt_file.close();
    }

    {
        std::ofstream speedrunopt_file(LocalAppdata + "\\Katana_ZERO\\KatanaSpeedrunOpt" + this->current_id + ".zero");
        if (!speedrunopt_file.is_open()) {
            wxLogError("Failed to open KatanaSpeedrunOpt" + wxString(this->current_id) + ".zero file");
            return;
        }
        speedrunopt_file << lines;
        speedrunopt_file.close();
    }
}


// ----- Events funcs -----

void MainFrame::ShowSettings(wxCommandEvent& _event)
{
    this->is_settings_shown = !this->is_settings_shown;
    this->settings_sizer->ShowItems(this->is_settings_shown);
    this->settings_sizer->Layout();
    this->settings_sizer->Fit(this);
    this->Layout();
    this->Fit();
    _event.Skip();
}

void MainFrame::UpdateTimesEvent(wxCommandEvent& _event) {
    this->UpdateTimes();
    _event.Skip();
}

void MainFrame::UpdateDifficultyEvent(wxCommandEvent& _event)
{
    this->difficulty = this->settings->difficulty->GetSelection();
    this->UpdateTimes();
    this->UpdateRunDescription();
    _event.Skip();
}

void MainFrame::UpdateRunTypeEvent(wxCommandEvent& _event)
{
    this->run_type = this->settings->run_type->GetSelection();
    this->UpdateTimes();
    this->UpdateRunDescription();
    _event.Skip();
}

void MainFrame::UpdateCategoryEvent(wxCommandEvent& _event)
{
    this->UpdateCategory();
    this->UpdateTimes();
    this->UpdateRunDescription();
    _event.Skip();
}

void MainFrame::SaveSettingsEvent(wxCommandEvent& _event)
{
    this->SaveSettings();
    _event.Skip();
}

void MainFrame::UpdateGameStoreEvent(wxCommandEvent& _event)
{
    this->UpdateGameStore();
    this->UpdateTimes();
    this->UpdateRunDescription();
    _event.Skip();
}

