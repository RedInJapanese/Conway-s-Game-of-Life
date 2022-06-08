#include "universe.h"
#include <stdlib.h>

uint32_t previous(uint32_t n, uint32_t i);
uint32_t next(uint32_t n, uint32_t i);
//line 26-35; obtained from Eugene Chou(Section)
//line 77-89; obtained from Eugene Chou(edstem.org/us/courses/16730/discussion/1065819)
//line 149-155; obtained from Eugene Chou(Section)
struct Universe { //declaration of struct
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols,
    bool toroidal) { //constructor that allocates memory to the Universe and its variables
    Universe *x = (Universe *) malloc(sizeof(Universe));
    x->rows = rows;
    x->cols = cols;
    x->toroidal = toroidal;
    x->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i++) {
        x->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }
    return x;
}

void uv_delete(Universe *
        u) { //frees memory in the Universe by looping through the columns and freeing them, then freeing the rows and pointer
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

uint32_t uv_rows(Universe *u) { //returns the rows from the universe
    return u->rows;
}

uint32_t uv_cols(Universe *u) { // returns the columns from the universe
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r,
    uint32_t
        c) { // determines if the current cell is alive or not by checking if its out of bounds or not
    uint32_t neg = -1;
    if (r >= u->rows || c >= u->cols || r == neg || c == neg) {
        ;
    } else {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, uint32_t r,
    uint32_t
        c) { // determines if the current cell is dead or not by checking if its out of bounds or not
    uint32_t neg = -1;
    if (r >= u->rows || c >= u->cols || r == neg || c == neg) {
        ;
    } else {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, uint32_t r,
    uint32_t c) { //returns the status of the cell by checking if its out of bounds or not
    uint32_t neg = -1;
    if (r >= u->rows || c >= u->cols || r == neg || c == neg) {
        return false;
    }
    return u->grid[r][c];
}

bool uv_populate(Universe *u,
    FILE *
        infile) { //takes the coordinates of the live cells by looping through the file till it reaches the EOF constant then takes the file coordinates accordingly
    uint32_t ro, col;
    uint32_t neg = -1;
    //fscanf(infile, "%d %d\n", &ro, &col);
    //fscanf(infile, "%d %d\n", &ro, &col);
    while (fscanf(infile, "%d %d\n", &ro, &col) != EOF) {
        if (ro >= u->rows || col >= u->cols || ro == neg || col == neg) {
            return false;
        } else {
            //fscanf(infile, "%d %d\n", &ro, &col);
            uv_live_cell(u, ro, col);
        }
        //fscanf(infile, "%d %d\n", &ro, &col);
    }
    return true;
}
uint32_t uv_census(Universe *u, uint32_t r,
    uint32_t c) { //uses an integer to return the number of adjacent variables
    int live_counter = 0;
    if (u->toroidal == true) { // runs and checks all 8 adjacent points if the data is toroidal
        if (u->grid[r][previous(u->cols, c)] == true) {
            live_counter++;
        }
        if (u->grid[r][next(u->cols, c)] == true) {
            live_counter++;
        }
        if (u->grid[previous(u->rows, r)][c] == true) {
            live_counter++;
        }
        if (u->grid[next(u->rows, r)][c] == true) {
            live_counter++;
        }
        if (u->grid[previous(u->rows, r)][previous(u->cols, c)] == true) {
            live_counter++;
        }
        if (u->grid[previous(u->rows, r)][next(u->cols, c)] == true) {
            live_counter++;
        }
        if (u->grid[next(u->rows, r)][previous(u->cols, c)] == true) {
            live_counter++;
        }
        if (u->grid[next(u->rows, r)][next(u->cols, c)] == true) {
            live_counter++;
        }

    } else { //toroidal is false and uses get cell to check which cells aren't out of bounds and which ones are alive
        if (uv_get_cell(u, r, c - 1) == true) {
            live_counter++;
        }
        if (uv_get_cell(u, r, c + 1) == true) {
            live_counter++;
        }
        if (uv_get_cell(u, r - 1, c) == true) {
            live_counter++;
        }
        if (uv_get_cell(u, r + 1, c) == true) {
            live_counter++;
        }
        if (uv_get_cell(u, r - 1, c - 1) == true) {
            live_counter++;
        }
        if (uv_get_cell(u, r - 1, c + 1) == true) {
            live_counter++;
        }
        if (uv_get_cell(u, r + 1, c - 1) == true) {
            live_counter++;
        }
        if (uv_get_cell(u, r + 1, c + 1) == true) {
            live_counter++;
        }
    }
    return live_counter;
}
uint32_t previous(
    uint32_t n, uint32_t i) { //returns the previous element of the array(for toroidal)
    return (i + n - 1) % n;
}

uint32_t next(uint32_t n, uint32_t i) {
    return (i + 1) % n; //returns the next element of the array(toroidal)
}

void uv_print(Universe *u,
    FILE *
        outfile) { //loops through the rows and columns and prints a '.' for dead cells and 'o' for live cells
    for (uint32_t i = 0; i < u->rows; i++) {
        for (uint32_t x = 0; x < u->cols; x++) {
            if (u->grid[i][x] == false) {
                fprintf(outfile, ".");
            } else if (u->grid[i][x] == true) {
                fprintf(outfile, "o");
            }
        }
        printf("\n");
    }
}
