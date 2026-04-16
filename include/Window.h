#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <gl\glu.h>
#include <string>
#include <vector>
#include <iostream>
#include "Settings.h"
class Timer {
private:
    bool started;       ///< Indicates whether the timer has started.
    bool paused;        ///< Indicates whether the timer is currently paused.
    Uint32 startTicks;  ///< Ticks counted when the timer was started.
    Uint32 pausedTicks; ///< Ticks counted while the timer was paused.

public:
    /**
     * @brief Constructs a new Timer object and initializes its state.
     */
    Timer();

    /**
     * @brief Starts the timer.
     */
    void start();

    /**
     * @brief Stops the timer and resets its counters.
     */
    void stop();

    /**
     * @brief Pauses the timer.
     */
    void pause();

    /**
     * @brief Resumes the timer if it was paused.
     */
    void unpause();

    /**
     * @brief Checks if the timer is currently paused.
     * @return true if paused, false otherwise.
     */
    bool isPaused();

    /**
     * @brief Checks if the timer is currently running.
     * @return true if started.
     */
    bool isStarted();

    /**
     * @brief Returns the number of ticks elapsed since the timer started.
     * @return Elapsed ticks as Uint32.
     */
    Uint32 getTicks();
};




/**
 * @brief Main window class responsible for SDL window and renderer handling.
 */
class Window {

public:
    int width;                     ///< Window width in pixels.
    int height;                    ///< Window height in pixels.
    SDL_Window* gWindow;           ///< SDL window pointer.
    bool quit;                     ///< Application quit flag.
    /**
     * @brief Closes the window and sets the quit flag.
     */
    void quitWindow();

    /**
     * @brief Constructs and initializes a window with the given width and height.
     * @param w Window width.
     * @param h Window height.
     */
    Window(Settings const& settings);

    /**
     * @brief Destroys the window and releases SDL resources.
     */
    ~Window();
};

/**
 * @brief Initializes SDL, the window, renderer, and other subsystems.
 * @param window Pointer to the Window object to initialize.
 * @return true on successful initialization, false otherwise.
 */
bool init(Window* window);
#endif