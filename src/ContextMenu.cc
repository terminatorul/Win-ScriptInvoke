#include <Objbase.h>

#include "ContextMenuHandler.hh"

ULONG WINAPI ContextMenuHandler::ContextMenu::AddRef()
{
    if (handler.pUnknownOuter)
	return handler.pUnknownOuter->AddRef();

    return handler.AddRef();
}

ULONG WINAPI ContextMenuHandler::ContextMenu::Release()
{
    if (handler.pUnknownOuter)
	handler.pUnknownOuter->Release();

    return handler.Release();
}

HRESULT WINAPI ContextMenuHandler::ContextMenu::QueryInterface(REFIID refIID, void **ppInterface)
{
    if (handler.pUnknownOuter)
	handler.pUnknownOuter->QueryInterface(refIID, ppInterface);

    return handler.QueryInterface(refIID, ppInterface);
}
