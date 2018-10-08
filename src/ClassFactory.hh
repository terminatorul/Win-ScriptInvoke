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
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;
    STDMETHOD(QueryInterface)(REFIID refIID, void **ppvObject) override;

    // IClassFactory
    STDMETHOD(CreateInstance)(IUnknown *pUnknownOuter, REFIID refIID, void **ppvInstance) override;
    STDMETHOD(LockServer)(BOOL fLock) override;
};

#endif // defined(WIN_SCRIPT_INVOKE_CLASS_FACTORY)
