#include <Objbase.h>

#include "ContextMenuHandler.hh"

ULONG WINAPI ContextMenuHandler::HandlerInfo::AddRef()
{
    if (handler.pUnknownOuter)
	return handler.pUnknownOuter->AddRef();

    return handler.AddRef();
}

ULONG WINAPI ContextMenuHandler::HandlerInfo::Release()
{
    if (handler.pUnknownOuter)
	handler.pUnknownOuter->Release();

    return handler.Release();
}

HRESULT WINAPI ContextMenuHandler::HandlerInfo::QueryInterface(REFIID refIID, void **ppInterface)
{
    if (handler.pUnknownOuter)
	handler.pUnknownOuter->QueryInterface(refIID, ppInterface);

    return handler.QueryInterface(refIID, ppInterface);
}

WCHAR applicationDisplayName[] = L"Win-ScriptInvoke",
      applicationPublisher[] = L"Timothy Madden <terminatorul@gmail.com>";

HRESULT WINAPI ContextMenuHandler::HandlerInfo::GetApplicationDisplayName(LPWSTR *displayName)
{
    WCHAR *pStr = applicationDisplayName;

    while (*pStr)
	*displayName++ = *pStr++;

    *displayName++ = *pStr++;

    return S_OK;
}

HRESULT WINAPI ContextMenuHandler::HandlerInfo::GetApplicationIconReference(LPWSTR *iconReference)
{
    return S_NOTIMPL;
}

HRESULT WINAPI ContextMenuHandler::HandlerInfo::GetApplicationPublisher(LPWSTR *publisher)
{
    WCHAR *pStr = applicationPublisher;

    while (*pStr)
	*publisher++ = *pStr++;

    *publisher++ = *pStr++;

    return S_OK;
}
