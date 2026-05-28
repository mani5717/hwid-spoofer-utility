#include "hwid_reader.h"
#include "registry_utils.h"
#include <sstream>
#include <iomanip>

std::string HwidReader::readHwid() {
    std::string hwid;
    std::string hdd = readHddSerial();
    std::string vol = readVolumeId();
    hwid = hdd + "-" + vol;
    return hwid;
}

std::string HwidReader::readHddSerial() {
    std::string serial;
    RegistryUtils::readRegistryKey(
        "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0",
        "SerialNumber",
        serial
    );
    return serial.empty() ? "00000000" : serial;
}

std::string HwidReader::readVolumeId() {
    std::string volumeId;
    RegistryUtils::readRegistryKey(
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
        "ProductId",
        volumeId
    );
    return volumeId.empty() ? "00000-00000-00000-00000" : volumeId;
}