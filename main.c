#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

enum { SQUARE_SIZE = 80, BOARD_SIZE = 8, WINDOW_SIZE = 640 };

/* Piece IDs: black rook through pawn, then white rook through pawn. */
static const char *glyphs[] = {
    "♜", "♞", "♝", "♛", "♚", "♟",
    "♜", "♞", "♝", "♛", "♚", "♟"
};

static const int starting_board[BOARD_SIZE][BOARD_SIZE] = {
    { 0, 1, 2, 3, 4, 2, 1, 0 },
    { 5, 5, 5, 5, 5, 5, 5, 5 },
    {-1,-1,-1,-1,-1,-1,-1,-1 },
    {-1,-1,-1,-1,-1,-1,-1,-1 },
    {-1,-1,-1,-1,-1,-1,-1,-1 },
    {-1,-1,-1,-1,-1,-1,-1,-1 },
    {11,11,11,11,11,11,11,11 },
    { 6, 7, 8, 9,10, 8, 7, 6 }
};

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /path/to/font.ttf\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;
    SDL_Texture *pieces[12] = {0};

    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, 0);
    renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) : NULL;
    if (window && !renderer)
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    font = TTF_OpenFont(argv[1], 60);

    if (!window || !renderer || !font) {
        fprintf(stderr, "Setup failed: %s\n",
                SDL_GetError()[0] ? SDL_GetError() : TTF_GetError());
        goto cleanup;
    }

    /* Cache piece textures once; recolor them for contrast on each square. */
    SDL_Color white = {255, 255, 255, 255};
    for (int i = 0; i < 12; i++) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, glyphs[i], white);
        if (!surface) {
            fprintf(stderr, "Rendering piece glyph: %s\n", TTF_GetError());
            goto cleanup;
        }
        pieces[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!pieces[i]) {
            fprintf(stderr, "Creating piece texture: %s\n", SDL_GetError());
            goto cleanup;
        }
    }

    static const int outline_offsets[8][2] = {
        {-1, -1}, {0, -1}, {1, -1}, {-1, 0},
        {1, 0}, {-1, 1}, {0, 1}, {1, 1}
    };

    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                SDL_Rect square = {
                    col * SQUARE_SIZE, row * SQUARE_SIZE,
                    SQUARE_SIZE, SQUARE_SIZE
                };
                int dark = (row + col) % 2 == 0;
                SDL_Color background = dark
                    ? (SDL_Color){48, 48, 48, 255}
                    : (SDL_Color){240, 240, 240, 255};
                SDL_SetRenderDrawColor(renderer, background.r, background.g,
                                       background.b, background.a);
                SDL_RenderFillRect(renderer, &square);

                int piece = starting_board[row][col];
                if (piece >= 0) {
                    int width, height;
                    SDL_QueryTexture(pieces[piece], NULL, NULL, &width, &height);
                    SDL_Rect target = {
                        square.x + (SQUARE_SIZE - width) / 2,
                        square.y + (SQUARE_SIZE - height) / 2,
                        width, height
                    };

                    SDL_Color foreground = piece < 6
                        ? (SDL_Color){32, 32, 32, 255}
                        : (SDL_Color){245, 245, 245, 255};
                    SDL_Color outline = piece < 6
                        ? (SDL_Color){245, 245, 245, 255}
                        : (SDL_Color){32, 32, 32, 255};

                    SDL_SetTextureColorMod(pieces[piece], outline.r, outline.g,
                                           outline.b);
                    for (int i = 0; i < 8; i++) {
                        SDL_Rect outline_target = target;
                        outline_target.x += outline_offsets[i][0];
                        outline_target.y += outline_offsets[i][1];
                        SDL_RenderCopy(renderer, pieces[piece], NULL,
                                       &outline_target);
                    }

                    SDL_SetTextureColorMod(pieces[piece], foreground.r,
                                           foreground.g, foreground.b);
                    SDL_RenderCopy(renderer, pieces[piece], NULL, &target);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    result = EXIT_SUCCESS;

cleanup:
    for (int i = 0; i < 12; i++)
        SDL_DestroyTexture(pieces[i]);
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return result;
}