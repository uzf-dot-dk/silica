#include <windows.h>
#include <cstdint>
#include <iostream>
#include <silica/Application.h>





namespace Silica
{

    void Application::platformSpecificInitialization()
    {
        //Nop on Windows.
    }

    MicroSeconds Application::microsecondsSinceStart() const
    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);

        // Convert FILETIME (100-ns intervals since Jan 1, 1601) to microseconds
        ULARGE_INTEGER uli;
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;

        return MicroSeconds(static_cast<int64_t>(uli.QuadPart / 10)); // 100 ns -> µs
    }


}
