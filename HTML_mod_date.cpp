// HTML_mod_date.cpp 
//
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <shlwapi.h>

#pragma warning(disable : 4996)

namespace fs = std::filesystem;

void updateHtmlFile(const std::string& filePath) {
    // Read the modification date of the file
    std::string str_modifiedTime;
    auto lastWriteTime = fs::last_write_time(filePath);
    const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(lastWriteTime);
    const auto modifiedTime = std::chrono::system_clock::to_time_t(systemTime);
    str_modifiedTime = std::ctime(&modifiedTime);

    // Open the file for editing
    std::ifstream inputFile(filePath);
    std::string fileContents;
    std::string line;


    if (std::getline(inputFile, line)) {
        // Add the modification date to the beginning of the first line in the comment
        
        fileContents += "<!--Modified on " + str_modifiedTime + " " + line + "-->\n\n";
    }

    while (std::getline(inputFile, line)) {
        fileContents += line + "\n";
    }

    inputFile.close();

    // Save the updated content back to the file
    std::ofstream outputFile(filePath);
    outputFile << fileContents;
    outputFile.close();
}

//Get the current folder path
std::string getPath() {
    std::string otherprogram = "Failed to get path";

    DWORD szPath;
    char selfdir[MAX_PATH] = { 0 };
    szPath = GetModuleFileNameA(NULL, selfdir, MAX_PATH);
    if (szPath != 0) // successfully got path of current program
    {
        // helper string to make life much, much easier
        std::string helper = selfdir;
        //find last backslash in current program path
        size_t pos = helper.find_last_of("\\");

        if (pos != std::string::npos) // found last backslash
        {
            // remove everything after last backslash. This should remove 
            // the current program's name.
            otherprogram = helper.substr(0, pos + 1);
        }
    }
    return otherprogram;
}



int main() {
    //Getting an .exe file path
    const std::string folderPath = getPath();
    std::cout << folderPath;

    //updated files counter
    int files_cnt = 0;

    if (folderPath != "Failed to get path")     
    for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            const std::string extension = entry.path().extension().string();
            if (extension == ".htm" || extension == ".html") {
                updateHtmlFile(entry.path().string());
                std::cout << "Updated file: " << entry.path() << std::endl;
                files_cnt++;
            }
        }
    }
    std::cout << "Updated " << files_cnt << " files.\n";
    std::cin.get();
    std::cin.get();
    //return 0;
}