#include <stdio.h>
#include <stdarg.h>
#include <SDL.h>
#include "utils/msgbox.h"

#ifdef STANDALONE_SERVER

void err_msgbox(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "... Exiting\n");
    va_end(args);
}

#else // STANDALONE_SERVER

char err_msgbox_buffer[1024];
void err_msgbox(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(err_msgbox_buffer, sizeof(err_msgbox_buffer), fmt, args);
    if(SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", err_msgbox_buffer, NULL) != 0) {
        // if the message box failed, fallback to fprintf
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "... Exiting\n");
    }
    va_end(args);
}

#endif // STANDALONE_SERVER
