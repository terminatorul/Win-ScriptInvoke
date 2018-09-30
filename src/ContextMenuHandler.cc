#include <Objbase.h>
#include <Shlobjidl.h>

#include "ContextMenuHandler.hh"

static std::uint_least32_t objectCount = 0;
static ContextMenuHandler *pContextMenuHandler = nullptr;

bool ContextMenuHandler::canUnloadNow()
{
    if (objectCount)
	return false;

    if (pContextMenuHandler)
    {
	delete pContextMenuHandler;
	pContextMenuHandler = nullptr;
    }

    return true;
}

// class ID: {D73E09D6-94A9-4D4D-9CE5-612A5E0E397B}
REFCLSID const ContextMenuHandler::ID = { 0xd73e09d6, 0x94a9, 0x4d4d, { 0x9c, 0xe5, 0x61, 0x2a, 0x5e, 0xe, 0x39, 0x7b } };

ContextMenuHandler *ContextMenuHandler::createInstance(IUnknown *pUnknownOuter)
{
    ContextMenuHandler *pInstance;

    if (pContextMenuHandler)
    {
	pInstance = pContextMenu;
	pInstane->pUnknownOuter = pUnknownOuter;
	pContextMenuHandler = nullptr;
    }
    else
	pInstance = new ContextMenuHandler(pUnknownOuter);

    objectCount++;

    return pInstance;
}

ULONG WINAPI ContextMenuHandler::AddRef()
{
    return ++ref_count;
}

ULONG WINAPI ContextMenuHandler::Release()
{
    if (ref_count)
    {
	ref_count--;

	if (ref_count)
	    return ref_count;

	delete pContextMenuHandler;
	pContextMenuHandler = this;

	if (objectCount)
	    objectCount--;

	return UINT32_C(0);
    }

    return UINT32_C(0);
}

HRESULT WINAPI ContextMenuHandler::QueryInterface(REFIID refIID, void **ppInterface)
{
    if (ppInterface)
    {
	if (IsEqualIID(refIID, IID_IUnknown))
	    *ppInterface = static_cast<IUnknown *>(this);
	else
	    if (IsEqualIID(refIID, IID_IContextMenu))
		*ppInterface = static_cast<IContextMenu *>(&this->contextMenu);
	    else
		if (IsEqualIID(refIID, IID_IHandlerInfo))
		    *ppInterface = static_cast<IHandlerInfo *>(&this->handlerInfo);
		else
		    if (IsEqualIID(refIID, IID_IShellExtInit))
			*ppInterface = static_cast<IShellExtInit *>(&this->shellExtInit);
		    else
			return E_NOINTERFACE;

	(*ppInterface)->AddRef();

	return S_OK;
    }

    return E_POINTER;
}

void ContextMenuHandler::reset()
{
    if (pDataObject)
    {
	pDataObject->Release();
	pDataObject = nullptr;
    }
}

ContextMenuHandler::ContextMenuHandler(IUnkown *pUnknownOuter)
    : pUnknownOuter(pUnknownOuter)
{
}
