// ==WindhawkMod==
// @id           cdeclwrapper
// @name         Cdecl Wrapper
// @description  Wraps the Windhawk API into a C ABI to load DLLs written in any language
// @version      1.0
// @author       diewellenlaenge
// @github       https://github.com/diewellenlaenge
// @include      notepad.exe
// @include      vsls-agent.exe
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Cdecl Wrapper

This is a proof of concept to load any C ABI compatible native DLL with Windhawk.

## Settings

You can specify the path to a x64 and x86 version of the DLL. It is recommended
to place the DLL inside the default mods folder on `C:\ProgramData\Windhawk\Engine\Mods\{32,64}`
so it's properly protected by elevated permissions in that folder to not be
replace by something malicious.

The paths to the DLL can be changed while the mod is loaded. It will handle this
properly and will issue the needed callbacks manually if it is happening at runtime.

## Processes

For testing/POC reasons, this currently injects only to Notepad (x64) and vsls-agent (x86)
in lack of a proper other 32bit process for testing.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- WrapperLogOutput: false
  $name: Wrapper Logging Output
  $description: Logs info on load and pre- and post-messages on redirection calls
- Wrapperx64Path: C:\ProgramData\Windhawk\Engine\Mods\64\cdeclwrapper_wrapped.dll
  $name: Path to x64 DLL
  $description: Path to x64 version of the wrapped DLL (empty for none)
- Wrapperx86Path: C:\ProgramData\Windhawk\Engine\Mods\32\cdeclwrapper_wrapped.dll
  $name: Path to x86 DLL
  $description: Path to x86 version of the wrapped DLL (empty for none)
*/
// ==/WindhawkModSettings==

#include <cstdint>
#include <memory>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

static void WrapperWh_Log(PCWSTR log)
{
  if (InternalWh_IsLogEnabled(InternalWhModPtr))
  {
    InternalWh_Log_Wrapper(log);
  }
}

static int WrapperWh_GetIntValue(PCWSTR valueName, int defaultValue)
{
  return Wh_GetIntValue(valueName, defaultValue);
}

static BOOL WrapperWh_SetIntValue(PCWSTR valueName, int value)
{
  return Wh_SetIntValue(valueName, value);
}

static size_t WrapperWh_GetStringValue(PCWSTR valueName, PWSTR stringBuffer, size_t bufferChars)
{
  return Wh_GetStringValue(valueName, stringBuffer, bufferChars);
}

static BOOL WrapperWh_SetStringValue(PCWSTR valueName, PCWSTR value)
{
  return Wh_SetStringValue(valueName, value);
}

static size_t WrapperWh_GetBinaryValue(PCWSTR valueName, BYTE* buffer, size_t bufferSize)
{
  return Wh_GetBinaryValue(valueName, buffer, bufferSize);
}

static BOOL WrapperWh_SetBinaryValue(PCWSTR valueName, const BYTE* buffer, size_t bufferSize)
{
  return Wh_SetBinaryValue(valueName, buffer, bufferSize);
}

static int WrapperWh_GetIntSetting(PCWSTR valueName)
{
  return Wh_GetIntSetting(valueName);
}

static PCWSTR WrapperWh_GetStringSetting(PCWSTR valueName)
{
  return Wh_GetStringSetting(valueName);
}

static void WrapperWh_FreeStringSetting(PCWSTR string)
{
  Wh_FreeStringSetting(string);
}

static BOOL WrapperWh_SetFunctionHook(void* targetFunction, void* hookFunction, void** originalFunction)
{
  return Wh_SetFunctionHook(targetFunction, hookFunction, originalFunction);
}

static HANDLE WrapperWh_FindFirstSymbol(HMODULE hModule, PCWSTR symbolServer, WH_FIND_SYMBOL* findData)
{
  return Wh_FindFirstSymbol(hModule, symbolServer, findData);
}

static BOOL WrapperWh_FindNextSymbol(HANDLE symSearch, WH_FIND_SYMBOL* findData)
{
  return Wh_FindNextSymbol(symSearch, findData);
}

static void WrapperWh_FindCloseSymbol(HANDLE symSearch)
{
  return Wh_FindCloseSymbol(symSearch);
}

struct WrapperApi
{
  void (*Wh_Log)(PCWSTR log);

  int (*Wh_GetIntValue)(PCWSTR valueName, int defaultValue);
  BOOL (*Wh_SetIntValue)(PCWSTR valueName, int value);
  size_t (*Wh_GetStringValue)(PCWSTR valueName, PWSTR stringBuffer, size_t bufferChars);
  BOOL (*Wh_SetStringValue)(PCWSTR valueName, PCWSTR value);
  size_t (*Wh_GetBinaryValue)(PCWSTR valueName, BYTE* buffer, size_t bufferSize);
  BOOL (*Wh_SetBinaryValue)(PCWSTR valueName, const BYTE* buffer, size_t bufferSize);

  int (*Wh_GetIntSetting)(PCWSTR valueName);
  PCWSTR (*Wh_GetStringSetting)(PCWSTR valueName);
  void (*Wh_FreeStringSetting)(PCWSTR string);

  BOOL (*Wh_SetFunctionHook)(void* targetFunction, void* hookFunction, void** originalFunction);

  HANDLE (*Wh_FindFirstSymbol)(HMODULE hModule, PCWSTR symbolServer, WH_FIND_SYMBOL* findData);
  BOOL (*Wh_FindNextSymbol)(HANDLE symSearch, WH_FIND_SYMBOL* findData);
  void (*Wh_FindCloseSymbol)(HANDLE symSearch);
};

typedef BOOL (*WrapperWh_ModInit)(const WrapperApi* api);
typedef void (*WrapperWh_ModAfterInit)(void);
typedef void (*WrapperWh_ModBeforeUninit)(void);
typedef void (*WrapperWh_ModUninit)(void);
typedef void (*WrapperWh_ModSettingsChanged)(void);
#ifdef __cplusplus
}
#endif

static WrapperApi api
{
  .Wh_Log = WrapperWh_Log,
  .Wh_GetIntValue = WrapperWh_GetIntValue,
  .Wh_SetIntValue = WrapperWh_SetIntValue,
  .Wh_GetStringValue = WrapperWh_GetStringValue,
  .Wh_SetStringValue = WrapperWh_SetStringValue,
  .Wh_GetBinaryValue = WrapperWh_GetBinaryValue,
  .Wh_SetBinaryValue = WrapperWh_SetBinaryValue,
  .Wh_GetIntSetting = WrapperWh_GetIntSetting,
  .Wh_GetStringSetting = WrapperWh_GetStringSetting,
  .Wh_FreeStringSetting = WrapperWh_FreeStringSetting,
  .Wh_SetFunctionHook = WrapperWh_SetFunctionHook,
  .Wh_FindNextSymbol = WrapperWh_FindNextSymbol,
  .Wh_FindCloseSymbol = WrapperWh_FindCloseSymbol,
};

std::wstring path;
bool wrapper_log = false;

class WrappedLibrary
{
public:
  WrappedLibrary(const std::wstring& path)
    : path_(path)
  {
    module_ = LoadLibrary(path_.c_str());

    if (wrapper_log) { Wh_Log(L"Loading wrapped DLL: %ws", path_.c_str()); }

    if (!module_)
    {
      error_ = L"DLL not found: " + path;
      return;
    }

    Wh_ModInit_ = reinterpret_cast<WrapperWh_ModInit>(GetProcAddress(module_, "Wh_ModInit"));

    if (!Wh_ModInit_)
    {
      error_ = L"wrapped DLL needs at least Wh_ModInit export";
      return;
    }

    Wh_ModAfterInit_ = reinterpret_cast<WrapperWh_ModAfterInit>(GetProcAddress(module_, "Wh_ModAfterInit"));
    Wh_ModBeforeUninit_ = reinterpret_cast<WrapperWh_ModBeforeUninit>(GetProcAddress(module_, "Wh_ModBeforeUninit"));
    Wh_ModUninit_ = reinterpret_cast<WrapperWh_ModUninit>(GetProcAddress(module_, "Wh_ModUninit"));
    Wh_ModSettingsChanged_ = reinterpret_cast<WrapperWh_ModSettingsChanged>(GetProcAddress(module_, "Wh_ModSettingsChanged"));
  }

  WrappedLibrary(const WrappedLibrary&) = delete;
  const WrappedLibrary& operator=(const WrappedLibrary&) = delete;

  ~WrappedLibrary()
  {
    if (module_)
    {
      Wh_ModBeforeUninit();
      Wh_ModUninit();

      auto copy = module_;
      module_ = nullptr;
      FreeLibrary(copy);
    }
  }

  explicit operator bool() const
  {
    return error_.empty() && module_ != NULL;
  }

  const std::wstring& error() const
  {
    return error_;
  }

  explicit operator HMODULE() const
  {
    return module_;
  }

  const std::wstring& path() const
  {
    return path_;
  }

  BOOL Wh_ModInit(const WrapperApi* api)
  {
    if (Wh_ModInit_)
    {
      return Wh_ModInit_(api);
    }

    return FALSE;
  }

  void Wh_ModAfterInit(void)
  {
    if (Wh_ModAfterInit_)
    {
      auto copy = Wh_ModAfterInit_;
      Wh_ModAfterInit_ = nullptr;
      copy();
    }
  }

  void Wh_ModBeforeUninit(void)
  {
    if (Wh_ModBeforeUninit_)
    {
      auto copy = Wh_ModBeforeUninit_;
      Wh_ModBeforeUninit_ = nullptr;
      copy();
    }
  }

  void Wh_ModUninit(void)
  {
    if (Wh_ModUninit_)
    {
      auto copy = Wh_ModUninit_;
      Wh_ModUninit_ = nullptr;
      copy();
    }
  }

  void Wh_ModSettingsChanged(void)
  {
    if (Wh_ModSettingsChanged_)
    {
      Wh_ModSettingsChanged_();
    }
  }


private:
  std::wstring error_;
  std::wstring path_;
  HMODULE module_;
  WrapperWh_ModInit Wh_ModInit_;
  WrapperWh_ModAfterInit Wh_ModAfterInit_;
  WrapperWh_ModBeforeUninit Wh_ModBeforeUninit_;
  WrapperWh_ModUninit Wh_ModUninit_;
  WrapperWh_ModSettingsChanged Wh_ModSettingsChanged_;
};

std::unique_ptr<WrappedLibrary> dll;

// helpers
inline std::wstring get_path()
{
#if INTPTR_MAX == INT64_MAX
  auto temp = Wh_GetStringSetting(L"Wrapperx64Path");
#elif INTPTR_MAX == INT32_MAX
  auto temp = Wh_GetStringSetting(L"Wrapperx86Path");
#endif
  std::wstring copy { temp };
  Wh_FreeStringSetting(temp);

  return copy;
}

//
void unload(bool fake)
{
  if (dll)
  {
    if (fake)
    {
      if (wrapper_log) { Wh_Log(L"Beginning fake unload"); }

      if (wrapper_log) { Wh_Log(L"Faking Wh_ModBeforeUninit"); }
      dll->Wh_ModBeforeUninit();
      if (wrapper_log) { Wh_Log(L"Done faking Wh_ModBeforeUninit"); }

      if (wrapper_log) { Wh_Log(L"Faking Wh_ModUninit"); }
      dll->Wh_ModUninit();
      if (wrapper_log) { Wh_Log(L"Done faking Wh_ModUninit"); }
    }

    if (wrapper_log) { Wh_Log(L"Begin FreeLibrary"); }
    dll.reset();
    if (wrapper_log) { Wh_Log(L"Done FreeLibrary"); }
  }
}

BOOL load(const std::wstring& path, bool fake)
{
  unload(fake);

  auto temp = std::make_unique<WrappedLibrary>(path);

  if (!*temp)
  {
    if (wrapper_log) { Wh_Log(L"%ws", temp->error().c_str()); }
    return FALSE;
  }

  dll = std::move(temp);

  if (fake)
  {
    if (wrapper_log) { Wh_Log(L"Beginning fake load"); }

    if (wrapper_log) { Wh_Log(L"Faking Wh_ModInit"); }
    auto result = dll->Wh_ModInit(&api);
    if (wrapper_log) { Wh_Log(L"Done faking Wh_ModInit"); }

    if (wrapper_log) { Wh_Log(L"Faking Wh_ModAfterInit"); }
    dll->Wh_ModAfterInit();
    if (wrapper_log) { Wh_Log(L"Done faking Wh_ModAfterInit"); }

    return result;
  }

  if (wrapper_log) { Wh_Log(L"Redirecting Wh_ModInit"); }
  auto result = dll->Wh_ModInit(&api);
  if (wrapper_log) { Wh_Log(L"Done redirecting Wh_ModInit"); }

  return result;
}

// callbacks
BOOL Wh_ModInit()
{
  wrapper_log = Wh_GetIntSetting(L"WrapperLogOutput") != 0;
  if (wrapper_log) { Wh_Log(L"Wh_ModInit"); }

  return load(get_path(), false);
}

void Wh_ModAfterInit(void)
{
  if (dll)
  {
    if (wrapper_log) { Wh_Log(L"Redirecting Wh_ModAfterInit");}
    dll->Wh_ModAfterInit();
    if (wrapper_log) { Wh_Log(L"Done redirecting Wh_ModAfterInit"); }
  }
}

void Wh_ModBeforeUninit(void)
{
  if (dll)
  {
    if (wrapper_log) { Wh_Log(L"Redirecting Wh_ModBeforeUninit"); }
    dll->Wh_ModBeforeUninit();
    if (wrapper_log) { Wh_Log(L"Done redirecting Wh_ModBeforeUninit");}
  }
}

void Wh_ModUninit()
{
  if (dll)
  {
    if (wrapper_log) { Wh_Log(L"Redirecting Wh_ModUninit"); }
    dll->Wh_ModUninit();
    if (wrapper_log) { Wh_Log(L"Done redirecting Wh_ModUninit"); }
  }

  unload(false);
}

void Wh_ModSettingsChanged()
{
  wrapper_log = Wh_GetIntSetting(L"WrapperLogOutput") != 0;

  auto new_path = get_path();
  if (new_path != path)
  {
    if (wrapper_log) { Wh_Log(L"DLL path changed, reloading: %ws", path.c_str()); }
    load(new_path, true);
    return;
  }

  if (dll)
  {
    if (wrapper_log) { Wh_Log(L"Redirecting Wh_ModSettingsChanged"); }
    dll->Wh_ModSettingsChanged();
    if (wrapper_log) { Wh_Log(L"Done redirecting Wh_ModSettingsChanged"); }
  }
}
