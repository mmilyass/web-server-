#include "../../includes/Webserv.hpp"

std::string ConfigFileReader::read(const std::string &path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
        throw std::runtime_error("cannot open this file " + path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string buildDirectoryListing(const std::string &directoryPath)
{
    DIR *dir;
    struct dirent *entry;
    std::ostringstream ss;

    dir = opendir(directoryPath.c_str());
    if (!dir)
    {
        return "rah_had_Directory_makaynch.html";
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
            continue;

        ss << entry->d_name << "\n";
    }

    closedir(dir);
    return ss.str();
}
