#include <stdexcept>

#include "window_miqp.h"

int main()
{
    try
    {
        USTC_CG::MIQP w("MIQPdemo");
        if (!w.init())
            return 1;

        w.run();
        return 0;
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }
}