#if !defined(WIN_SCRIPT_INVOKE)
#define WIN_SCRIPT_INVOKE

#include <cstdint>

#include <GuidDef.h>
#include <Objbase.h>

__declspec(dllexport) extern HRESULT CALLBACK DllCanUnloadNow();
__declspec(dllexport) extern HRESULT CALLBACK DllGetClassObject(REFCLSID refClsID, REFIID refIID, LPVOID *ppIClassFactory);

// extern BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
// extern WINAPI DllRegisterServer();
// extern WINAPI DllUnregisterServer();

#endif // defined(WIN_SCRIPT_INVOKE)
