#ifndef SPOOFER_H
#define SPOOFER_H

#include <string>
#include <vector>

class Spoofer {
public:
    Spoofer();
    ~Spoofer();

    bool spoofHwid();
    bool restoreHwid();
    std::string getCurrentHwid() const;
    std::vector<std::string> getBackupList() const;

private:
    std::vector<std::string> registryKeys;
    std::vector<std::string> backupValues;
    bool backupRegistry();
    bool modifyRegistry();
    bool restoreFromBackup();
};

#endif // SPOOFER_H