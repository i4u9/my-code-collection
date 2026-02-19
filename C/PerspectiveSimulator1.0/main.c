#include "perspectivesimulator.c"

int main(int argc, char **argv)
{
    int status = EXIT_FAILURE;

    struct Application *app = NULL;

    print_keybindings();

    if (application_new(&app))
    {
        application_run(app);

        status = EXIT_SUCCESS;
    }
    
    application_free(&app);
    printf("Exit status: %d", status);

    return status;
}