#include <stdio.h>
#include <windows.h>
#include <string>
//*************************************************************
//
//  main()
//
//  Purpose:    Modify a registry key value.
//
//  Parameters: hKeyRoot    -   Root key
//              lpSubKey    -   SubKey to delete
//
//  Return:     TRUE if successful.
//              FALSE if an error occurs.
//
//*************************************************************

bool RegModKey_Dword(HKEY hKeyRoot, const char *lpSubKey, const char *lpValueName) {
    HKEY hTestKey;
    if (RegOpenKeyEx(hKeyRoot, TEXT(lpSubKey), 0, KEY_READ, &hTestKey) !=
        ERROR_SUCCESS) {
        printf("Unable to open %s\n", lpSubKey);
        return false;
    }
    printf("Was able to open %s\n", lpSubKey);
    int32_t dword_number = 0;
    // Restrict type to REG_DWORD.

    if (RegGetValue(hKeyRoot, TEXT(lpSubKey), TEXT(lpValueName), RRF_RT_ANY, NULL,
                    NULL, NULL) != ERROR_SUCCESS) {
        printf("Unable to read value %s\n", lpValueName);
        return false;
    }
    printf("Was able to read: %s = %d\n", lpValueName, dword_number);
    int32_t new_value = 1;
    std::string name = std::string(lpSubKey) + "\\" + std::string(lpValueName);
    printf("Full name: %s\n", name.c_str());
    LONG ret = RegSetValueEx(hKeyRoot, name.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE *>(1), NULL);
//    LONG ret = RegSetValueEx(hKeyRoot, name.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE *>(&new_value), sizeof(new_value));
    //RegSetValue(hKeyRoot, lpSubKey, REG_DWORD, lpValueName, 1) // for 16 bit, deprecated
    if (ret != ERROR_SUCCESS) {
        printf("FAIL!?");
        return false;
    }
    printf("SUCCESS!?");

    RegFlushKey(hTestKey);
    return true;
}

int main() {
    HKEY hKeyRoot = HKEY_LOCAL_MACHINE;
    const char *lpSubKey =
            R"(SOFTWARE\Policies\Microsoft\Windows\Network Connections)";
    const char *lpValueName = R"(NC_PersonalFirewallConfig)";
//  const char *lpValueName = R"(NC_ShowSharedAccessUI)";
    printf("Monitoring HKEY_LOCAL_MACHINE: %s\n", lpSubKey);
    bool bSuccess = RegModKey_Dword(hKeyRoot, lpSubKey, lpValueName);
    if (bSuccess) {
        printf("Success!\n");
    } else {
        printf("Failure.\n");
    }
    return 0;
}
