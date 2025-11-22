#include "auth.h"

#include <fstream>
#include <sstream>

AuthService::AuthService(const std::string &csvPath)
    : m_csvPath(csvPath)
{
}

bool AuthService::login(const std::string &username,
                        const std::string &password) const
{
    std::ifstream file(m_csvPath);
    if (!file.is_open())
    {
        // No DB file yet -> no users
        return false;
    }

    std::string line;
    bool first = true;

    while (std::getline(file, line))
    {
        if (first)
        {
            // skip header: "Username,Password"
            first = false;
            continue;
        }

        std::stringstream ss(line);
        std::string fileUser, filePass;

        std::getline(ss, fileUser, ',');
        std::getline(ss, filePass, ',');

        if (fileUser == username && filePass == password)
            return true;
    }

    return false;
}

bool AuthService::userExists(const std::string &username) const
{
    std::ifstream file(m_csvPath);
    if (!file.is_open())
    {
        // If file doesn't exist, there are no users
        return false;
    }

    std::string line;
    bool first = true;

    while (std::getline(file, line))
    {
        if (first)
        {
            first = false;   // skip header
            continue;
        }

        std::stringstream ss(line);
        std::string fileUser, filePass;

        std::getline(ss, fileUser, ',');
        std::getline(ss, filePass, ',');

        if (fileUser == username)
            return true;
    }

    return false;
}

bool AuthService::createAccount(const std::string &username,
                                const std::string &password)
{
    // Don't allow duplicates
    if (userExists(username))
        return false;

    // Check if file already exists to know whether to write header
    bool existed = false;
    {
        std::ifstream check(m_csvPath);
        existed = check.good();
    }

    std::ofstream out(m_csvPath, std::ios::app);
    if (!out.is_open())
    {
        return false;
    }

    if (!existed)
    {
        // Row 0 header
        out << "Username,Password\n";
    }

    // Row 1+ = users
    out << username << "," << password << "\n";
    return true;
}
