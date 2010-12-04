#include <stdlib.h>
#include <term.h>
#include <curses.h>
#include <errno.h>

class Termios
{
    struct termios save_term;
    struct termios temp_term;

    void update()
    {
        errno = 0;
        if(tcsetattr(fileno(stdin), TCSANOW, &temp_term) == -1){
            perror("tcsetattr failure");
            exit(EXIT_FAILURE);
        }
    }

public:
    Termios()
    {
        // save term
        errno = 0;
        if(tcgetattr(fileno(stdin), &save_term) == -1){
            perror("tcgetattr failure");
            exit(EXIT_FAILURE);
        }
        temp_term=save_term;
    }

    ~Termios()
    {
        restore();
    }

    void no_canonical()
    {
        temp_term.c_iflag &= IGNCR;
        temp_term.c_oflag &= ONLRET;
        temp_term.c_lflag &= (~ISIG & ~ICANON & ~ECHO);
        temp_term.c_cc[VMIN] = 1;
        temp_term.c_cc[VTIME] = 5;
        update();
    }

    void restore()
    {
        tcsetattr(fileno(stdin), TCSANOW, &save_term);
        temp_term=save_term;
    }

    void echo(bool enable)
    {
        if(enable){
            temp_term.c_lflag |= ECHO;
        }
        else{
            temp_term.c_lflag &= ~ECHO;
        }
        update();
    }
};


class Terminfo
{
public:
    static void initialize()
    {
        if(setupterm(NULL, fileno(stdout), (int *)0) == ERR){
            fprintf(stderr,"setupterm failure\n");
            exit(EXIT_FAILURE);
        }
        //tcmd("clear");
    }

    static void tcmd(char *cap)
    {
        errno=0;
        char *parmcmd=tparm(tigetstr(cap));
        if(parmcmd==NULL){
            perror("tparm NULL");
            exit(EXIT_FAILURE);
        }
        tputs(parmcmd, 1, putchar);
        fflush(stdout);
    }
};

