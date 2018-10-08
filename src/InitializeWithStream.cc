#include "ContextMenuHandler.hh"

InitializeWithStream::InitializeWithStream(ContextMenuHandler &handler)
    : handler(handler)
{
}

STDMETHODIMP_(ULONG) ContextMenuHandler::ContextMenu::AddRef()
{
    return handler.pUnknownOuter->AddRef();
}

STDMETHODIMP_(ULONG) ContextMenuHandler::ContextMenu::Release()
{
    return handler.pUnknownOuter->Release();
}

STDMETHOD ContextMenuHandler::ContextMenu::QueryInterface(REFIID refIID, void **ppInterface)
{
    return handler.pUnknownOuter->QueryInterface(refIID, ppInterface);
}

STDMETHOD ContextMenuHandler::InitializeWithStream::Initialize(IStream *pStream, DWORD)
{
}
