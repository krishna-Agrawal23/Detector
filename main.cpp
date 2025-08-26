#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>

#include "monitor.h"
#include "scanner.h"

int main() {
    std::string pathToWatch = "A:\\Downloads\\Chrome";

    try {
        auto scanner = std::make_shared<Scanner>("signatures.db");
        Monitor monitor(pathToWatch, scanner);

        std::cout << "Starting file monitor on directory: " << pathToWatch << std::endl;
        std::cout << "Press Ctrl+C to stop the monitor." << std::endl;
        
        monitor.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}