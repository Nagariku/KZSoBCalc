#include "utils.h"


bool utils::GetKnownFolderWrapper(std::string& out, REFKNOWNFOLDERID rfid)
{
    /* Shoutouts to nik for making this func for me :) */
    // тут будет вайдовая строка.
    PWSTR dir = nullptr;

    // ас пер... эм эс дэ эн https://docs.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath
    HRESULT hR = SHGetKnownFolderPath(rfid, KF_FLAG_DEFAULT, nullptr, &dir);

    if (FAILED(hR))
    {
        CoTaskMemFree(dir);
        return false;
    }
    else
    {
        // окей, но надо вайдовую строку перевести в multibyte UTF-8
        int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, dir, -1, nullptr, 0, nullptr, nullptr);
        LPSTR dirMB = new CHAR[size];
        size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, dir, -1, dirMB, size, nullptr, nullptr);
        CoTaskMemFree(dir);

        // получилось ли преобразовать строку? если да, то скопировать её в out.
        if (size > 0)
        {
            out = dirMB;
        }

        // выгрузить нашу временную перведённую строку.
        delete[] dirMB;
        dirMB = nullptr;

        return size > 0;
    }
}


std::string utils::GetLocalAppdata() {
    std::string localappdata;
    bool result = GetKnownFolderWrapper(localappdata, FOLDERID_LocalAppData);

    if (!result) {
        throw;
    }
    return localappdata;
}


wxBoxSizer* utils::CenterControl(wxWindow* control, wxBoxSizer* sizer) {
    int orientation, alignment;
    if (sizer->GetOrientation() == wxHORIZONTAL) {
        orientation = wxVERTICAL; alignment = wxALIGN_CENTER_HORIZONTAL;
    }
    else {
        orientation = wxHORIZONTAL; alignment = wxALIGN_CENTER_VERTICAL;
    }

    wxBoxSizer* center_sizer = new wxBoxSizer(orientation);
    center_sizer->Add(control, 1, alignment);

    return center_sizer;
}

std::vector<wxBitmap> utils::GetMedalBitmaps()
{
    const int MEDAL_IMAGE_HEIGHT = 25;
    const std::string medal_names[5] = { "none", "bronze", "silver", "gold", "platinum" };

    std::vector<wxBitmap> MedalBitmaps;

    for (int i = 0; i < 5; i++) {
        wxImage image = wxImage(wxString("images\\medals\\" + medal_names[i] + ".png"), wxBITMAP_TYPE_PNG);

        float proportion = (float)image.GetWidth() / (float)image.GetHeight();
        image = image.Scale(MEDAL_IMAGE_HEIGHT * proportion, MEDAL_IMAGE_HEIGHT, wxIMAGE_QUALITY_HIGH);
        MedalBitmaps.push_back(wxBitmap(image));
    }

    return MedalBitmaps;
}

std::string utils::CreateTimeString(float timedelta)
{
    std::string hours =        std::to_string(int(floor(timedelta / 3600)));
    std::string minutes =      std::to_string(int(int(floor(timedelta / 60)) % 60));
    std::string seconds =      std::to_string(int(floor(fmod(timedelta, 60))));
    std::string milliseconds = std::to_string(int(round(fmod(timedelta, 1) * 100)));

    std::string time_string = zfill(seconds, 2) + "." + zfill(milliseconds, 2);
    if (hours != "0") time_string = zfill(hours, 2) + ":" + zfill(minutes, 2) + ":" + time_string;
    else if (minutes != "0") time_string = zfill(minutes, 2) + ":" + time_string;

    return time_string;
}

std::string utils::zfill(std::string _string, int leading_zeros) {
    std::ostringstream sstream;
    sstream << std::setw(leading_zeros) << std::setfill('0') << _string;

    return sstream.str();
}


wxColour utils::GetColor(std::string color) {
    std::map<std::string, wxColour> colors = {
        {"Gray", wxColour(155, 155, 155)},
        {"White", wxColour(255, 255, 255)},
        {"Black", wxColour(0, 0, 0)}
    };
    return colors[color];
}

std::string utils::GetCurrentDatetimeString() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%d-%m-%y_%H-%M-%S", timeinfo);
    std::string str(buffer, 80);
    return str;
}

bool utils::CreateBackupDirectory() {
    std::string local_appdata = utils::GetLocalAppdata();
    if (!fs::exists(local_appdata + "\\Katana_ZERO\\KZ SoB Backups")) {
        return fs::create_directory(local_appdata + "\\Katana_ZERO\\KZ SoB Backups");
    }
    return true;
}

bool utils::GetSteamId(int& out)
{
    DWORD dwType = REG_DWORD;
    DWORD dwValue;
    DWORD dwSize = sizeof(dwValue);

    LSTATUS lsRet = RegGetValueA(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\Steam\\ActiveProcess", "ActiveUser", RRF_RT_REG_DWORD, &dwType, &dwValue, &dwSize);

    if (lsRet == ERROR_SUCCESS) {
        out = dwValue;
        return true;
    }
    else {
        return false;
    }
}

bool utils::GetGalaxyId(std::string& out)
{
    #define SUBKEY L"SOFTWARE\\GOG.com\\Galaxy\\settings"
    #define VALUE L"userId"
    
    DWORD dwType = REG_SZ;
    DWORD dwSize = 0;

    LSTATUS lsRet = RegGetValueW(HKEY_CURRENT_USER, SUBKEY, VALUE, RRF_RT_REG_SZ, &dwType, nullptr, &dwSize);
    if (lsRet != ERROR_SUCCESS) {
        return false;
    }

    DWORD dwSizeInWCHARs = dwSize / sizeof(WCHAR);
    LPWSTR str = new WCHAR[dwSizeInWCHARs];

    lsRet = RegGetValueW(HKEY_CURRENT_USER, SUBKEY, VALUE, RRF_RT_REG_SZ, &dwType, str, &dwSize);
    if (lsRet != ERROR_SUCCESS) {
        delete[] str;
        return false;
    }
    else
    {
        int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str, -1, nullptr, 0, nullptr, nullptr);
        if (size <= 0) {
            delete[] str;
            return false;
        }

        LPSTR strMB = new CHAR[size];
        size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str, -1, strMB, size, nullptr, nullptr);
        delete[] str;
        str = nullptr;

        if (size <= 0) {
            delete[] strMB;
            return false;
        }

        out = strMB;
        delete[] strMB;
        strMB = nullptr;

        return true;
    }
}

void utils::ShowErrorMessage(std::string message) {
    wxMessageDialog* error = new wxMessageDialog(
        NULL,
        message,
        "Error accured!",
        wxICON_ERROR
    );
    error->ShowModal();
}