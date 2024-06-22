#ifndef SOME_LIBRARY_H
#define SOME_LIBRARY_H

#ifdef _WIN32
#define SOME_LIBRARY_API extern "C" __declspec(dllexport)
#else
#define SOME_LIBRARY_API extern "C"
#endif

SOME_LIBRARY_API int get_something();

#endif // SOME_LIBRARY_H
