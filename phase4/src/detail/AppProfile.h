#pragma once

class CWinApp;

extern "C" unsigned int impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(
    CWinApp* app, const wchar_t* section, const wchar_t* entry, int defaultValue);
extern "C" int impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(
    CWinApp* app, const wchar_t* section, const wchar_t* entry, int value);
extern "C" int impl__WriteProfileStringW_CWinApp__UEAAHPEB_W00_Z(
    CWinApp* app, const wchar_t* section, const wchar_t* entry, const wchar_t* value);
