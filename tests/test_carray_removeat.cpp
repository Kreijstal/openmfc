// Test that CArray::RemoveAt destroys removed elements.
#include "openmfc/afx.h"
#include <cstdio>

struct Tracker {
    static int alive;
    static int nextId;

    int id;

    Tracker() : id(++nextId) { ++alive; }
    Tracker(const Tracker& other) : id(other.id) { ++alive; }
    Tracker& operator=(const Tracker& other) {
        id = other.id;
        return *this;
    }
    ~Tracker() { --alive; }
};

int Tracker::alive = 0;
int Tracker::nextId = 0;

int main() {
    {
        CArray<Tracker, const Tracker&> arr;
        arr.SetSize(5);
        if (Tracker::alive != 5) {
            std::printf("FAIL: expected 5 live after SetSize, got %d\n", Tracker::alive);
            return 1;
        }

        arr.RemoveAt(1, 2);
        if (arr.GetSize() != 3) {
            std::printf("FAIL: expected size 3 after RemoveAt, got %d\n", arr.GetSize());
            return 1;
        }
        if (Tracker::alive != 3) {
            std::printf("FAIL: expected 3 live after RemoveAt, got %d\n", Tracker::alive);
            return 1;
        }

        arr.RemoveAt(0, 3);
        if (arr.GetSize() != 0) {
            std::printf("FAIL: expected size 0 after RemoveAt all, got %d\n", arr.GetSize());
            return 1;
        }
        if (Tracker::alive != 0) {
            std::printf("FAIL: expected 0 live after RemoveAt all, got %d\n", Tracker::alive);
            return 1;
        }
    }

    if (Tracker::alive != 0) {
        std::printf("FAIL: expected 0 live after scope exit, got %d\n", Tracker::alive);
        return 1;
    }

    std::printf("OK: CArray::RemoveAt destroys removed elements\n");
    return 0;
}
