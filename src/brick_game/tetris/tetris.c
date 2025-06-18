#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_HEIGHT 30
#define WINDOW_WIDTH 50
#define WINDOW_POSITION_X 20
#define WINDOW_POSITION_Y 5

typedef struct {
    int x;
    int y;
} coord;

void init(void);
void game(void);
short **makeMtrx(int row, int col);
void printMtrx(WINDOW *win, short **mtrx, int row, int col);
void fillingMtrx(short **mtrx, coord *coord, size_t count_elem);
void figure_down(coord *coord, size_t count_elem);
short figureMove(short **mtrx, int row, int col, coord *coord, size_t count_elem);

int main(void) {
    game();
    return 0;
}

void game(void) {
    init();
    short **mtrx = makeMtrx(WINDOW_HEIGHT, WINDOW_WIDTH);
    short flag_figure = 0; // фигура отсуствует
    size_t count_elem;
    coord *coords = NULL;
    while (1) {
        if (!flag_figure) {
            count_elem = 4;
            coords = (coord *)malloc(count_elem * sizeof(coord));
            coords[0].x = 1;
            coords[0].y = 0;
            coords[1].x = 0;
            coords[1].y = 0;
            coords[2].x = 0;
            coords[2].y = 1;
            coords[3].x = 0;
            coords[3].y = 2;
        } else {
            if (figureMove(mtrx, WINDOW_HEIGHT, WINDOW_WIDTH, coords, count_elem)) {
                for (int i = 0; i < count_elem; i++) {
                    coords[i].y++;
                }
            } else {
                continue;
            }
        }
        fillingMtrx(mtrx, coords, count_elem);
        refresh();
        WINDOW *nw = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_POSITION_Y, WINDOW_POSITION_X);
        box(nw, 0, 0);
        printMtrx(nw, mtrx, WINDOW_HEIGHT, WINDOW_WIDTH);
        wrefresh(nw);
        getch();
        delwin(nw);
        figure_down(coords, count_elem);
    }
    free(mtrx);
    mtrx = NULL;
    endwin();
}

short figureMove(short **mtrx, int row, int col, coord *coord, size_t count_elem) {
    short res = 1; // фигура движется
    for (int i = 0; i < count_elem; i++) {
        if (mtrx[coord[i].y][coord[i].x - 1] || coord[i].y == row - 1) {
            res = 0;
            break;
        }
    }
    return res;
}

void figure_down(coord *coord, size_t count_elem) {
    for (int i = 0; i < count_elem; i++) {
        coord[i].y++;
    }
}

void fillingMtrx(short **mtrx, coord *coord, size_t count_elem) {
    for (int i = 0; i < count_elem; i++) {
        mtrx[coord[i].y - 1][coord[i].x - 1] = 1;
    }
}

void printMtrx(WINDOW *win, short **mtrx, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (mtrx[i][j]) {
                mvwprintw(win, i, j, "#");
            }
        }
    }
}

short **makeMtrx(int row, int col) {
    short *dtMtrx = (short *)calloc(row * col, sizeof(short));
    short **mtrx = (short **)calloc(row, sizeof(short *));
    for (int i = 0; i < row; i++) {
        mtrx[i] = dtMtrx + i * col;
    }
    return mtrx;
}

void init(void) {
    initscr();
    cbreak();
    noecho();
    halfdelay(750);
    curs_set(0);
}