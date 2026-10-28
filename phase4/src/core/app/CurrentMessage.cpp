// The global accessor returns the last pumped message, even without a thread object.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstat_.h"
// Symbol: ?AfxGetCurrentMessage@@YAPEAUtagMSG@@XZ
extern "C" MSG* AFXAPI impl__AfxGetCurrentMessage__YAPEAUtagMSG__XZ() {
    return &AfxGetThreadState()->m_msgCur;
}
