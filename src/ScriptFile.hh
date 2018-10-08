#if !defined(WIN_SCRIPTINVOKE_SHABANG_LINE)
#define WIN_SCRIPTINVOKE_SHABANG_LINE

#include <string>

class ScriptFile
{
protected:
    bool loaded = false;
    std::wstring fileName;
    std::wstring shabangLine;
    std::wstring shellExecutable;
    void readShabangLine();
public:
    ScriptFile();
    ScriptFile(wchar_t const *fileName);
    bool hasShabangLine();
    bool isShellCommandMatching();
    void shellInvokeCommand(std::string &command);
    void shellInvoke();
}

#endif // !defined(WIN_SCRIPTINVOKE_SHABANG_LINE)
