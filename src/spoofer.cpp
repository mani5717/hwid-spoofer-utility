#include "spoofer.h"
#include "hwid_reader.h"
#include "registry_utils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Spoofer::Spoofer() {
    registryKeys = {
        "HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS",
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography"
    };
    srand(static_cast<unsigned>(time(nullptr)));
}

Spoofer::~Spoofer() {}

bool Spoofer::spoofHwid() {
    if (!backupRegistry()) {
        std::cerr << "Backup failed!" << std::endl;
        return false;
    }
    if (!modifyRegistry()) {
        std::cerr << "Modify failed!" << std::endl;
        return false;
    }
    return true;
}

bool Spoofer::restoreHwid() {
    return restoreFromBackup();
}

std::string Spoofer::getCurrentHwid() const {
    return HwidReader::readHwid();
}

std::vector<std::string> Spoofer::getBackupList() const {
    return backupValues;
}

bool Spoofer::backupRegistry() {
    backupValues.clear();
    for (const auto& key : registryKeys) {
        std::string value;
        if (RegistryUtils::readRegistryKey(key, "SystemProductName", value)) {
            backupValues.push_back(value);
        } else {
            backupValues.push_back("");
        }
    }
    return true;
}

bool Spoofer::modifyRegistry() {
    for (const auto& key : registryKeys) {
        std::string fakeValue = "SPOOFED-" + std::to_string(rand() % 10000);
        if (!RegistryUtils::writeRegistryKey(key, "SystemProductName", fakeValue)) {
            return false;
        }
    }
    return true;
}

bool Spoofer::restoreFromBackup() {
    size_t i = 0;
    for (const auto& key : registryKeys) {
        if (i < backupValues.size() && !backupValues[i].empty()) {
            RegistryUtils::writeRegistryKey(key, "SystemProductName", backupValues[i]);
        }
        i++;
    }
    return true;
}