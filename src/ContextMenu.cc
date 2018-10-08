#include <Objbase.h>

#include "ContextMenuHandler.hh"

STDMETHODIMP_(ULONG) ContextMenuHandler::ContextMenu::AddRef()
{
    return handler.pUnknownOuter->AddRef();
}

STDMETHODIMP_(ULONG) ContextMenuHandler::ContextMenu::Release()
{
    return handler.pUnknownOuter->Release();
}

STDMETHODIMP ContextMenuHandler::ContextMenu::QueryInterface(REFIID refIID, void **ppInterface)
{
    return handler.pUnknownOuter->QueryInterface(refIID, ppInterface);
}