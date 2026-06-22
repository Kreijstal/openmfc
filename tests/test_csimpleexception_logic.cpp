// Behavioral test for CSimpleException RTTI + methods.
//
// This file #includes the impl .cpp directly and drives the impl_ thunks. It
// links nothing else, so it must supply the base RTTI descriptors that the impl
// references (CException::classCException and CObject::classCObject). In the
// real DLL those are provided by mfc_exceptions.cpp / cobject_impl.cpp; here we
// stand in for them so the descriptor chain resolves.

#include <cstdio>
#include <cstring>
#include <cwchar>

// Pull in the impl under test (brings in afxwin.h -> CException, CObject,
// CRuntimeClass, and defines classCSimpleException + the four thunks).
#include "../phase4/src/global_csimpleexception_rtti.cpp"

// ---- Standalone definition of the base descriptor the impl references ----
// CObject::classCObject is already provided inline (weak) by afx.h. But
// CException::classCException is only declared (via DECLARE_DYNAMIC) here and
// defined in mfc_exceptions.cpp in the DLL build; supply it for the standalone
// link so the descriptor chain resolves.
CRuntimeClass CException::classCException = {
    "CException", (int)sizeof(CException), 0xFFFF, nullptr, nullptr,
    &CObject::classCObject, nullptr
};
// afxwin.h's inline code also drags in a few other DECLARE_DYNAMIC descriptors
// (CCmdTarget/CWinThread/CWinApp/CWnd) whose definitions live in the DLL.
// Provide standalone definitions so this test links without the DLL.
CRuntimeClass CCmdTarget::classCCmdTarget = {
    "CCmdTarget", sizeof(CCmdTarget), 0xFFFF, nullptr, nullptr,
    &CObject::classCObject, nullptr
};
CRuntimeClass CWinThread::classCWinThread = {
    "CWinThread", sizeof(CWinThread), 0xFFFF, nullptr, nullptr,
    &CCmdTarget::classCCmdTarget, nullptr
};
CRuntimeClass CWinApp::classCWinApp = {
    "CWinApp", sizeof(CWinApp), 0xFFFF, nullptr, nullptr,
    &CWinThread::classCWinThread, nullptr
};
CRuntimeClass CWnd::classCWnd = {
    "CWnd", sizeof(CWnd), 0xFFFF, nullptr, nullptr,
    &CCmdTarget::classCCmdTarget, nullptr
};

// The impl thunks only touch POD member fields via the pointer (no virtual
// dispatch, no construction). To avoid pulling in CException/CWnd vtable
// machinery (which would require out-of-line virtual defs not linked here), we
// drive the thunks against raw, suitably-aligned/zeroed storage reinterpreted
// as CSimpleException rather than constructing a real polymorphic object.
struct alignas(CSimpleException) RawEx {
    unsigned char bytes[sizeof(CSimpleException)];
    CSimpleException* get() { return reinterpret_cast<CSimpleException*>(bytes); }
    RawEx() { std::memset(bytes, 0, sizeof(bytes)); }
};

static int g_failures = 0;
static void check(bool cond, const char* what) {
    if (cond) {
        std::printf("PASS: %s\n", what);
    } else {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

int main() {
    // Build a CSimpleException-layout object (raw storage) and seed it.
    RawEx storage;
    CSimpleException& ex = *storage.get();
    ex.m_nHelpContext = 42;
    ex.m_nResourceID  = 7;
    ex.m_bInitialized = FALSE;

    const wchar_t* msg = L"The quick brown fox jumped";  // 26 chars
    std::wcsncpy(ex.m_szMessage, msg, 256);
    ex.m_szMessage[255] = L'\0';

    // --- 1. Before InitString: not initialized -> returns 0, empty buffer. ---
    {
        wchar_t buf[64];
        for (int i = 0; i < 64; ++i) buf[i] = L'X';
        UINT help = 999;
        int r = impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(&ex, buf, 64, &help);
        check(r == 0, "uninitialized message returns 0");
        check(buf[0] == L'\0', "uninitialized message yields empty buffer");
        check(help == 42, "help context populated even when uninitialized");
    }

    // --- 2. InitString flips m_bInitialized. ---
    impl__InitString_CSimpleException__QEAAXXZ(&ex);
    check(ex.m_bInitialized == TRUE, "InitString sets m_bInitialized");

    // --- 3. Full copy when buffer is large enough. ---
    {
        wchar_t buf[64];
        UINT help = 0;
        int r = impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(&ex, buf, 64, &help);
        check(r == 1, "initialized message returns 1");
        check(std::wcscmp(buf, msg) == 0, "full message copied verbatim");
        check(help == 42, "help context returned == 42");
    }

    // --- 4. Truncation: buffer of 10 -> 9 chars + NUL. ---
    {
        wchar_t buf[10];
        UINT help = 0;
        int r = impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(&ex, buf, 10, &help);
        check(r == 1, "truncated message still returns 1");
        check(std::wcslen(buf) == 9, "truncated to nMaxError-1 (9) chars");
        check(buf[9] == L'\0', "truncated buffer NUL-terminated");
        check(std::wcsncmp(buf, msg, 9) == 0, "truncated prefix matches source");
    }

    // --- 5. nMaxError == 0 is rejected (returns 0, no write). ---
    {
        wchar_t buf[4] = { L'Z', L'Z', L'Z', L'Z' };
        int r = impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(&ex, buf, 0, nullptr);
        check(r == 0, "nMaxError==0 returns 0");
        check(buf[0] == L'Z', "nMaxError==0 leaves buffer untouched");
    }

    // --- 6. Empty message returns 0 even when initialized. ---
    {
        RawEx storage2;
        CSimpleException& ex2 = *storage2.get();
        ex2.m_bInitialized = TRUE;
        ex2.m_nHelpContext = 5;
        ex2.m_szMessage[0] = L'\0';
        wchar_t buf[16];
        UINT help = 0;
        int r = impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(&ex2, buf, 16, &help);
        check(r == 0, "empty message returns 0");
        check(buf[0] == L'\0', "empty message yields empty buffer");
        check(help == 5, "empty message still sets help context");
    }

    // --- 7. RTTI: GetThisClass / GetRuntimeClass and base chain. ---
    {
        CRuntimeClass* rc = impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ();
        check(rc != nullptr, "GetThisClass non-null");
        check(std::strcmp(rc->m_lpszClassName, "CSimpleException") == 0,
              "class name is CSimpleException");
        check(rc->m_nObjectSize == (int)sizeof(CSimpleException),
              "m_nObjectSize == sizeof(CSimpleException)");
        check(rc->m_wSchema == 0xFFFF, "schema is 0xFFFF");
        check(rc->m_pBaseClass == &CException::classCException,
              "base class chains to CException");
        check(rc->m_pBaseClass->m_pBaseClass == &CObject::classCObject,
              "CException chains to CObject");

        CRuntimeClass* rrc =
            impl__GetRuntimeClass_CSimpleException__UEBAPEAUCRuntimeClass__XZ(&ex);
        check(rrc == rc, "GetRuntimeClass == GetThisClass");
    }

    std::printf("RESULT: %s (%d failures)\n", g_failures == 0 ? "ALL PASS" : "FAILED", g_failures);
    return g_failures == 0 ? 0 : 1;
}
