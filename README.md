# Cdecl Wrapper

This is a proof of concept to load any C ABI compatible native DLL with [Windhawk][windhawk-ref].

[windhawk-ref]: https://windhawk.net

## Settings

You can specify the path to a x64 and x86 version of the DLL. It is recommended
to place the wrapped DLL inside the default mods folder on `C:\ProgramData\Windhawk\Engine\Mods\{32,64}`
so it's properly protected by elevated permissions in that folder to not be
replace by something malicious.

The paths to the DLL can be changed while the mod is loaded. It will handle this
properly and will issue the needed callbacks manually if it is happening at runtime.

## Example Wrapped DLL

To show that this works, a friend of mine prepared a native DLL in [Zig][ziglang-ref].
You can find the source code for the repo in his [GitHub repository _windhawk-zig_][examplerepo-ref].

[ziglang-ref]: https://ziglang.org
[examplerepo-ref]: https://github.com/s0LA1337/windhawk-zig

## Processes

For testing/POC reasons, this currently injects only to Notepad (x64) and vsls-agent (x86)
in lack of a proper other 32bit process for testing.
