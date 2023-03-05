#include "dbbeagleapplication.h"

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        DBBeagleApplication app(argc, argv);

        return app.exec();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Cought an exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Cought an unknown exception" << std::endl;
    }

    return EXIT_FAILURE;
}
