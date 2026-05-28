#ifndef HWID_READER_H
#define HWID_READER_H

#include <string>

class HwidReader {
public:
    static std::string readHwid();
    static std::string readHddSerial();
    static std::string readVolumeId();
};

#endif // HWID_READER_H