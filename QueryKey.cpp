//
// Created by jason on 1/22/2018.
//

// QueryKey - Enumerates the subkeys of key and its associated values.
//     hKey - Key whose subkeys and values are to be enumerated.

#include <stdio.h>
#include <windows.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383m

void QueryKey(HKEY hKey) {
  TCHAR achKey[MAX_KEY_LENGTH];        // buffer for subkey name
  DWORD cbName;                        // size of name string
  TCHAR achClass[MAX_PATH] = TEXT(""); // buffer for class name
  DWORD cchClassName = MAX_PATH;       // size of class string
  DWORD cSubKeys = 0;                  // number of subkeys
  DWORD cbMaxSubKey;                   // longest subkey size
  DWORD cchMaxClass;                   // longest class string
  DWORD cValues;                       // number of values for key
  DWORD cchMaxValue;                   // longest value name
  DWORD cbMaxValueData;                // longest value data
  DWORD cbSecurityDescriptor;          // size of security descriptor
  FILETIME ftLastWriteTime;            // last write time

  DWORD i, retCode;

  TCHAR achValue[MAX_VALUE_NAME];
  DWORD cchValue = MAX_VALUE_NAME;

  // Get the class name and the value count.
  retCode = RegQueryInfoKey(hKey,            // key handle
                            achClass,        // buffer for class name
                            &cchClassName,   // size of class string
                            NULL,            // reserved
                            &cSubKeys,       // number of subkeys
                            &cbMaxSubKey,    // longest subkey size
                            &cchMaxClass,    // longest class string
                            &cValues,        // number of values for this key
                            &cchMaxValue,    // longest value name
                            &cbMaxValueData, // longest value data
                            &cbSecurityDescriptor, // security descriptor
                            &ftLastWriteTime);     // last write time

  // Enumerate the subkeys, until RegEnumKeyEx fails.

  if (cSubKeys) {
    printf("\nNumber of subkeys: %d\n", cSubKeys);

    for (i = 0; i < cSubKeys; i++) {
      cbName = MAX_KEY_LENGTH;
      retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL,
                             &ftLastWriteTime);
      if (retCode == ERROR_SUCCESS) {
        printf(TEXT("(%d) %s\n"), i + 1, achKey);
      }
    }
  }

  // Enumerate the key values.

  if (cValues) {
    printf("\nNumber of values: %d\n", cValues);

    for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++) {
      cchValue = MAX_VALUE_NAME;
      achValue[0] = '\0';
      retCode =
          RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL);

      if (retCode == ERROR_SUCCESS) {
        printf(TEXT("(%d) %s\n"), i + 1, achValue);
      }
    }
  }
}

int main() {
  HKEY hTestKey;

  HKEY base_path = HKEY_CURRENT_USER;
  const char *key_path = "SOFTWARE\\Microsoft";

  if (RegOpenKeyEx(base_path, TEXT(key_path), 0, KEY_READ, &hTestKey) ==
      ERROR_SUCCESS) {
    QueryKey(hTestKey);
  }

  RegCloseKey(hTestKey);
  return 0;
}
