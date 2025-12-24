#include "headers/game.h"

float distance_square_FPoints(SDL_FPoint p1, SDL_FPoint p2)
{
    float distance_square = (float)(SDL_pow((p2.x - p1.x), 2) + SDL_pow((p2.y - p1.y), 2));
    return distance_square;
}

void enemy_check_overlap_by_distance(struct Enemy **enemy, int enemy_index, int enemies_quantity)
{
    struct Enemy *e = *enemy;
 
    e[enemy_index].rectangle.x = SDL_rand((float)WINDOW_WIDTH - e[enemy_index].rectangle.w);
    e[enemy_index].rectangle.y = SDL_rand(MAX_ENEMY_Y);

    e[enemy_index].center = (SDL_FPoint){e[enemy_index].rectangle.x + (e[enemy_index].rectangle.w / 2.0), e[enemy_index].rectangle.y + (e[enemy_index].rectangle.h / 2.0)};

    for (int i = 0; i < enemies_quantity; i++)
    {
        if (i != enemy_index && (distance_square_FPoints(e[enemy_index].center, e[i].center) < ENEMY_TOLERANCE_RADIUS_SQUARED))
        {
            enemy_check_overlap_by_distance(enemy, i, enemies_quantity);
        }
    }
}

// void enemy_check_overlap(struct Enemy **enemy, int enemy_index, int enemies_quantity)
// {
//     struct Enemy *e = *enemy;
 
//     e[enemy_index].rectangle.x = (float)SDL_rand((Sint32)(WINDOW_WIDTH - e[enemy_index].rectangle.w));
//     e[enemy_index].rectangle.y = (float)SDL_rand((Sint32)(MAX_ENEMY_Y));

//     for (int i = 0; i < enemies_quantity; i++)
//     {
//         if (i != enemy_index && SDL_HasRectIntersectionFloat(&e[enemy_index].rectangle, &e[i].rectangle) == true)
//         {
//             enemy_check_overlap(enemy, enemy_index, enemies_quantity);
//         }
//     }
// }

bool enemy_new(struct Enemy **enemy, SDL_Renderer *game_renderer, SDL_Texture **enemy_image, int enemies_quantity)
{
    *enemy = calloc(enemies_quantity, sizeof(struct Enemy));
    if (*enemy == NULL)
    {
        fprintf(stderr, "Error allocating memory to enemies\n");
        return false;
    }

    struct Enemy *e = *enemy;

    *enemy_image = IMG_LoadTexture(game_renderer, "images\\enemy.bmp");
    if (*enemy_image == NULL)
    {
        fprintf(stderr, "Error loading enemies texture: %s", SDL_GetError());
        return false;
    }
    for (int i = 0; i < enemies_quantity; i++)
    {
        if (SDL_GetTextureSize(*enemy_image, &e[i].rectangle.w, &e[i].rectangle.h) == false)
        {
            fprintf(stderr, "Error getting enemies texture size: %s\n", SDL_GetError());
            return false;
        }

        enemy_check_overlap_by_distance(enemy, i, enemies_quantity);
    }
    // for (int i = 0; i < enemies_quantity; i++)
    // {
    //     if (SDL_GetTextureSize(*enemy_image, &e[i].rectangle.w, &e[i].rectangle.h) == false)
    //     {
    //         fprintf(stderr, "Error getting enemies texture size: %s\n", SDL_GetError());
    //         return false;
    //     }

    //     SDL_srand(0);
    //     enemy_check_overlap(enemy, i, enemies_quantity);
    // }

    return true;
}

void enemy_update(struct Enemy *e, SDL_FRect bullet_rectangle, int enemies_quantity, int *kills, float delta_time)
{
    for (int i = 0; i < enemies_quantity; i++)
    {
        if (e[i].killed_enemy == false)
        {
            e[i].rectangle.y += ENEMY_VELOCITY * delta_time * TARGET_FPS;

            if (e[i].rectangle.y >= WINDOW_HEIGHT + e[i].rectangle.h)
            {
                e[i].rectangle.y = -e[i].rectangle.h;
            }

            if (SDL_HasRectIntersectionFloat(&bullet_rectangle, &e[i].rectangle) == true)
            {
                e[i].killed_enemy = true;
                e[i].rectangle = (SDL_FRect){0};
                (*kills)++;
            }
        }
    }
}

void enemy_draw(const struct Enemy *e, SDL_Renderer *game_renderer, SDL_Texture *enemy_image, int enemies_quantity)
{
    for (int i = 0; i < enemies_quantity; i++)
    {
        if (e[i].killed_enemy == false)
        {
            SDL_RenderTexture(game_renderer, enemy_image, NULL, &e[i].rectangle);
        }
    }
}
void enemy_free(struct Enemy **enemy, SDL_Texture **enemy_image, int enemies_quantity)
{
    if (*enemy != NULL)
    {
        SDL_DestroyTexture(*enemy_image);
        *enemy_image = NULL;

        free(*enemy);
        *enemy = NULL;

        printf("Enemies freed\n");
    }
}