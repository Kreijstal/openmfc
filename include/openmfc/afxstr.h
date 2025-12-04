#pragma once
#include <string>
#include <cwchar>

// Very small CString stand-in for Linux-side consumers.
class CString {
public:
    CString() = default;
    explicit CString(const wchar_t* wstr) : data_(wstr ? wstr : L"") {}

    int GetLength() const { return static_cast<int>(data_.size()); }
    bool IsEmpty() const { return data_.empty(); }
    const wchar_t* GetString() const { return data_.c_str(); }

private:
    std::wstring data_;
};

