// Compile once per HARVEST_CLASS with
// /d1reportSingleClassLayout<HARVEST_CLASS>. The broad include set lets the
// same translation unit cover core MFC, OLE, database, sockets, Direct2D, and
// Feature Pack classes without maintaining category-specific probes.
#ifndef _AFXDLL
#define _AFXDLL
#endif
#define _CRT_SECURE_NO_WARNINGS

#include <afxwin.h>
#include <afxext.h>
#include <afxadv.h>
#include <afxcmn.h>
#include <afxdlgs.h>
#include <afxcontrolbars.h>
#include <afxole.h>
#include <afxdisp.h>
#include <afxoledb.h>
#include <afxdb.h>
#include <afxsock.h>
#include <afxinet.h>
#include <afxtaskdialog.h>
#include <afxdtctl.h>
#include <afxhtml.h>
#include <afxrendertarget.h>

#ifndef HARVEST_CLASS
#error HARVEST_CLASS must name the class being harvested
#endif

volatile size_t g_harvested_class_size = sizeof(HARVEST_CLASS);
