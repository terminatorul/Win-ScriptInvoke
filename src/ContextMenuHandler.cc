#include <Objbase.h>
#include "ContextMenuHandler.hh"

static ContextMenu *pContextMenu = nullptr;

bool ContextMenuHandler::CanUnloadNow()
{
    if (pContextMenu)
    {
	delete pContextMenu;
	pContextMenu = nullptr;
    }

    return !pContextMenu;
}

ContextMenuHandler::createInstance()
{
    if (pContextMenu)
    {
	ContextMenuHandler *pInstance = pContextMenu;
	pContextMenu = nullptr;

	return pInstance;
    }

    return new ContextMenuHandler();
}

// {D73E09D6-94A9-4D4D-9CE5-612A5E0E397B}
REFCLSID const ContextMenuHandler::contextMenuClassID = { 0xd73e09d6, 0x94a9, 0x4d4d, { 0x9c, 0xe5, 0x61, 0x2a, 0x5e, 0xe, 0x39, 0x7b } };

ULONG WINAPI ClassFactory::AddRef()
{
    if (pUnknown)
	return pUnknown->AddRef();

    return (++objectCount, ++ref_count);
}

ULONG WINAPI ClassFactory::Release()
{
    if (pUnknown)
	return pUnknown->Release();

    if (ref_count)
    {
	if (objectCount)
	    objectCount--;

	ref_count--;

	if (lock_count || ref_count)
	    return ref_count;

	delete pClassFactory;
	pClassFactory = this;

	return INT32_C(0);
    }

    return ref_count;
}

HRESULT WINAPI ClassFacgtory::QueryInterface(REFIID refIID, void **ppInterface)
{
    if (pUnknown)
	return pUnknown->QueryInterface(refIID, ppInterface);

    if (ppInterface)
    {
	if (IsEqualIID(refIID, IID_IUnknown))
	    *ppInterface = static_cast<IUnknown *>(this);
	else
	    if (IsEqualIID(refIID, IID_IClassFactory))
		*ppInterface = static_cast<IClassFactory *>(this);
	    else
		return E_NOINTERFACE;

	(*ppInterface)->AddRef();

	return S_OK;
    }

    return E_POINTER;
}

