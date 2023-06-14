#include "Application.h"
#include "ArgumentParser.h"
#include "ProgramOptions.h"

// DONE: add files to CMAKELISTS
// TODO: delete Keyboard.cpp, and Keyboard.h and any old code;
// FIX parameters in launch.json
// TODO: get transposing working

int main(int argc, char *argv[])
{
    ProgramOptions cliOptions(ArgumentParser(argc, argv));

    if (!cliOptions.areValid()) {
        cliOptions.reportErrors();
        return EXIT_FAILURE;
    }

    Application app(cliOptions);
    app.run();

    return EXIT_SUCCESS;
};