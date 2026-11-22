// Native DLGINIT records are packed WORD ID / WORD message / DWORD length.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxctrlcontainer.h"
#include "detail/WindowMessageSupport.h"
#include <cstring>
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" {
HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*,const wchar_t*);
void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
}

namespace {
BOOL ExecuteInitRecords(CWnd* window, BYTE* records, size_t bytes, bool bounded) {
    constexpr size_t headerSize = sizeof(WORD) * 2 + sizeof(DWORD);
    for (BYTE* record = records; record;) {
        if (bounded && bytes < sizeof(WORD)) return FALSE;
        WORD id;
        std::memcpy(&id, record, sizeof(id));
        if (!id) break;
        if (bounded && bytes < headerSize) return FALSE;

        WORD message;
        DWORD length;
        std::memcpy(&message, record + sizeof(id), sizeof(message));
        std::memcpy(&length, record + sizeof(id) + sizeof(message), sizeof(length));
        if (bounded && length > bytes - headerSize) return FALSE;
        BYTE* payload = record + headerSize;

        if (message == 0x401) message = LB_ADDSTRING;
        else if (message == 0x403) message = CB_ADDSTRING;
        if (message == LB_ADDSTRING || message == CB_ADDSTRING) {
            if (bounded && !std::memchr(payload, 0, length)) return FALSE;
            if (::SendDlgItemMessageA(window->m_hWnd, id, message, 0, reinterpret_cast<LPARAM>(payload)) == -1)
                return FALSE;
        } else if (message == 0x37c) {
            if (!window->m_pMFCCtrlContainer) {
                void* storage = impl___2_YAPEAX_K_Z(sizeof(CMFCControlContainer));
                if (!storage) impl__AfxThrowMemoryException__YAXXZ();
                window->m_pMFCCtrlContainer = new (storage) CMFCControlContainer(window);
                window->m_pMFCCtrlContainer->SubclassDlgControls();
            }
            if (::SendDlgItemMessageA(window->m_hWnd, id, message, length, reinterpret_cast<LPARAM>(payload)) == -1)
                return FALSE;
            window->m_pMFCCtrlContainer->SetControlData(id, length, payload);
        }

        if (bounded) bytes -= headerSize + length;
        record = payload + length;
    }
    CWnd::SendMessageToDescendants(window->m_hWnd, 0x364, 0, 0, FALSE, FALSE);
    return TRUE;
}
}

int CWnd::ExecuteDlgInit(void* data) {
    return ExecuteInitRecords(this, static_cast<BYTE*>(data), 0, false);
}

int CWnd::ExecuteDlgInit(const wchar_t* name) {
    if (name) {
        const auto type = MAKEINTRESOURCEW(240);
        HINSTANCE module = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name, type);
        HRSRC resource = ::FindResourceW(module, name, type);
        if (resource) {
            const auto bytes = ::SizeofResource(module, resource);
            HGLOBAL loaded = ::LoadResource(module, resource);
            auto* data = loaded ? static_cast<BYTE*>(::LockResource(loaded)) : nullptr;
            return data && ExecuteInitRecords(this, data, bytes, true);
        }
    }
    return ExecuteInitRecords(this, nullptr, 0, false);
}
// Symbol: ?ExecuteDlgInit@CWnd@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__ExecuteDlgInit_CWnd__QEAAHPEAX_Z(CWnd* self,void* data) {return self->ExecuteDlgInit(data);}
// Symbol: ?ExecuteDlgInit@CWnd@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__ExecuteDlgInit_CWnd__QEAAHPEB_W_Z(CWnd* self,const wchar_t* name) {return self->ExecuteDlgInit(name);}
