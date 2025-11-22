#ifndef AUTH_H
#define AUTH_H

#include <string>

class AuthService
{
public:
    // csvPath defaults to "db.csv" in the working directory
    explicit AuthService(const std::string &csvPath = "db.csv");

    // Returns true if (username, password) pair exists in CSV
    bool login(const std::string &username,
               const std::string &password) const;

    // Returns true if username already exists (any password)
    bool userExists(const std::string &username) const;

    // Creates a new account if username doesn't exist;
    // returns true on success, false if username already exists
    bool createAccount(const std::string &username,
                       const std::string &password);

private:
    std::string m_csvPath;
};

#endif // AUTH_H
