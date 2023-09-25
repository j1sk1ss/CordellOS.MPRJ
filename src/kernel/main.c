#include <stdint.h>

#include <boot/bootparams.h>

#include "memory/memory.h"

#include "../libs/core/shared/allocator/malloc.h"
#include "../libs/core/shared/file_system/ata.h"
#include "../libs/core/shared/file_system/file_system.h"

#include <io/debug.h>
#include "io/string.h"
#include "io/stdio.h"

#include <hal/hal.h>

#include <arch/i686/irq.h>
#include <arch/i686/io.h>

#include "Keyboard.h"

extern void _init();

char* currentPassword;

void start(BootParams* bootParams, void* partition, DISK* disk, Partition* partirion) {
    _init();                            // global constructors
    mm_init(0x50000);                   // Kernel Load is 0x50000 and kernel size is 0x00010000. Malloc start in 0x50000
    HAL_initialize();
    x86_init_keyboard();

    printf("  _____  ____  ____   ___    ___ ||    ||        ____    ____\r\n");
    printf("_|      ||  || || ||  || || ||   ||    ||       ||  ||  |    \r\n");
    printf("||      ||  || ||_||  || || ||   ||    ||       ||  || ||    \r\n");
    printf("||      ||  || |||    || || ||__ ||    ||       ||  || ||    \r\n");
    printf("||      ||  || || ||  || || ||   ||    ||       ||  || ||    \r\n");
    printf(" |_____  |__|  ||  || ||_|| ||__ ||___ ||___     |__|   |____\r\n");

    printf("\r\n Questo sistema operativo 'e in costruzione. \r\n");

<<<<<<< HEAD
    log_debug("Main.c", "Boot device: [%x]", bootParams->BootDevice);
    log_debug("Main.c", "Disk device: [%x]", disk->id);
    log_debug("Main.c", "Partition device: [%x]", partition);
=======
    log_debug("Main.c", "Boot device: %x", bootParams->BootDevice);

    char *data1 = malloc(512);
    strcpy(data1, "The content of the first file on 539filesystem");
    create_file("first_file", data1);
>>>>>>> 1043c8eb9a5869425647fbe15dbee89f49fddfdb

    printf("Content: %s\n", read_file("first_file"));

    // currentPassword = (char*)malloc(6);
    // strcpy(currentPassword, "12345\0");

    // while (1) {
    //     printf("\r\n[CORDELL OS]: ");

    //     char* command = keyboard_read(1);
    //     execute_command(command);
            
    //     free(command);
    // }

end:
    for (;;);
}

void execute_command(char* command) {
    if (strstr(command, "help") == 0) {
        printf("\r\n> Use help for getting help");
        printf("\r\n> Use clear for screen cleaning");
        printf("\r\n> Use echo for echo");
        printf("\r\n> Use setpas for setting password for cordell");
        printf("\r\n> Use cordell for using cordell commands");
    }
    else if (strstr(command, "clear") == 0) 
        VGA_clrscr();
    else if (strstr(command, "echo") == 0) {
        char* echo = command + strlen("echo") + 1; 
        printf("\r\n%s", echo);
    }
    else if (strstr(command, "cordell") == 0) {
        char* cordellCommand = command + strlen("cordell") + 1; 

        printf("\r\n[PASSWORD]: ");
        char* password = keyboard_read(0);
        while (strcmp(password, currentPassword) != 0) {
            printf("\r\nIncorrect password, try again.\r\n[PASSWORD]: ");
            free(password);

            password = keyboard_read(0);
        }

        free(password);

        if (strstr(cordellCommand, "setpas") == 0) {
            char* newPassword = cordellCommand + strlen("setpas") + 1; 
            free(currentPassword);

            char* buffer = (char*)malloc(strlen(newPassword) + 1);
            memset(buffer, 0, sizeof(buffer));
            if (buffer == NULL)
                return;

            strcpy(buffer, newPassword);

            currentPassword                             = buffer;
            currentPassword[strlen(currentPassword)]    = '\0';
        }
    } else 
        printf("\r\nUnknown command. Maybe you forgot CORDELL?");
            
    printf("\r\n");
}