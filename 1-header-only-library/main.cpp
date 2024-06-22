#include <iostream>
#include "some-library.h"

int main()
{
    int result = get_something();
    std::cout << "get_something() returned: " << result << "\n";

    return 0;
}
