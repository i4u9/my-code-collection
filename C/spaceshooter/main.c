#include "game.c"

int main(int argc, char *argv[])
{
    bool exit_status = EXIT_FAILURE;

    struct Game *game = NULL; 

    if(game_new(&game) == true)
    {
        game_run(game);

        exit_status = EXIT_SUCCESS;       
    }
    
    game_free(&game, exit_status);
    return exit_status;
}