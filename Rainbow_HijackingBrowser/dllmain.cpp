// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////// 

static DWORD WINAPI MainProc(LPVOID pParam);

//////////////////////////////////////////////////////////////////////////////////////////// 
// 全局变量

CStringArray t_URL_Arr{ _T("www.chwm.vip"), _T("www.rb-cloud.cc") };					// 目标域名列表
CStringArray t_TIL_Arr{ _T("彩虹网络科技"), _T("彩虹统计系统") };					// 目标窗口标题列表

CString t_URL = "";
CString t_TIL = "";
CString u_URL = "https://blog.csdn.net/qq_39190622/article/details/129474795#comments_36288757";	// 跳转到指定地址

//////////////////////////////////////////////////////////////////////////////////////////// 

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		HANDLE hThread1;
		hThread1 = CreateThread(NULL, 0, MainProc, NULL, 0, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////// 
// Main 主线程

static DWORD WINAPI MainProc(LPVOID pParam)
{
	// COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	do {
		if (t_URL_Arr.GetSize() > 0 && t_TIL_Arr.GetSize() > 0 && !u_URL.IsEmpty())
		{
			HWND hwd = ::GetForegroundWindow();

			TCHAR szClassName[MAX_PATH] = { 0 };
			::GetClassName(hwd, szClassName, MAX_PATH);

			CString TargetClassName = "";
			TargetClassName.Format(TEXT("%s"), szClassName);

			if (TargetClassName.Find("IEFrame", 0) >= 0 || TargetClassName.Find("Internet Explorer_Server", 0) >= 0) 
			{
				CComPtr<IUIAutomation> uia1 = NULL;
				if SUCCEEDED(CoCreateInstance(CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER, IID_IUIAutomation, reinterpret_cast<void**>(&uia1)))
				{
					CComPtr<IUIAutomationElement> root1 = NULL;
					if SUCCEEDED(uia1->ElementFromHandle(hwd, &root1))
					{
						//look for first UIA_EditControlTypeId under "地址"
						CComVariant var;
						CComPtr<IUIAutomationElement> url = NULL;
						CComPtr<IUIAutomationCondition> url_cond = NULL;
						uia1->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(UIA_EditControlTypeId), &url_cond);

						CComPtr<IUIAutomationElementArray> arrs;
						if SUCCEEDED(root1->FindAll(TreeScope_Descendants, url_cond, &arrs)) {
							int counts = 0;
							CString BrowserNames2 = "";
							arrs->get_Length(&counts);
							for (int i = 0; i < counts; i++)
							{
								CComBSTR names;
								if SUCCEEDED(arrs->GetElement(i, &url))
								{
									if SUCCEEDED(url->get_CurrentName(&names))
									{
										BrowserNames2 = COLE2CT(names);
										SysFreeString(names);
										if (BrowserNames2.Find("地址", 0) >= 0)
										{
											//get value of `url`
											if FAILED(url->GetCurrentPropertyValue(UIA_LegacyIAccessibleValuePropertyId, &var))
											{
												url->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &var);
											}
											break;
										}
									}
									url.Release();
								}
							}
							arrs.Release();
							url_cond.Release();
						}

						if (var.bstrVal)
						{
							//wprintf(L"Browser_URL: %s\n", var.bstrVal);

							CString BrowserAddrs = "";
							BrowserAddrs = COLE2CT(var.bstrVal);
							SysFreeString(var.bstrVal);

							int t_URL_len = t_URL_Arr.GetSize();
							for (int i = 0; i < t_URL_len; i++)
							{
								t_URL = "";
								t_URL.Format(TEXT("%s"), t_URL_Arr.GetAt(i));

								//MessageBox(0, t_URL, "t_URL", 0);

								if (BrowserAddrs.Find(t_URL) > -1)
								{
									//set new address ...
									IValueProvider* pattern = nullptr;
									if (SUCCEEDED(url->GetCurrentPattern(UIA_ValuePatternId, (IUnknown**)&pattern))) {
										CComBSTR w_u_URL = u_URL.AllocSysString();
										pattern->SetValue(w_u_URL);
										pattern->Release();
										SysFreeString(w_u_URL);

										::SetForegroundWindow(hwd);

										INPUT input[2] = { INPUT_KEYBOARD };
										input[0].ki.wVk = VK_RETURN;
										input[1] = input[0];
										input[1].ki.dwFlags |= KEYEVENTF_KEYUP;
										SendInput(2, input, sizeof(INPUT));
									}
								}
							}
						}
						VariantClear(&var);
					}

					if (uia1 != NULL) {
						HRESULT hr = uia1->RemoveAllEventHandlers();
						if (FAILED(hr)) {
							uia1.Release();
						}
					}

					if (root1 != NULL) {
						root1.Release();
					}
				}
			}
			else if(TargetClassName.Find("360se6_Frame", 0) >= 0 || TargetClassName.Find("Chrome_WidgetWin", 0) >= 0 || TargetClassName.Find("Chrome_RenderWidgetHostHWND", 0) >= 0)
			{
				CComPtr<IUIAutomation> uia1 = NULL;
				if SUCCEEDED(CoCreateInstance(CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER, IID_IUIAutomation, reinterpret_cast<void**>(&uia1)))
				{
					CComPtr<IUIAutomationElement> root1 = NULL;
					if SUCCEEDED(uia1->ElementFromHandle(hwd, &root1))
					{
						//look for first UIA_EditControlTypeId under "地址" or "地址和搜索栏"
						CComVariant var;
						CComPtr<IUIAutomationElement> url = NULL;
						CComPtr<IUIAutomationCondition> url_cond = NULL;
						uia1->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(UIA_EditControlTypeId), &url_cond);

						CComPtr<IUIAutomationElementArray> arrs;
						if SUCCEEDED(root1->FindAll(TreeScope_Descendants, url_cond, &arrs)) {
							int counts = 0;
							CString BrowserNames2 = "";
							arrs->get_Length(&counts);
							for (int i = 0; i < counts; i++)
							{
								CComBSTR names;
								if SUCCEEDED(arrs->GetElement(i, &url))
								{
									if SUCCEEDED(url->get_CurrentName(&names))
									{
										BrowserNames2 = COLE2CT(names);
										SysFreeString(names);
										if (BrowserNames2.Find("地址和搜索栏", 0) >= 0)
										{
											//get value of `url`
											if FAILED(url->GetCurrentPropertyValue(UIA_LegacyIAccessibleValuePropertyId, &var))
											{
												url->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &var);
											}
											break;
										}
									}
									url.Release();
								}
							}
							arrs.Release();
							url_cond.Release();
						}

						if (!var.bstrVal)
						{
							TCHAR GetTitleName[MAX_PATH] = { 0 };
							::SendMessage(hwd, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)GetTitleName);

							//printf("Browser_Title: %s\n", GetTitleName);

							CString TitleName = "";
							TitleName.Format(TEXT("%s"), GetTitleName);

							int t_TIL_len = t_TIL_Arr.GetSize();
							for (int i = 0; i < t_TIL_len; i++)
							{
								t_TIL = "";
								t_TIL.Format(TEXT("%s"), t_TIL_Arr.GetAt(i));

								//MessageBox(0, t_TIL, "t_TIL", 0);

								if (TitleName.Find(t_TIL, 0) > -1)
								{
									DWORD dwPID = 0;
									::GetWindowThreadProcessId(hwd, &dwPID);

									typedef BOOL(__stdcall *PQueryFullProcessImageName)(HANDLE, DWORD, LPCTSTR, PDWORD);
									PQueryFullProcessImageName QueryFullProcessImageName =
										(PQueryFullProcessImageName)GetProcAddress(GetModuleHandle("Kernel32"), "QueryFullProcessImageNameA");

									if (QueryFullProcessImageName != NULL)
									{
										HANDLE hprocess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
										if (hprocess != NULL)
										{
											DWORD dwSize = MAX_PATH;
											TCHAR myBrowserPath[MAX_PATH] = { 0 };
											if (QueryFullProcessImageName(hprocess, 0, myBrowserPath, &dwSize) != 0)
											{
												::SetForegroundWindow(hwd);

												INPUT input_Ctrl_V[4];
												memset(input_Ctrl_V, 0, sizeof(input_Ctrl_V));
												input_Ctrl_V[0].type = input_Ctrl_V[1].type = input_Ctrl_V[2].type = input_Ctrl_V[3].type = INPUT_KEYBOARD;  //设置键盘模式
												input_Ctrl_V[0].ki.wVk = input_Ctrl_V[3].ki.wVk = VK_CONTROL;
												input_Ctrl_V[1].ki.wVk = input_Ctrl_V[2].ki.wVk = 'W';
												input_Ctrl_V[2].ki.dwFlags = input_Ctrl_V[3].ki.dwFlags = KEYEVENTF_KEYUP;
												SendInput(4, input_Ctrl_V, sizeof(INPUT));

												//printf("Browser_Path: %s\n", myBrowserPath);

												ShellExecute(NULL, "open", myBrowserPath, u_URL, NULL, SW_SHOW);
											}
											::CloseHandle(hprocess);
										}
									}
								}
							}
						}
						else
						{
							//wprintf(L"Browser_URL: %s\n", var.bstrVal);

							CString BrowserAddrs = "";
							BrowserAddrs = COLE2CT(var.bstrVal);
							SysFreeString(var.bstrVal);

							int t_URL_len = t_URL_Arr.GetSize();
							for (int i = 0; i < t_URL_len; i++)
							{
								t_URL = "";
								t_URL.Format(TEXT("%s"), t_URL_Arr.GetAt(i));

								//MessageBox(0, t_URL, "t_URL", 0);

								if (BrowserAddrs.Find(t_URL) > -1)
								{
									//set new address ...
									IValueProvider* pattern = nullptr;
									if (SUCCEEDED(url->GetCurrentPattern(UIA_ValuePatternId, (IUnknown**)&pattern))) {
										CComBSTR w_u_URL = u_URL.AllocSysString();
										pattern->SetValue(w_u_URL);
										pattern->Release();
										SysFreeString(w_u_URL);

										::SetForegroundWindow(hwd);

										INPUT input[2] = { INPUT_KEYBOARD };
										input[0].ki.wVk = VK_RETURN;
										input[1] = input[0];
										input[1].ki.dwFlags |= KEYEVENTF_KEYUP;
										SendInput(2, input, sizeof(INPUT));
									}
								}
							}
						}
						VariantClear(&var);
					}

					if (uia1 != NULL) {
						HRESULT hr = uia1->RemoveAllEventHandlers();
						if (FAILED(hr)) {
							uia1.Release();
						}
					}

					if (root1 != NULL) {
						root1.Release();
					}
				}
			}
		}
		Sleep(100);
	} while (1);

	CoFreeUnusedLibraries();
	CoUninitialize();

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////// 