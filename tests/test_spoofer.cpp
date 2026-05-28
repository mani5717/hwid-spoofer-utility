#include <iostream>
#include <cassert>
#include "../include/spoofer.h"
#include "../include/hwid_reader.h"

void testHwidReader() {
    std::string hwid = HwidReader::readHwid();
    assert(!hwid.empty());
    std::cout << "testHwidReader passed: " << hwid << std::endl;
}

void testSpooferBackupRestore() {
    Spoofer spoofer;
    std::string originalHwid = spoofer.getCurrentHwid();
    std::cout << "Original HWID: " << originalHwid << std::endl;

    bool spoofResult = spoofer.spoofHwid();
    assert(spoofResult);
    std::string spoofedHwid = spoofer.getCurrentHwid();
    std::cout << "Spoofed HWID: " << spoofedHwid << std::endl;
    assert(spoofedHwid != originalHwid);

    bool restoreResult = spoofer.restoreHwid();
    assert(restoreResult);
    std::string restoredHwid = spoofer.getCurrentHwid();
    std::cout << "Restored HWID: " << restoredHwid << std::endl;
    assert(restoredHwid == originalHwid);

    std::cout << "testSpooferBackupRestore passed!" << std::endl;
}

int main() {
    std::cout << "Running HWID Spoofer tests..." << std::endl;
    testHwidReader();
    testSpooferBackupRestore();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}