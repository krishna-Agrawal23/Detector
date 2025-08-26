#include "scanner.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>

extern "C" {
    #include "sha256.h"
}

Scanner::Scanner(const std::string& signature_db_path) {
    loadSignatures(signature_db_path);
}

void Scanner::loadSignatures(const std::string& signature_db_path) {
    std::ifstream db_file(signature_db_path);
    if (!db_file) {
        throw std::runtime_error("Could not open signature database file: " + signature_db_path);
    }
    std::string line;
    while (std::getline(db_file, line)) {
        if (!line.empty()) {
            malware_signatures.insert(line);
        }
    }
}

std::string Scanner::calculateFileHash(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        return "";
    }
    const size_t buffer_size = 4096;
    std::vector<char> buffer(buffer_size);
    SHA256_CTX ctx;
    sha256_init(&ctx);
    while (file.good()) {
        file.read(buffer.data(), buffer_size);
        std::streamsize bytes_read = file.gcount();
        if (bytes_read > 0) {
            sha256_update(&ctx, reinterpret_cast<const BYTE*>(buffer.data()), bytes_read);
        }
    }
    BYTE hash[SHA256_BLOCK_SIZE];
    sha256_final(&ctx, hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_BLOCK_SIZE; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

bool Scanner::isMalware(const std::string& file_path) {
    std::string file_hash = calculateFileHash(file_path);
    if (file_hash.empty()) {
        return false;
    }
    std::cout << "Scanning " << file_path << " [HASH: " << file_hash << "]" << std::endl;
    if (malware_signatures.count(file_hash)) {
        return true;
    }
    return false;
}