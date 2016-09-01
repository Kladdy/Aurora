// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#include <SFML/Graphics.hpp>
#include <iostream>

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>


#include <shellapi.h>
#include <map>
#include <string>
#include <assert.h>

#include <stdio.h>
#include <tchar.h>

#include "TrayIcon.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <tchar.h>


#pragma once

#ifndef STRICT
#define STRICT
#endif

#ifndef WINVER
#define WINVER 0x0502
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0502
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#define _AFX_ALL_WARNINGS // turns off MFC's hiding of some common and often safely ignored warning messages

#ifndef _SECURE_ATL
#define _SECURE_ATL 1 //Use the Secure C Runtime in ATL
#endif

//Pull in MFC support
//#include <afxext.h> 
//#include <afxtempl.h>

//Or Pull in Standard Windows support
#include <Windows.h>

//Pull in ATL support
#include <atlbase.h>
#include <atlstr.h>

//Other includes
#include <tchar.h>
#include <setupapi.h>
#include <malloc.h>
#include <winspool.h>
#include <Wbemcli.h>
#include <comdef.h>
#include <stdio.h>

#define CENUMERATESERIAL_USE_STL //Uncomment this line if you want to test the MFC / ATL support in CEnumerateSerial

#ifdef CENUMERATESERIAL_USE_STL
//Pull in STL support
#include <vector>
#include <string>
#endif

//Out of the box lets exclude support for CEnumerateSerial::UsingComDB on the Windows SDK 7.1 or earlier since msports.h
//is only available with the Windows SDK 8 or later.
#include <ntverp.h>
#if VER_PRODUCTBUILD <= 7600
#define NO_CENUMERATESERIAL_USING_COMDB
#endif

#ifndef NO_CENUMERATESERIAL_USING_COMDB
#include <msports.h>
#endif //#ifndef NO_CENUMERATESERIAL_USING_COMDB