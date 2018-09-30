#if !defined(WIN_SCRIPT_INVOKE_CONTEXT_MENU_HANDLER)
#define WIN_SCRIPT_INVOKE_CONTEXT_MENU_HANDLER
#include <cstdint>

#include <Objbase.h>
#include <Shlobjidl.h>

class ContextMenuHandler: public IUnknown
{
protected:
    ULONG	    ref_count = UINT32_C(0);
    IUnknown	   *pUnknownOuter;
    IDataObject	   *pDataObject = nullptr;
    void	    reset();

public:
    static const CLSID	    ID;
    bool		    canUnloadNow();
    ContextMenuHandler	   *createInstance(IUnknown *pUnknownOuter);

    // IUnknown
    virtual ULONG   WINAPI AddRef() override;
    virtual HRESULT WINAPI QueryInterface(REFIID refIID, void **ppvInterface) override;
    virtual ULONG   WINAPI Release() override;

    class ContextMenu: public IContextMenu
    {
    protected:
	ContextMenuHandler &handler;

    public:
	ContextMenu(ContextMenuHandler &handler);

	virtual ULONG   WINAPI AddRef() override;
	virtual HRESULT WINAPI QueryInterface(REFIID refIID, void **ppvInterface) override;
	virtual ULONG   WINAPI Release() override;

	// IContextMenu
	virtual HRESULT WINAPI GetCommandString(UINT_PTR iCmd, UINT uType, UINT *pReserved, CHAR *pszName, UINT cchMax) override;
	virtual HRESULT WINAPI InvokeCommand(CMINVOKECOMMANDINFO *pInvokeCommandInfo) override;
	virtual HRESULT WINAPI QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags) override;
    }
	contextMenu;

    class HandlerInfo: public IHandlerInfo
    {
    protected:
	ContextMenuHandler &handler;
    public:
	HandlerInfo(ContextMenuHandler &handler);

	virtual ULONG   WINAPI AddRef() override;
	virtual HRESULT WINAPI QueryInterface(REFIID refIID, void **ppvInterface) override;
	virtual ULONG   WINAPI Release() override;

	// IHandlerInfo
	virtual HRESULT WINAPI GetApplicationDisplayName(LPWSTR *displayName) override;
	virtual HRESULT WINAPI GetApplicationIconReference(LPWSTR *iconReference) override;
	virtual HRESULT WINAPI GetApplicationPublisher(LPWSTR *publisher) override;
    }
	handlerInfo;

    class ShellExtInit: public IShellExtInit
    {
    protected:
	ContextMenuHandler &handler;
    public:
	ShellExtInit(ContextMenuHandler &handler);

	virtual ULONG   WINAPI AddRef() override;
	virtual HRESULT WINAPI QueryInterface(REFIID refIID, void **ppvInterface) override;
	virtual ULONG   WINAPI Release() override;
	//
	// IShellExtInit
	virtual HRESULT WINAPI Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject *pdtobj, HKEY hkeyProgID) override;
    }
	shellExtInit;

    ContextMenuHandler(IUnknown *pUnknownOuter = nullptr);
};

#endif // defined(WIN_SCRIPT_INVOKE_CONTEXT_MENU_HANDLER)
