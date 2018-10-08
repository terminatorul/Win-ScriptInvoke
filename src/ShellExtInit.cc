#include <Windows.h>
#include <Objbase.h>

#include <wtypes.h>
#include <Ole2.h>
#include <objidl.h>
#include <shellapi.h>

#include <cstdlib>
#include <vector>

#include "ContextMenuHandler.hh"

STDMETHODIMP_(ULONG) ContextMenuHandler::ShellExtInit::AddRef()
{
    return handler.pUnknownOuter->AddRef();
}

STDMETHODIMP_(ULONG) ContextMenuHandler::ShellExtInit::Release()
{
    return handler.pUnknownOuter->Release();
}

STDMETHODIMP ContextMenuHandler::ShellExtInit::QueryInterface(REFIID refIID, void **ppInterface)
{
    return handler.pUnknownOuter->QueryInterface(refIID, ppInterface);
}

STDMETHODIMP ContextMenuHandler::ShellExtInit::Initialize(PCIDLIST_ABSOLUTE, IDataObject *pdtobj, HKEY)
{
    if (handler.pDataObject)
	handler.pDataObject->Release();

    handler.pDataObject = pdtobj;
    handler.pDataObject->AddRef();

    STGMEDIUM storageMedium = { };
    FORMATETC format = { .cfFormat = CF_HDROP, ptd = nullptr, dwAspect = DVASPECT_CONTENT,
			    .lindex = -1, .tymed = TYMED_HGLOBAL };

    if (SUCCEEDED(pdtobj->GetData(&format, &storageMedium))
	    &&
	DragQueryFileW(static_cast<HDROP>(storageMedium.hGlobal), static_cast<UINT>(-1), nullptr, 0))
    {
	std::size_t charCount = DragQueryFileW(static_cast<HDROP>(storageMedium.hGlobal), 0, nullptr, 0);

	if (charCount)
	{
	    std::vector<wchar_t> fileName(charCount + 1);
	    charCount = DragQueryFileW(static_cast<HDROP>(storageMedium.hGlobal), 0, fileName.data(), fileName.size());

	    if (charCount && charCount <= fileName.size())
	    {
		handler.scriptFile = ScriptFile(fileName.data());
		return S_OK;
	    }
	}
    }

    return S_FALSE;
}
