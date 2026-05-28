#ifndef REGISTRY_UTILS_H
#define REGISTRY_UTILS_H

#include <string>
#include <vector>

namespace RegistryUtils {
    bool readRegistryKey(const std::string& path, const std::string& valueName, std::string& outValue);
    bool writeRegistryKey(const std::string& path, const std::string& valueName, const std::string& value);
    bool deleteRegistryValue(const std::string& path, const std::string& valueName);
    std::vector<std::string> enumerateKeys(const std::string& path);
}

#endif // REGISTRY_UTILS_H