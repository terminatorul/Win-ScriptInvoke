#if !defined(WIN_SCRIPT_INVOKE_CLASS_FACTORY)
#define WIN_SCRIPT_INVOKE_CLASS_FACTORY
#include <cstdint>

#include <Objbase.h>
#include <Shobjidl.h>

class ClassFactory: public IClassFactory
{
protected:
    std::uint_least32_t lock_count = 0, ref_count = 0;

public:
    static bool		   canUnloadNow();
    ClassFactory	  *createInstance();

    // IUnknown
    IUnknown		  *pUnknown = nullptr;
    virtual ULONG   WINAPI AddRef() override;
    virtual HRESULT WINAPI QueryInterface(REFIID refIID, void **ppvObject) override;
    virtual ULONG   WINAPI Release() override;

    // IClassFactory
    virtual HRESULT WINAPI CreateInstance(IUnknown *pUnknownOuter, REFIID refIID, void **ppvInstance) override;
    virtual HRESULT WINAPI LockServer(BOOL fLock) override;
};

#endif // defined(WIN_SCRIPT_INVOKE_CLASS_FACTORY)
