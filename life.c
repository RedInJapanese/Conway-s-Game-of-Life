#include <inttypes.h>
#include "universe.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define OPTIONS "tsn:i:o:" //obtained from collatz.c(Assignment 1)
#define DELAY   50000
//lines 40-49; obtained from sorting.c(Assignment 3)
//line 45 and 48; obtained from Atomic#2440; https://discord.com/channels/@me/930638168804450364/938517418026885130
struct Universe { //declaration of struct
    uint32_t rows, cols;
    bool **grid;
    bool toroidal;
};

void swap_uv(Universe *x, Universe *y); //derived from stats.c(Assignment 3)

int main(int argc, char **argv) {
    int opt = 0;
    Universe *A,
        *B; // declaration of two universes as pointers. Makes it possible to free memory with no leaks
    FILE *infile = NULL;
    FILE *outfile = NULL; // declaration of two file pointers
    char *s = "";
    char *o
        = ""; //declaration of two pointers used to take input for the two files from the command line
    uint32_t rows, columns;
    uint32_t x, y;
    uint32_t a,
        b; //declaration of variables used for the rows, columns, and variables for stdin files
    uint32_t generations
        = 100; // generations being used when populating the universes. Can change with command line
    bool toroidal = false; //toroidal set to false by default
    int silence = 0; //variable to turn ncurses on and off
    uint32_t neg = -1; //negative variable for checking bounds
    rows = 0, columns = 0;
    x = 0, y = 0;
    a = 0, b = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        default:
            infile = stdin;
            outfile = stdout;
            return EXIT_FAILURE; //command line options using a switch statement
        case 'i': s = optarg; break;
        case 't': toroidal = true; break;
        case 's': silence = 1; break;
        case 'n': generations = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'o': o = optarg; break;
        }
    }
    //conditionals used to check if the files are stdin/stdout or user input
    if (strcmp(s, "") == 0) {
        infile = stdin;
    }
    if (strcmp(o, "") == 0) {
        outfile = stdout;
    }
    if (infile == stdin && strcmp(s, "") == 0) { //takes input from stdin
        fscanf(infile, "%d %d\n", &rows, &columns);
        while (fscanf(infile, "%d %d\n", &x, &y) != EOF) {
            //fscanf(infile, "%d %d\n", &x , &y);
            if (x == a && y == b) {
                puts("Malformed input.");
                break;
            }
            if (x >= rows || y >= columns || x == neg || y == neg) {
                puts("Malformed input.");
                break;
            }
            a = x, b = y;
        }

    } else if (infile != stdin && (infile = fopen(s, "r"))) {
        infile = fopen(s, "r");
        fscanf(infile, "%d %d\n", &rows, &columns);

    } else if (infile != stdin && !(infile = fopen(s, "r"))) {
        printf("Error opening %s.\n", s);
    }
    if (outfile == stdout && strcmp(o, "") == 0) {
        ;
    } else if (outfile != stdout && (outfile = fopen(o, "w"))) {
        outfile = fopen(o, "w");
    } else if (outfile != stdout && (outfile = fopen(o, "w"))) {
        printf("Error opening %s.\n", o);
    }
    A = uv_create(rows, columns, toroidal); //constructor for universes
    B = uv_create(rows, columns, toroidal);
    //printf("rows %d\n", rows);
    //printf("columns %d\n", columns);
    uv_populate(A, infile); //populates A
    if (silence
        == 0) { //conditional for when ncurses is on in which 3 for loops are used to draw the cells and take a census each time
        initscr();
        curs_set(FALSE);
        for (uint32_t i = 0; i < generations; i++) {
            clear();
            for (uint32_t x = 0; x < rows; x++) {
                for (uint32_t y = 0; y < columns; y++) {
                    if (uv_get_cell(A, x, y) == true) {
                        mvprintw(x, y, "o");
                        if (uv_census(A, x, y) == 2
                            || uv_census(A, x, y) == 3) { //checks using game of life rules
                            uv_live_cell(B, x, y);
                        } else {
                            uv_dead_cell(B, x, y);
                        }

                    } else {
                        if (uv_census(A, x, y) == 3) { //checks dead cells using game of life rules
                            uv_live_cell(B, x, y);
                        } else {
                            uv_dead_cell(B, x, y);
                        }
                    }
                }
            }
            refresh();
            usleep(DELAY);
            swap_uv(A, B); //swapping function
        }
        endwin();
        uv_print(A, outfile);
        fclose(infile);
        fclose(outfile);
        uv_delete(A);
        uv_delete(B); //frees memory
    } else { //functions in the same way as the last conditional, but doesn't use ncurses
        for (uint32_t i = 0; i < generations; i++) {
            for (uint32_t x = 0; x < rows; x++) {
                for (uint32_t y = 0; y < columns; y++) {
                    if (uv_get_cell(A, x, y) == true) {
                        if (uv_census(A, x, y) == 2 || uv_census(A, x, y) == 3) {
                            uv_live_cell(B, x, y);
                        } else {
                            uv_dead_cell(B, x, y);
                        }
                    } else {
                        if (uv_census(A, x, y) == 3) {
                            uv_live_cell(B, x, y);
                        } else {
                            uv_dead_cell(B, x, y);
                        }
                    }
                }
            }
            swap_uv(A, B);
        }
        uv_print(A, outfile);
        fclose(infile);
        fclose(outfile);
        uv_delete(A);
        uv_delete(B);
    }
}
void swap_uv(Universe *x, Universe *y) { //swap method that is pass by reference
    Universe t = *x;
    *x = *y;
    *y = t;
}
