#include <Windows.h>
#include <Objbase.h>

#include "ContextMenuHandler.hh"

ULONG WINAPI ContextMenuHandler::ShellExtInit::AddRef()
{
    if (handler.pUnknownOuter)
	return handler.pUnknownOuter->AddRef();

    return handler.AddRef();
}

ULONG WINAPI ContextMenuHandler::ShellExtInit::Release()
{
    if (handler.pUnknownOuter)
	handler.pUnknownOuter->Release();

    return handler.Release();
}

HRESULT WINAPI ContextMenuHandler::ShellExtInit::QueryInterface(REFIID refIID, void **ppInterface)
{
    if (handler.pUnknownOuter)
	handler.pUnknownOuter->QueryInterface(refIID, ppInterface);

    return handler.QueryInterface(refIID, ppInterface);
}


HRESULT WINAPI ContextMenuHandler::ShellExtInit::Initialize(PCIDLIST_ABSOLUTE, IDataObject *pdtobj, HKEY)
{
    if (pDataObject)
	pDataObject->Release();

    pDataObject = pdtobj;
    pDataObject->AddRef();
}
