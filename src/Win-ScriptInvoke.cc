#include <cstdint>
#include <new>

#include <GuidDef.h>
#include <Objbase.h>
#include <Unknown.h>

#include "ContextMenuHandler.hh"
#include "ClassFactory.hh"
#include "Win-ScriptInvoke.hh"

std::uint_least32_t objectCount = 0;

HRESULT dllexport WINAPI DllCanUnloadNow()
{
    if (!objectCount && ClassFactory::canUnloadNow() && ContextMenuHandler::canUnloadNow())
	return S_OK;
    
    return S_FALSE;
}

HRESULT dllexport WINAPI DllGetClassObject(REFCLSID refClsID, REFIID refIID, LPVOID *ppIClassFactory)
try
{
    if (!ppIClassFactory)
	return E_INVALIDARG;

    if (IsEqualCLSID(refClsID, ContextMenuHandler::contextMenuClassID))
    {
	*ppIClassFactory = static_cast<IClassFactory *>(ClassFactory::createInstance());

	HRESULT hr = (*ppIClassFactory)->QueryInterface(refIID, ppIClassFactory);

	if (SUCCEEDED(hr))
	    return S_OK;

	(*ppIClassFactory)->Release();
	*ppIClassFactory = nullptr;

	return hr;
    }

    *ppIClassFactory = nullptr;
    return CLASS_E_CLASSNOTAVAILABLE;
}
catch(std::bad_alloc const &)
{
    *ppIClassFactory = nullptr;
    return E_OUTOFMEMORY;
}
