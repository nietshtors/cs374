#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *string_dup(char *source)
{
    char *s = malloc(sizeof *source);
    strcpy(s, source);
    return s;
}

void string_free(char *copy)
{
    free(copy);   // IMPLEMENT ME
}

int main(void)
{
    char *s;

    s = string_dup("hello, world!");
    s[0] = 'H';  // modify duplicate
    printf("Duplicate is: \"%s\"\n", s);
    string_free(s);

    s = string_dup("Another test.");
    s[12] = '!';
    printf("Duplicate is: \"%s\"\n", s);
    string_free(s);
}