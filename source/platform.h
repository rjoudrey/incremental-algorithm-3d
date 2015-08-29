#include <string>

using namespace std;

#if defined(unix) || defined(__unix__) || defined(__unix)
#define PLATFORM_UNIX
#elif defined(_WIN32)
#define PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM_OSX
#endif