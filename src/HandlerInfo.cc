#include <Objbase.h>

#include "ContextMenuHandler.hh"

STDMETHODIMP_(ULONG) ContextMenuHandler::HandlerInfo::AddRef()
{
    return handler.pUnknownOuter->AddRef();
}

STDMETHODIMP_(ULONG) ContextMenuHandler::HandlerInfo::Release()
{
    return handler.pUnknownOuter->Release();
}

STDMETHODIMP ContextMenuHandler::HandlerInfo::QueryInterface(REFIID refIID, void **ppInterface)
{
    return handler.pUnknownOuter->QueryInterface(refIID, ppInterface);
}

WCHAR applicationDisplayName[] = OLESTR("Win-ScriptInvoke"),
      applicationPublisher[] = OLESTR("Timothy Madden <terminatorul@gmail.com>");

STDMETHODIMP ContextMenuHandler::HandlerInfo::GetApplicationDisplayName(LPWSTR *displayName)
{
    WCHAR *pStr = applicationDisplayName;

    while (*pStr)
	*displayName++ = *pStr++;

    *displayName++ = *pStr++;

    return S_OK;
}

STDMETHODIMP ContextMenuHandler::HandlerInfo::GetApplicationIconReference(LPWSTR *iconReference)
{
    return S_NOTIMPL;
}

STDMETHODIMP ContextMenuHandler::HandlerInfo::GetApplicationPublisher(LPWSTR *publisher)
{
    WCHAR *pStr = applicationPublisher;

    while (*pStr)
	*publisher++ = *pStr++;

    *publisher++ = *pStr++;

    return S_OK;
}
