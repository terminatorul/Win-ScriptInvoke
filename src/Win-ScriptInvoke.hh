#if !defined(WIN_SCRIPT_INVOKE)
#define WIN_SCRIPT_INVOKE

#include <cstdint>

#include <GuidDef.h>
#include <Objbase.h>

extern HRESULT dllexport WINAPI DllCanUnloadNow();
extern HRESULT dllexport WINAPI DllGetClassObject(REFCLSID refClsID, REFIID refIID, LPVOID *ppIClassFactory);

// extern WINAPI DllRegisterServer();
// extern WINAPI DllUnregisterServer();

#endif // defined(WIN_SCRIPT_INVOKE)
