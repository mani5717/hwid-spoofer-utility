#include "registry_utils.h"
#ifdef _WIN32
#include <windows.h>
#include <winreg.h>
#endif
#include <sstream>

namespace RegistryUtils {

bool readRegistryKey(const std::string& path, const std::string& valueName, std::string& outValue) {
#ifdef _WIN32
    HKEY hKey;
    std::string root;
    std::string subKey;

    size_t pos = path.find('\\');
    if (pos != std::string::npos) {
        root = path.substr(0, pos);
        subKey = path.substr(pos + 1);
    } else {
        return false;
    }

    HKEY hRootKey = nullptr;
    if (root == "HKEY_LOCAL_MACHINE") hRootKey = HKEY_LOCAL_MACHINE;
    else if (root == "HKEY_CURRENT_USER") hRootKey = HKEY_CURRENT_USER;
    else return false;

    if (RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    char buffer[1024];
    DWORD bufferSize = sizeof(buffer);
    DWORD type;
    if (RegQueryValueExA(hKey, valueName.c_str(), nullptr, &type, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
        outValue = std::string(buffer, bufferSize - 1);
        RegCloseKey(hKey);
        return true;
    }

    RegCloseKey(hKey);
#endif
    return false;
}

bool writeRegistryKey(const std::string& path, const std::string& valueName, const std::string& value) {
#ifdef _WIN32
    HKEY hKey;
    std::string root;
    std::string subKey;

    size_t pos = path.find('\\');
    if (pos != std::string::npos) {
        root = path.substr(0, pos);
        subKey = path.substr(pos + 1);
    } else {
        return false;
    }

    HKEY hRootKey = nullptr;
    if (root == "HKEY_LOCAL_MACHINE") hRootKey = HKEY_LOCAL_MACHINE;
    else if (root == "HKEY_CURRENT_USER") hRootKey = HKEY_CURRENT_USER;
    else return false;

    if (RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    if (RegSetValueExA(hKey, valueName.c_str(), 0, REG_SZ, (const BYTE*)value.c_str(), value.size() + 1) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return true;
    }

    RegCloseKey(hKey);
#endif
    return false;
}

bool deleteRegistryValue(const std::string& path, const std::string& valueName) {
#ifdef _WIN32
    HKEY hKey;
    std::string root;
    std::string subKey;

    size_t pos = path.find('\\');
    if (pos != std::string::npos) {
        root = path.substr(0, pos);
        subKey = path.substr(pos + 1);
    } else {
        return false;
    }

    HKEY hRootKey = nullptr;
    if (root == "HKEY_LOCAL_MACHINE") hRootKey = HKEY_LOCAL_MACHINE;
    else if (root == "HKEY_CURRENT_USER") hRootKey = HKEY_CURRENT_USER;
    else return false;

    if (RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    if (RegDeleteValueA(hKey, valueName.c_str()) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return true;
    }

    RegCloseKey(hKey);
#endif
    return false;
}

std::vector<std::string> enumerateKeys(const std::string& path) {
    std::vector<std::string> keys;
#ifdef _WIN32
    HKEY hKey;
    std::string root;
    std::string subKey;

    size_t pos = path.find('\\');
    if (pos != std::string::npos) {
        root = path.substr(0, pos);
        subKey = path.substr(pos + 1);
    } else {
        return keys;
    }

    HKEY hRootKey = nullptr;
    if (root == "HKEY_LOCAL_MACHINE") hRootKey = HKEY_LOCAL_MACHINE;
    else if (root == "HKEY_CURRENT_USER") hRootKey = HKEY_CURRENT_USER;
    else return keys;

    if (RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_ENUMERATE_SUB_KEYS, &hKey) != ERROR_SUCCESS) {
        return keys;
    }

    char keyName[256];
    DWORD keyNameSize;
    DWORD index = 0;
    while (RegEnumKeyExA(hKey, index, keyName, &keyNameSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
        keys.push_back(std::string(keyName, keyNameSize));
        keyNameSize = sizeof(keyName);
        index++;
    }

    RegCloseKey(hKey);
#endif
    return keys;
}

} // namespace RegistryUtils