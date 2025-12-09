// Unit test for AfxGetDllVersion functionality
// Tests actual behavior, not just ABI compatibility

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <windows.h>

// Import the function we're testing
__declspec(dllimport) uint32_t __cdecl AfxGetDllVersion();

// Helper to extract version components
inline uint16_t GetMajorVersion(uint32_t version) {
    return (version >> 16) & 0xFFFF;
}

inline uint16_t GetMinorVersion(uint32_t version) {
    return version & 0xFFFF;
}

int main() {
    int failures = 0;
    
    uint32_t version = AfxGetDllVersion();
    
    if (version == 0) {
        failures++;
    }
    
    uint16_t major = GetMajorVersion(version);
    uint16_t minor = GetMinorVersion(version);
    
    if (!(major == 0x000E && minor == 0x0000) && !(major >= 0x000C)) {
        failures++;
    }
    
    uint32_t version2 = AfxGetDllVersion();
    uint32_t version3 = AfxGetDllVersion();
    
    if (version != version2 || version != version3) {
        failures++;
    }
    
    uint32_t reconstructed = (major << 16) | minor;
    if (reconstructed != version) {
        failures++;
    }
    
    return failures == 0 ? 0 : 1;
}