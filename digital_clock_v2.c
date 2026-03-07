#include "digital_clock_v2.h"

int main()
{
    clear_screen();
    printf("\nWelcome to the digital clock!\n");
    printf("\'time\'           - Displays the current time\n");
    printf("\'timer XX:YY:ZZ\' - Take the limit for the timer with the format as show and starts it\n");
    printf("\"stop\"           - Stops current active timer, if there is any\n");
    printf("\'exit\'           - closes the digital clock\n\n");

    int exit_result = start();
    
    return exit_result; 
}