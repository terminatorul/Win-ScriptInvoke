#if !defined(WIN_SCRIPT_INVOKE_CONTEXT_MENU_HANDLER)
#define WIN_SCRIPT_INVOKE_CONTEXT_MENU_HANDLER
#include <cstdint>

#include <Objbase.h>
#include <Shlobjidl.h>
#include <propsys.h>

#include "ScriptFile.hh"

class ContextMenuHandler: public IUnknown
{
protected:
    ULONG	    ref_count = UINT32_C(0);
    IUnknown	   *pUnknownOuter;
    IDataObject	   *pDataObject = nullptr;
    void	    reset();
    ScriptFile	    scriptFile;

public:
    static const CLSID	    ID;
    bool		    canUnloadNow();
    ContextMenuHandler	   *createInstance(IUnknown *pUnknownOuter);

    // IUnknown
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;
    STDMETHOD(QueryInterface)(REFIID refIID, void **ppvInterface) override;

    class ContextMenu: public IContextMenu
    {
    protected:
	ContextMenuHandler &handler;

    public:
	ContextMenu(ContextMenuHandler &handler);

	STDMETHOD_(ULONG, AddRef)() override;
	STDMETHOD_(ULONG, Release)() override;
	STDMETHOD(QueryInterface)(REFIID refIID, void **ppvInterface) override;

	// IContextMenu
	STDMETHOD(GetCommandString)(UINT_PTR iCmd, UINT uType, UINT *pReserved, CHAR *pszName, UINT cchMax) override;
	STDMETHOD(InvokeCommand)(CMINVOKECOMMANDINFO *pInvokeCommandInfo) override;
	STDMETHOD(QueryContextMenu)(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags) override;
    }
	contextMenu;

    friend class ContextMenuHandler::ContextMenu;

    class HandlerInfo: public IHandlerInfo
    {
    protected:
	ContextMenuHandler &handler;
    public:
	HandlerInfo(ContextMenuHandler &handler);

	STDMETHOD_(ULONG, AddRef)() override;
	STDMETHOD_(ULONG, Release)() override;
	STDMETHOD(QueryInterface)(REFIID refIID, void **ppvInterface) override;

	// IHandlerInfo
	STDMETHOD(GetApplicationDisplayName)(LPWSTR *displayName) override;
	STDMETHOD(GetApplicationIconReference)(LPWSTR *iconReference) override;
	STDMETHOD(GetApplicationPublisher)(LPWSTR *publisher) override;
    }
	handlerInfo;

    friend class ContextMenuHandler::HandlerInfo;

    class ShellExtInit: public IShellExtInit
    {
    protected:
	ContextMenuHandler &handler;
    public:
	ShellExtInit(ContextMenuHandler &handler);

	STDMETHOD_(ULONG, AddRef)() override;
	STDMETHOD_(ULONG, Release)() override;
	STDMETHOD(QueryInterface)(REFIID refIID, void **ppvInterface) override;
	//
	// IShellExtInit
	STDMETHOD(Initialize)(PCIDLIST_ABSOLUTE pidlFolder, IDataObject *pdtobj, HKEY hkeyProgID) override;
    }
	shellExtInit;

    friend class ContextMenuHandler::ShellExtInit;

    class InitializeWithStream: public IInitializeWithStream
    {
    protected:
	ContextMenuHandler &handler;
    public:
	InitializeWithStream(ContextMenuHandler &handler);

	STDMETHOD_(ULONG, AddRef)() override;
	STDMETHOD_(ULONG, Release)() override;
	STDMETHOD(QueryInterface)(REFIID refIID, void **ppvInterface) override;
	//
	// IInitializeWithStream
	STDMETHOD(Initialize)(IStream *pStream, DWORD grfMode) override;
    }
	initializeWithStream;

    friend class ContextMenuHandler::InitializeWithStream;

    ContextMenuHandler(IUnknown *pUnknownOuter = nullptr);
    ~ContextMenuHandler();
};

#endif // defined(WIN_SCRIPT_INVOKE_CONTEXT_MENU_HANDLER)
