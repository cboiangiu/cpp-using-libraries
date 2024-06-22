#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <string>

#ifdef _WIN32
#include <windows.h>
#define LIB_EXT ".dll"
#define GetFunctionPointer GetProcAddress
#define FreeDynamicLibrary FreeLibrary
#elif __APPLE__ || __linux__
#include <dlfcn.h>
#define LIB_EXT ".so"
#define GetFunctionPointer dlsym
#define FreeDynamicLibrary dlclose
#endif

class DynamicLibrary
{
public:
    typedef int (*get_something_function_t)();

    DynamicLibrary(const std::string &path) : libPath(path)
    {
        loadLibrary();
        monitorThread = std::thread(&DynamicLibrary::monitorLibrary, this);
    }

    ~DynamicLibrary()
    {
        if (handle)
        {
            FreeDynamicLibrary(handle);
        }
        monitorThread.join();
    }

    int callGetSomethingFunction()
    {
        return get_something_function();
    }

private:
    void *handle = nullptr;
    get_something_function_t get_something_function = nullptr;
    std::string libPath;
    std::thread monitorThread;

    std::string getLoadLibraryError()
    {
#ifdef _WIN32
        return std::system_category().message(GetLastError());
#else
        char *error = dlerror();
        if (error != nullptr)
        {
            return std::string(error);
        }
        else
        {
            return "Unknown error";
        }
#endif
    }

    void *loadDynamicLibrary(const char *path)
    {
#ifdef _WIN32
        return LoadLibraryA(path);
#else
        return dlopen(path, RTLD_NOW);
#endif
    }

    void loadLibrary()
    {
        std::cout << "Loading library..." << std::endl;

        if (handle)
        {
            std::cout << "Closing old library reference..." << std::endl;
            FreeDynamicLibrary(handle);
        }

        handle = loadDynamicLibrary(libPath.c_str());
        if (!handle)
        {
            std::cerr << "Error loading library: " << getLoadLibraryError() << std::endl;
            return;
        }

        get_something_function = (get_something_function_t)GetFunctionPointer(handle, "get_something");
        if (!get_something_function)
        {
            std::cerr << "Error finding function: " << getLoadLibraryError() << std::endl;
            return;
        }

        std::cout << "Library loaded successfully!" << std::endl;
    }

    void monitorLibrary()
    {
        auto lastModifiedTime = std::filesystem::last_write_time(libPath);
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto newModifiedTime = std::filesystem::last_write_time(libPath);
            if (newModifiedTime != lastModifiedTime)
            {
                loadLibrary();
                lastModifiedTime = newModifiedTime;
            }
        }
    }
};

int main()
{
    DynamicLibrary somelibrary("./libsomelibrary" + std::string(LIB_EXT));

    while (true)
    {
        int result = somelibrary.callGetSomethingFunction();
        std::cout << "get_something_function() returned: " << result << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
