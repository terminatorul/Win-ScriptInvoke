#include "ContextMenuHandler.hh"
#include "ClassFactory.hh"

static ClassFactory *pClassFactory = nullptr;

bool ClassFactory::canUnloadNow()
{
    if (pClassFactory && !pClassFactory->ref_count && !pClassFactory->lock_count)
    {
	delete pClassFactory;
	pClassFactory = nullptr;
    }

    return !pClassFactory;
}

ClassFactory *ClassFactory::createInstance()
{
    if (!pClassFactory)
	pClassFactory = new ClassFactory();

    return pInstance;
}

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
    }

    return ref_count;
}

HRESULT WINAPI ClassFactory::QueryInterface(REFIID refIID, void **ppInterface)
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

HRESULT WINAPI ClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
	lock_count++;
    else
    {
	if (lock_count)
	    lock_count--;
    }

    return S_OK;
}

HRESULT WINAPI ClassFactory::CreateInstance(IUnknown *pUnknownOuter, REFIID refIID, void **ppInstance)
try
{
    if (!ppInstance || (pUnknownOuter && !IsEqualIID(refIID, IID_IUnknown))
	return E_INVALIDARG;

    ContextMenuHandler *pInstance = ContextMenuHandler::createInstance(pUnknownOuter);
    HRESULT hr = pInstance->QueryInterface(refIID, ppInstance);

    if (FAILED(hr))
    {
	*ppInstance = nullptr;
	pInstance->Release();
    }

    return hr
}
catch(std::bad_alloc const &)
{
    *ppInstance = nullptr;
    return E_OUTOFMEMORY;
}
