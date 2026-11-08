// Native 24-byte borrowed/owned data wrapper and lazy clipboard attachment.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleDropTargetSupport.h"
COleDataObject::COleDataObject() : m_lpDataObject(nullptr),m_lpEnumerator(nullptr),m_bClipboard(FALSE),m_bAutoRelease(TRUE) {}
COleDataObject::~COleDataObject(){Release();}
void COleDataObject::Release() {
    if(auto* enumerator=m_lpEnumerator){enumerator->Release();m_lpEnumerator=nullptr;}
    if(m_lpDataObject) {
        if(m_bAutoRelease)m_lpDataObject->Release();
        m_lpDataObject=nullptr;
    }
    m_bClipboard=FALSE;
}
void COleDataObject::Attach(IDataObject* data,BOOL release){Release();m_lpDataObject=data;m_bAutoRelease=release;}
IDataObject* COleDataObject::Detach(){EnsureClipboardObject();auto* data=m_lpDataObject;m_lpDataObject=nullptr;m_bClipboard=FALSE;return data;}
IDataObject* COleDataObject::GetIDataObject(BOOL addRef){EnsureClipboardObject();auto* data=m_lpDataObject;if(addRef && data)data->AddRef();return data;}
BOOL COleDataObject::AttachClipboard(){m_bClipboard=TRUE;return TRUE;}
void COleDataObject::EnsureClipboardObject(){
    if(m_bClipboard && !m_lpDataObject){IDataObject* data;if(::OleGetClipboard(&data)==S_OK)Attach(data,TRUE);}
}

BOOL COleDataObject::IsDataAvailable(CLIPFORMAT format,FORMATETC* details){
    if(m_bClipboard)return ::IsClipboardFormatAvailable(format);
    if(!format && !details)return FALSE;
    FORMATETC generated;
    if(!details){generated.cfFormat=format;generated.ptd=nullptr;generated.dwAspect=DVASPECT_CONTENT;generated.lindex=-1;generated.tymed=DWORD(-1);details=&generated;}
    return m_lpDataObject->QueryGetData(details)==S_OK;
}
