// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#pragma warning(disable:4091)
#pragma warning(disable:4244)

// NO_WARN_MBCS_MFC_DEPRECATION
#pragma warning(disable:4996)

#include "targetver.h"

#include "resource.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// MFC ��չͷ�ļ�
#include <afxinet.h>
// Windows ͷ�ļ�: 
#include <windows.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

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
