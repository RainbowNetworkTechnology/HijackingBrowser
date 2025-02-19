// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#pragma warning(disable:4091)
#pragma warning(disable:4244)

// NO_WARN_MBCS_MFC_DEPRECATION
#pragma warning(disable:4996)

#include "targetver.h"

#include "resource.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// MFC 扩展头文件
#include <afxinet.h>
// Windows 头文件: 
#include <windows.h>



// TODO:  在此处引用程序需要的其他头文件

#include <comdef.h>
#include <objbase.h>
#include <AtlBase.h>
#include <AtlCom.h>
#include <UIAutomation.h>

#include "Shlobj.h"

#include <ShellAPI.h>
#pragma comment(lib, "shell32.lib")

#include <Wininet.h>
#pragma comment(lib, "Wininet.lib") 

#pragma comment(lib, "ole32.lib")
