#include "CDataExchangeSupport.h"

static_assert(sizeof(S_Cdataexchange) == 24, "CDataExchange must be 24 bytes");
static_assert(offsetof(S_Cdataexchange, m_bSaveAndValidate) == 0, "m_bSaveAndValidate offset");
static_assert(offsetof(S_Cdataexchange, m_pDlgWnd) == 8, "m_pDlgWnd offset");
static_assert(offsetof(S_Cdataexchange, m_idLastControl) == 16, "m_idLastControl offset");
static_assert(offsetof(S_Cdataexchange, m_bEditLastControl) == 20, "m_bEditLastControl offset");
