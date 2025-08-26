#ifndef MONITOR_H
#define MONITOR_H

#include <string>
#include <memory>
#include "scanner.h"

class Monitor {
public:
    Monitor(const std::string& path, std::shared_ptr<Scanner> scanner);
    void start();

private:
    std::string path_to_watch;
    std::shared_ptr<Scanner> file_scanner;
};

#endif