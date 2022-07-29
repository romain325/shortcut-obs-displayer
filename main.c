#include <stdio.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <libevdev-1.0/libevdev/libevdev.h>

#define KEY_RELEASE 0
#define KEY_PRESS 1
#define KEY_REPEAT 2

typedef int Shortcut[5];

// listen to ../event8 (hardcoded for now on)


//input events:
// code = key pressed
// value = press || release

int qwerty_to_azerty(int code){
    switch (code) {
        case KEY_Q:
            return KEY_A;
        case KEY_W:
            return KEY_Z;
        case KEY_A:
            return KEY_Q;
        case KEY_Z:
            return KEY_W;
        case KEY_M:
            return KEY_QUESTION;
        case KEY_SEMICOLON:
            return KEY_M;
        case KEY_COMMA:
            return KEY_DOT;
        case KEY_DOT:
            return KEY_SLASH;
        case KEY_SLASH:
            return KEY_DOT;
        default:
            return code;
    }
}

void check_for_root() {
    struct passwd *p = getpwuid(getuid());
    if(strcmp(p->pw_name, "root") != 0){
        fprintf(stderr, "you must be root to execute this program as it reads /dev/input files");
        exit(1);
    }
}

void reinit_shortcut(Shortcut shortcut){
    for(int i = 0; i< 5; i++){
        shortcut[i] = -1;
    }
}

void print_shortcut(Shortcut shortcut) {
    for(int i = 0; i< 5; i++){
        printf("%d ", shortcut[i]);
    }
    printf("\n");
}

int check_first_key_release(Shortcut shortcut, struct input_event new_key){
    return shortcut[0] == new_key.code && new_key.value == KEY_RELEASE;
}

int check_not_ascii_char(Shortcut shortcut, struct input_event inputEvent) {
    if( shortcut[0] == KEY_LEFTSHIFT || shortcut[0] == KEY_RIGHTSHIFT ||
        shortcut[0] == KEY_LEFTALT || shortcut[0] == KEY_RIGHTALT) {
        for (int i = KEY_1; i <= KEY_L; i++) {
            if(inputEvent.code == i) return 0;
        }
        for (int i = KEY_Z; i <= KEY_M; i++) {
            if(inputEvent.code == i) return 0;
        }
    }
    return 1;
}

int add_new_key(Shortcut shortcut, struct input_event new_key) {
    for(int i = 0; i < 5; i++) {
        if(shortcut[i] == -1) {
            if(i == 1 && !check_not_ascii_char(shortcut, new_key)){
                reinit_shortcut(shortcut);
            }

            shortcut[i] = new_key.code;
            return 1;
        }
    }
    return 0;
}

int is_detecting(Shortcut shortcut) {
    return shortcut[0] != -1;
}

void save_shortcut(Shortcut shortcut){
    //log
    print_shortcut(shortcut);

    // maybe keep open ???
    FILE* f;
    char mess[2048] = "";
    int cnt = 0;
    if((f = fopen("last_shortcut.txt", "w")) == NULL){
        perror("file");
        exit(1);
    }

    for(int i = 0; i < 5; i++) {
        if(shortcut[i] != -1) {
            if(cnt > 0) {
                sprintf(mess, "%s + ", mess);
            }
            sprintf(mess, "%s%s", mess, libevdev_event_code_get_name(EV_KEY, shortcut[i]));
            cnt++;
        }
    }
    if(cnt > 1){
        printf("%s\n", mess);
        fprintf(f, "%s", mess);
    }
    fclose(f);
}



int is_code_in(Shortcut shortcut, struct input_event inputEvent) {
    for(int i = 0; i < 5; i++) {
        if(shortcut[i] == inputEvent.code) {
            return 1;
        }
    }
    return 0;
}

void remove_key(Shortcut shortcut, struct input_event inputEvent){
    for(int i = 0; i <5; i++){
        if(shortcut[i] == inputEvent.code) {
            shortcut[i] = -1;
            return;
        }
    }
}


int main() {
    check_for_root();

    struct input_event inputEvent;
    Shortcut shortcut;

    int fd;
    struct pollfd pfd;

    reinit_shortcut(shortcut);
    fd = open("/dev/input/event8", O_RDWR | O_NONBLOCK);

    if( fd == -1 )
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pfd.fd = fd;
    pfd.events = POLLIN ;

    while( 1 ) {
        if (poll(&pfd, (unsigned long) 1, 5000) < 0) {
            perror("poll");
            return 0;
        }

        if ((pfd.revents & POLLIN) == POLLIN) {
            if (read(pfd.fd, &inputEvent, sizeof(inputEvent)) <= 0) {
                perror("read");
                return 0;
            }

            //check if keyboard input && not repeat key
            if (inputEvent.type != EV_KEY || inputEvent.value == KEY_REPEAT) continue;

            //log
            /*char *message;
            if (inputEvent.value == KEY_PRESS) message = "Press: %s\n";
            else message = "Release: %s\n";
            printf(message, libevdev_event_code_get_name(EV_KEY, inputEvent.code));*/

            int detecting = is_detecting(shortcut);
            int is_release = inputEvent.value == KEY_RELEASE;

            if (!is_release) {
                if ((!detecting && (inputEvent.code == KEY_LEFTCTRL || inputEvent.code == KEY_RIGHTCTRL ||
                                    inputEvent.code == KEY_RIGHTALT || inputEvent.code == KEY_LEFTALT ||
                                    inputEvent.code == KEY_LEFTSHIFT || inputEvent.code == KEY_RIGHTSHIFT))
                    || detecting) {
                    if (!is_code_in(shortcut, inputEvent))
                        add_new_key(shortcut, inputEvent);
                }
            } else {
                if (detecting && check_first_key_release(shortcut, inputEvent)) {
                    save_shortcut(shortcut);
                    reinit_shortcut(shortcut);
                }
            }
        }
    }
}
