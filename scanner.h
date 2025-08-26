#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <unordered_set>

class Scanner {
public:
    Scanner(const std::string& signature_db_path);
    bool isMalware(const std::string& file_path);

private:
    std::unordered_set<std::string> malware_signatures;
    void loadSignatures(const std::string& signature_db_path);
    std::string calculateFileHash(const std::string& file_path);
};

#endif