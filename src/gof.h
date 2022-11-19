#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include <sys/time.h>
int sizex,sizey;
int N;
FILE* fout=NULL;
int ocupiedslots=0;
int jx=0,iy=0;
struct cell **grid;
struct cell{
    int dead;
    int aliveneighbours;
};
struct timeval start_parallel, end_parallel;
double cpu_time_used_parallel;   
void allocate_grid();
void deallocate_grid();
void add_cell(int token);
void free_grid();
void calculate_game();
void calculate_game_paralel();
void update_neighbours(int i,int j);

