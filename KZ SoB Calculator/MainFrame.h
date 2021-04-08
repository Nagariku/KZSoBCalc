#pragma once

#include <filesystem>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wfstream.h>

#include "Table.h"
#include "Settings.h"

#include "time_reader.h"
#include "globals.h"
#include "json.h"

namespace fs = std::filesystem;


class MainFrame : public wxFrame
{
    public:
        MainFrame();

        bool ternimated = false;

    private:
        // ----- Widgets -----
        Table* table;
        Settings* settings;

        wxBoxSizer* settings_sizer;
        wxBoxSizer* run_description_sizer;

        wxStaticText* run_description;

        // ----- Variables -----
        int difficulty, run_type;
        bool is_settings_shown;

        std::vector<Level> level_times;

        int steam_id;
        std::string galaxy_id;
        std::string current_id;

        // ----- Functions -----
        void PreSizer();
        void PostSizer();

        // --- Misc functions ---
        void Terminate();
        void SaveSettings();

        // --- Setup functions ---
        void SetupIcon();
        void SetupSettings();
        void SetupGameStore();

        // --- Create functions ---
        void CreateSizer();
        wxBoxSizer* CreateTableSizer();
        void CreateSettingsSizer();

        // Value get
        void GetTimes();
        float GetTotalTime();
        int GetTotalRank();

        // Updators
        void UpdateTimes();
        void UpdateTotalTime();
        void UpdateRunDescription();

        void UpdateCategory();
        void UpdateGameStore();

        // Events
        void ShowSettings(wxCommandEvent& _event);
        void UpdateTimesEvent(wxCommandEvent& _event);
        void UpdateDifficultyEvent(wxCommandEvent& _event);
        void UpdateRunTypeEvent(wxCommandEvent& _event);
        void UpdateCategoryEvent(wxCommandEvent& _event);
        void SaveSettingsEvent(wxCommandEvent& _event);
        void UpdateGameStoreEvent(wxCommandEvent& _event);

        // Backup saving/loading
        void SaveBackupFile(wxCommandEvent& _event);
        void LoadBackupFile(wxCommandEvent& _event);

        void CreateTempBackup();
        void RevertTempBackup();

        wxDECLARE_EVENT_TABLE();
};

