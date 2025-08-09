#include <iostream>
#include <memory>
#include <SDL.h>

// --- Constants ---
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

// --- RAII Deleters for SDL Types ---
// This is the modern C++ way to ensure SDL resources are always freed.
struct SdlWindowDeleter {
    void operator()(SDL_Window* w) const {
        if (w) {
            SDL_DestroyWindow(w);
        }
    }
};

struct SdlRendererDeleter {
    void operator()(SDL_Renderer* r) const {
        if (r) {
            SDL_DestroyRenderer(r);
        }
    }
};

// --- Type Aliases for our Unique Pointers ---
using UniqueSdlWindow = std::unique_ptr<SDL_Window, SdlWindowDeleter>;
using UniqueSdlRenderer = std::unique_ptr<SDL_Renderer, SdlRendererDeleter>;


int main(int argc, char* argv[]) {
    // --- 1. Initialize SDL ---
    // The SDL_Init function initializes the SDL library.
    // SDL_INIT_VIDEO is a flag that initializes the video subsystem.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    // --- 2. Create Window ---
    // SDL_CreateWindow creates a window for our application.
    // It returns a pointer to the created window, or NULL on failure.
    UniqueSdlWindow window(SDL_CreateWindow(
        "Software Rasterizer",           // Window title
        SDL_WINDOWPOS_CENTERED,          // Initial x position
        SDL_WINDOWPOS_CENTERED,          // Initial y position
        SCREEN_WIDTH,                    // Width, in pixels
        SCREEN_HEIGHT,                   // Height, in pixels
        SDL_WINDOW_SHOWN                 // Flags
    ));

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // --- 3. Create Renderer ---
    // SDL_CreateRenderer creates a 2D rendering context for a window.
    // We will use this to present our final framebuffer to the screen.
    UniqueSdlRenderer renderer(SDL_CreateRenderer(
        window.get(),                   // The window to create a renderer for
        -1,                             // Index of the rendering driver to initialize (-1 for the first one supporting the flags)
        SDL_RENDERER_ACCELERATED        // Renderer flags
    ));

    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        // The window unique_ptr will handle its own cleanup here automatically.
        SDL_Quit();
        return EXIT_FAILURE;
    }


    // --- 4. Main Application Loop ---
    bool running = true;
    SDL_Event event;

    while (running) {
        // --- Event Polling ---
        // SDL_PollEvent checks for pending events, returns 1 if there is one.
        while (SDL_PollEvent(&event)) {
            // If the user clicks the window's 'X' button or presses a quit shortcut.
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // --- Rendering ---
        // Set the color for drawing operations (for clearing the screen).
        SDL_SetRenderDrawColor(renderer.get(), 20, 20, 30, 255); // A dark blue-gray
        // Clear the entire screen to the currently set draw color.
        SDL_RenderClear(renderer.get());
        
        // (In the future, our code to draw into our software framebuffer
        //  and copy it to a texture will go here)

        // Update the screen with any rendering performed since the last call.
        SDL_RenderPresent(renderer.get());
    }


    // --- 5. Cleanup ---
    // Thanks to our unique_ptr wrappers, the renderer and window are
    // automatically destroyed here when they go out of scope.
    // We only need to call SDL_Quit().
    SDL_Quit();

    return EXIT_SUCCESS;
}