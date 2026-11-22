// Storage operations shared by native CPtrArray and CWordArray.
#pragma once

#include "openmfc/afx.h"
#include <cstdlib>
#include <cstring>

extern "C" void* impl___2_YAPEAX_K_Z(size_t);

namespace openmfc::detail {
template<class Array, class Element>
struct PlainArrayStorage {
    static void SetSize(Array& array, INT_PTR size, INT_PTR grow) {
        if (size < 0) AfxThrowInvalidArgException();
        if (grow >= 0) array.m_nGrowBy = grow;
        if (!size) {
            free(array.m_pData);
            array.m_pData = nullptr;
            array.m_nSize = array.m_nMaxSize = 0;
            return;
        }
        if (!array.m_pData) {
            array.m_pData = Allocate(size);
            memset(array.m_pData, 0, Bytes(size));
            array.m_nMaxSize = array.m_nSize = size;
            return;
        }
        if (size <= array.m_nMaxSize) {
            if (size > array.m_nSize)
                memset(array.m_pData + array.m_nSize, 0, Bytes(size - array.m_nSize));
            array.m_nSize = size;
            return;
        }
        INT_PTR extra = array.m_nGrowBy;
        if (!extra) {
            extra = array.m_nSize / 8;
            if (extra < 4) extra = 4;
            if (extra > 1024) extra = 1024;
        }
        INT_PTR capacity = Sum(array.m_nMaxSize, extra);
        if (size >= capacity) capacity = size;
        else if (capacity < array.m_nMaxSize) AfxThrowInvalidArgException();
        Element* data = Allocate(capacity);
        CopyBytes(data, capacity, array.m_pData, array.m_nSize);
        memset(data + array.m_nSize, 0, Bytes(size - array.m_nSize));
        free(array.m_pData);
        array.m_pData = data;
        array.m_nSize = size;
        array.m_nMaxSize = capacity;
    }

    static void FreeExtra(Array& array) {
        if (array.m_nSize == array.m_nMaxSize) return;
        Element* data = nullptr;
        if (array.m_nSize) {
            data = Allocate(array.m_nSize);
            CopyBytes(data, array.m_nSize, array.m_pData, array.m_nSize);
        }
        free(array.m_pData);
        array.m_nMaxSize = array.m_nSize;
        array.m_pData = data;
    }

    static void SetAtGrow(Array& array, INT_PTR index, Element value) {
        if (index < 0) AfxThrowInvalidArgException();
        if (index >= array.m_nSize) array.SetSize(Sum(index, 1));
        array.m_pData[index] = value;
    }

    static INT_PTR Append(Array& array, const Array& source) {
        if (&array == &source) AfxThrowInvalidArgException();
        INT_PTR oldSize = array.m_nSize;
        array.SetSize(Sum(oldSize, source.m_nSize));
        if (source.m_nSize)
            CopyBytes(array.m_pData + oldSize, source.m_nSize, source.m_pData, source.m_nSize);
        return oldSize;
    }

    static void Copy(Array& array, const Array& source) {
        if (&array == &source) return;
        array.SetSize(source.m_nSize);
        CopyBytes(array.m_pData, source.m_nSize, source.m_pData, source.m_nSize);
    }

    static void InsertAt(Array& array, INT_PTR index, Element value, INT_PTR count) {
        if (index < 0 || count <= 0) AfxThrowInvalidArgException();
        if (index >= array.m_nSize) {
            array.SetSize(Sum(index, count));
        } else {
            INT_PTR oldSize = array.m_nSize;
            array.SetSize(Sum(oldSize, count));
            if (memmove_s(array.m_pData + Sum(index, count), Bytes(array.m_nSize - index - count),
                          array.m_pData + index, Bytes(oldSize - index)))
                AfxThrowInvalidArgException();
            memset(array.m_pData + index, 0, Bytes(count));
        }
        for (INT_PTR at = 0; at < count; ++at) array.m_pData[index + at] = value;
    }

    static void InsertAt(Array& array, INT_PTR index, Array* source) {
        if (!source || index < 0) AfxThrowInvalidArgException();
        if (source->GetSize() > 0) {
            array.InsertAt(index, source->GetAt(0), source->GetSize());
            for (INT_PTR at = 0; at < source->GetSize(); ++at)
                array.SetAt(Sum(index, at), source->GetAt(at));
        }
    }

    static void RemoveAt(Array& array, INT_PTR index, INT_PTR count) {
        INT_PTR end = Sum(index, count);
        if (index < 0 || count < 0 || end > array.m_nSize || end < index || end < count)
            AfxThrowInvalidArgException();
        INT_PTR remaining = array.m_nSize - end;
        if (remaining && memmove_s(array.m_pData + index, Bytes(remaining),
                                   array.m_pData + end, Bytes(remaining)))
            AfxThrowInvalidArgException();
        array.m_nSize -= count;
    }

private:
    static INT_PTR Sum(INT_PTR left, INT_PTR right) {
        return INT_PTR(UINT_PTR(left) + UINT_PTR(right));
    }
    static size_t Bytes(INT_PTR count) { return size_t(count) * sizeof(Element); }

    static Element* Allocate(INT_PTR count) {
        return static_cast<Element*>(impl___2_YAPEAX_K_Z(Bytes(count)));
    }

    static void CopyBytes(Element* to, INT_PTR capacity, const Element* from, INT_PTR count) {
        if (memcpy_s(to, Bytes(capacity), from, Bytes(count))) AfxThrowInvalidArgException();
    }
};
}
