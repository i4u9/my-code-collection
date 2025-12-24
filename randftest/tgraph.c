#include "testcontrol.h"
#include <SDL3/SDL.h>

#define RESULFILE_BUFFERSIZE 750
#define SDL_FLAGS (SDL_INIT_VIDEO)

#define TITLE "randftestgraph"
#define WIDTH 1800
#define HEIGHT 950

struct App
{
    bool is_running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
};

struct Graph_PlotationLayer
{
    FILE *pResultfile;
    int **file_result_array;
};

struct Graph_Elements
{
    float origin_x;
    float origin_y;
    float unit;    // mesured in px
    float ratio_x;
    float ratio_y;
    float unit_mark_height;
    float unit_mark_quantity_x_p;
    float unit_mark_quantity_y_p;
    float unit_mark_quantity_x_n;
    float unit_mark_quantity_y_n;
    struct Graph_PlotationLayer result;
};

int error_checking(void*, char*);
int read_resultfile(int**, int, int);
void free_result_array(int**, int);

bool get_results(struct Graph_PlotationLayer *result);
bool app_sdl_init(struct App *app);
bool app_new(struct App *app);
void app_events(struct App *app, struct Graph_Elements *graph);
void draw_graph(struct App *app, struct Graph_Elements *graph);
void plot_results(struct App *app, struct Graph_Elements *graph);
void app_draw(struct App *app, struct Graph_Elements *graph);
void app_run(struct App *app, struct Graph_Elements *graph);
void app_free(struct App *app, struct Graph_Elements *graph, bool);

int main(int argc, char *argv[])
{
    bool exit_status = EXIT_FAILURE;

    struct App app = {NULL};
    struct Graph_Elements graph = {
        10,
        HEIGHT - graph.origin_x,
        7.6,    // mesured in px
        100,
        3,
        3,
        (WIDTH - graph.origin_x) / graph.unit,
        graph.origin_y / graph.unit,
        0,//graph.origin_x / graph.unit,
        0,//(HEIGHT - graph.origin_y) / graph.unit,
        {NULL, NULL}
    };

    printf("unit_mark_quantity_x_p: %f\n", graph.unit_mark_quantity_x_p);
    printf("unit_mark_quantity_y_p: %f\n", graph.unit_mark_quantity_y_p);

    if (get_results(&graph.result) == false)
    {
        free_result_array(graph.result.file_result_array, lines_limit);
        return exit_status;
    }

    if (app_new(&app) == true)
    {
        app_run(&app, &graph);
        exit_status = EXIT_SUCCESS;
    }

    app_free(&app, &graph, exit_status);
    return exit_status;
}

int error_checking(void *var, char *message)
{
    if (var == NULL)
    {
        perror(message);
        return 1;
    }
    return 0;
}

int read_resultfile(int **result_array, int lines_limit, int tests)
{
    FILE *pResultfile = fopen("result.txt", "r");
    if (error_checking((void*)pResultfile, "Error opening file: ") == 1) {   return 1;   }
    
    char buffer[RESULFILE_BUFFERSIZE];
    if (error_checking((void*)buffer, "Error allocating memory: ") == 1) {   return 1;   }
    for (int i = 0; i < lines_limit; i++)
    {
        if (fgets(buffer, RESULFILE_BUFFERSIZE + 1, pResultfile) != NULL)
        {
            char *token = strtok(buffer, " ");
            for (int j = 0; (j < tests) && token != NULL; j++)
            {
                result_array[i][j] = atoi(token);
                token = strtok(NULL, " ");
            }
            token = NULL;
        }
        else
        {
            perror("Error reading file: ");
            return 1;
        }
    }

    fclose(pResultfile);
    return 0;
}

void free_result_array(int **result_array, int lines_limit)
{
    for (int i = 0; i < lines_limit; i++)
    {
        if (result_array[i] != NULL)
        {
            free(result_array[i]);
        }
    }
    if (result_array != NULL)
    {
        free(result_array);
    }
}

bool get_results(struct Graph_PlotationLayer *result)
{
    result->file_result_array = calloc(lines_limit, sizeof(int*));
    if (error_checking((void*)result->file_result_array, "Error allocating memory: ") == 1)   {   return false;   }
    for (int i = 0; i < lines_limit; i++)
    {
        result->file_result_array[i] = calloc(tests, sizeof(int));
        if (error_checking((void*)result->file_result_array[i], "Error allocating memory: ") == 1)   {   return false;   }
    }
    
    if (read_resultfile(result->file_result_array, lines_limit, tests) == 1)    {   return false;   }

    // for (int i = 0; i < result.file_lines_limit; i++)
    // {
    //     for (int j = 0; j < result.file_numberof_tests; j++)
    //     {
    //         printf("%d ", result.file_result_array[i][j]);
    //     }
    //     printf("\n");
    // }
    
    return true;
}

bool app_sdl_init(struct App *app)
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        fprintf(stderr, "Error initializing SDL video subsystem: %s\n", SDL_GetError());
        return false;
    }

    app->window = SDL_CreateWindow(TITLE, WIDTH, HEIGHT, 0);
    if (app->window == NULL)
    {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return false;
    }

    app->renderer = SDL_CreateRenderer(app->window, NULL);
    if (app->renderer == NULL)
    {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool app_new(struct App *app)
{
    if (app_sdl_init(app) == false)
    {
        return false;
    }

    app->is_running = true;

    return true;
}

void app_events(struct App *app, struct Graph_Elements *graph)
{
    while (SDL_PollEvent(&app->event) == true)
    {
        switch (app->event.type)
        {
        case SDL_EVENT_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void draw_graph(struct App *app, struct Graph_Elements *graph)
{
    SDL_RenderLine(app->renderer, 0, graph->origin_y, WIDTH, graph->origin_y);   // x axis, from left to right
    SDL_RenderLine(app->renderer, graph->origin_x, HEIGHT, graph->origin_x, 0);     // y axis, from bottom to top

    float x1_x_p = 0;
    float y1_x_p = 0;
    //float x2_x_p = x1_x_p;
    float y2_x_p = 0;

    float x1_x_n = 0;
    //float y1_x_n = y1_x_p;
    //float x2_x_n = x1_x_n;
    //float y2_x_n = y2_x_p;

    float x1_y_p = 0;
    float y1_y_p = 0;
    float x2_y_p = 0;
    //float y2_y_p = y1_y_p;

    //float x1_y_n = x1_y_p;
    float y1_y_n = 0;
    //float x2_y_n = x2_y_p;
    //float y2_y_n = y1_y_n;

    for (float i = 1; i <= graph->unit_mark_quantity_x_p; i++)
    {
        x1_x_p =  graph->origin_x + (graph->unit * i);
        y1_x_p = graph->origin_y - graph->unit_mark_height;
        y2_x_p = graph->origin_y + graph->unit_mark_height;
        SDL_RenderLine(app->renderer, x1_x_p, y1_x_p, x1_x_p, y2_x_p);
    }
    for (float i = 1; i <= graph->unit_mark_quantity_x_n; i++)
    {
        x1_x_n = graph->origin_x - (graph->unit * i);
        SDL_RenderLine(app->renderer, x1_x_n, y1_x_p, x1_x_n, y2_x_p);
    }
    for (float i = 1; i <= graph->unit_mark_quantity_y_p; i++)
    {
        x1_y_p = graph->origin_x - graph->unit_mark_height;
        y1_y_p = graph->origin_y - (graph->unit * i);
        x2_y_p = graph->origin_x + graph->unit_mark_height;
        SDL_RenderLine(app->renderer, x1_y_p, y1_y_p, x2_y_p, y1_y_p);
    }
    for (float i = 1; i <= graph->unit_mark_quantity_y_n; i++)
    {
        y1_y_n = graph->origin_y + (graph->unit * i);
        SDL_RenderLine(app->renderer, x1_y_p, y1_y_n, x2_y_p, y1_y_n);
    }
}

void plot_results(struct App *app, struct Graph_Elements *graph)
{
    for (int i = 1; i <= lines_limit; i++)
    {
        for (int j = 0; j < tests; j++)
        {
            float x = graph->origin_x + (((float)graph->result.file_result_array[i - 1][j] / graph->ratio_x) * graph->unit);
            float y = graph->origin_y - (((float)i / graph->ratio_y) * graph->unit);
            
            if (x > (WIDTH - 1) || y > (HEIGHT - 1))    {   break;  }
            SDL_RenderPoint(app->renderer, x, y);
        }
    }
    for (int i = 1; i <= lines_limit; i++)
    {
        for (int j = 0; j < tests; j++)
        {
            float x = graph->origin_x + (((float)graph->result.file_result_array[i - 1][j]) * graph->unit);
            float y = graph->origin_y - (((float)i) * graph->unit);
            
            if (x > (WIDTH - 1) || y > (HEIGHT - 1))    {   break;  }
            SDL_RenderPoint(app->renderer, x, y);
        }
    }
}

void app_draw(struct App *app, struct Graph_Elements *graph)
{
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    draw_graph(app, graph);
    plot_results(app, graph);

    SDL_RenderPresent(app->renderer);
}

void app_run(struct App *app, struct Graph_Elements *graph)
{
    while (app->is_running == true)
    {
        app_events(app, graph);
        app_draw(app, graph);
        SDL_Delay(16);
    }
}

void app_free(struct App *app, struct Graph_Elements *graph, bool status)
{
    if (app->window != NULL)
    {
        SDL_DestroyWindow(app->window);
        app->window = NULL;
    }

    if (app->renderer != NULL)
    {
        SDL_DestroyRenderer(app->renderer);
        app->renderer = NULL;
    }

    SDL_Quit();
    printf("\n%d\n", status);
}