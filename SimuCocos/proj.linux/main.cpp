#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "../Classes/AppDelegate.hpp"

USING_NS_CC;

int main(int argc, char **argv)
{
    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
