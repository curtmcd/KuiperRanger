#include <windows.h>
#include <shlobj.h>
#include <filesystem>
#include <fstream>
#include <string>

#include "persist.hpp"

// This file is for Windows systems only
namespace fs = std::filesystem;

//Helper to retrieve the shared ProgramData path and append the game folder.
static fs::path GetTargetFilePath() {
    PWSTR path_tmp = nullptr;
    fs::path scorePath;

    // FOLDERID_ProgramData typically points to C:\ProgramData
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &path_tmp))) {
        scorePath = fs::path(path_tmp) / "kuiper-ranger" / "scores";
        CoTaskMemFree(path_tmp);
    }

    return scorePath;
}

bool Persist::load(std::string &buf) {
    fs::path path = GetTargetFilePath();

    if (path.empty() || !fs::exists(path))
        return false;

    // Open in binary mode to prevent unexpected newline conversions
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
        return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buf.resize(static_cast<size_t>(size));
    if (file.read(&buf[0], size))
        return true;

    return false;
}

bool Persist::save(const std::string& buf) {
    fs::path path = GetTargetFilePath();

    if (path.empty())
        return false;

    try {
        // Ensure the subdirectory exists (e.g., C:\ProgramData\kuiper-ranger)
        fs::path parentDir = path.parent_path();
        if (!fs::exists(parentDir))
            fs::create_directories(parentDir);

        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file.is_open())
            return false;

        file.write(buf.data(), buf.size());
        file.close();

        return !file.fail();
    }
    catch (const fs::filesystem_error&) {
        return false;
    }
}
