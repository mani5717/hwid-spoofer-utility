#include <iostream>
#include <cstdlib>
#include "spoofer.h"

int main() {
    std::cout << "HWID Spoofer Utility v1.0.0" << std::endl;
    std::cout << "============================" << std::endl;

    Spoofer spoofer;
    std::cout << "Current HWID: " << spoofer.getCurrentHwid() << std::endl;

    std::cout << "Spoofing HWID..." << std::endl;
    if (spoofer.spoofHwid()) {
        std::cout << "Spoof successful!" << std::endl;
        std::cout << "New HWID: " << spoofer.getCurrentHwid() << std::endl;
    } else {
        std::cerr << "Spoof failed!" << std::endl;
        return 1;
    }

    std::cout << "Restoring original HWID..." << std::endl;
    if (spoofer.restoreHwid()) {
        std::cout << "Restore successful!" << std::endl;
    } else {
        std::cerr << "Restore failed!" << std::endl;
        return 1;
    }

    return 0;
}