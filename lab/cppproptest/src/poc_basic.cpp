#include "proptest/proptest.hpp"

#include <cstdlib>
#include <iostream>

int main()
{
    using namespace proptest;

    auto last = forAll(
        [](int x, int y) {
            PROP_ASSERT(x + y == y + x);
            return true;
        },
        {
            .seed = 42,
            .numRuns = 64,
        });

    if (!static_cast<bool>(last)) {
        std::cerr << "poc_basic: property failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "poc_basic: property passed\n";
    return EXIT_SUCCESS;
}
