#include "include/game.h"
#include <iostream>

int main()
{
    try
    {
        Game g;
        return g.Run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unknown exception" << std::endl;
    }
}
