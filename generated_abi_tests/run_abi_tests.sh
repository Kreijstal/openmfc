#!/bin/bash
# Generated ABI Test Runner

echo "=== Running Generated ABI Tests ==="

# Compile and run CString test
if [ -f test_cstring_abi_generated.cpp ]; then
    echo "\n1. Testing CString ABI..."
    g++ -std=c++17 -I../include test_cstring_abi_generated.cpp -o test_cstring_abi
    if [ $? -eq 0 ]; then
        ./test_cstring_abi
        cstring_result=$?
    else
        echo "FAIL: Could not compile CString test"
        cstring_result=1
    fi
fi

# Compile and run export test
if [ -f test_exports_abi_generated.cpp ]; then
    echo "\n2. Testing Export ABI..."
    g++ -std=c++17 test_exports_abi_generated.cpp -o test_exports_abi -ldl
    if [ $? -eq 0 ]; then
        ./test_exports_abi
        export_result=$?
    else
        echo "FAIL: Could not compile export test"
        export_result=1
    fi
fi

echo "\n=== Summary ==="
if [ $cstring_result -eq 0 ] && [ $export_result -eq 0 ]; then
    echo "✅ All ABI tests passed"
    exit 0
else
    echo "❌ ABI test failures detected"
    exit 1
fi
