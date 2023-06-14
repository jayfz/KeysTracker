#include "Application.h"
#include "ArgumentParser.h"
#include "ProgramOptions.h"

// TODO: get transposing working appropiatedly
// TODO: make trackers care about their own stretegies
// TODO: notesToRemoveFromMap from FallingNotesTracker, consider removing during the loop
// with an iterator
// TODO: toystory and sorry need work

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