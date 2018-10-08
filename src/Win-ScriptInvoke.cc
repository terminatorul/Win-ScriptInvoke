#include <GuidDef.h>
#include <Objbase.h>
#include <Unknown.h>

#include <new>

#include "ContextMenuHandler.hh"
#include "ClassFactory.hh"
#include "Win-ScriptInvoke.hh"

__declspec(dllexport) HRESULT CALLBACK DllCanUnloadNow()
{
    if (ClassFactory::canUnloadNow() && ContextMenuHandler::canUnloadNow())
	return S_OK;
    
    return S_FALSE;
}

__declspec(dllexport) HRESULT CALLBACK DllGetClassObject(REFCLSID refClsID, REFIID refIID, LPVOID *ppIClassFactory)
try
{
    if (!ppIClassFactory)
	return E_INVALIDARG;

    if (IsEqualCLSID(refClsID, ContextMenuHandler::ID))
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
