#include <io/common-io.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

size_t menu(const char* prompt, size_t choices_count, ...)
{   
    va_list choices;
    size_t choice;

    puts(prompt);
    va_start(choices, choices_count);
    for (size_t i = 0; i < choices_count; i++)
    {
        printf("\t(%zu) %s\n", i + 1, va_arg(choices, const char*));
    }
    va_end(choices);

    printf("insert a number in range 1-%zu: ", choices_count);
    scanf("%zu", &choice);
    getchar();
    return (choice >= 1 && choice <= choices_count) ? choice : 0;
}

void any_key()
{
    puts("[press any key to continue...]");
    getchar();
}