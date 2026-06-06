#ifndef CONIO_COMPAT_H
#define CONIO_COMPAT_H

// Cross-platform shim for the bits of <conio.h> this project uses.
// On Windows it just forwards to the real <conio.h>.
// On macOS/Linux it implements _getch() via termios and maps the
// "cls" clear-screen call to the POSIX "clear" command.

#ifdef _WIN32
    #include <conio.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <termios.h>
    #include <unistd.h>

    // Read a single character without waiting for Enter and without echo.
    static inline int _getch() {
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }

    #define CLEAR_SCREEN "clear"
#endif

#endif // CONIO_COMPAT_H
