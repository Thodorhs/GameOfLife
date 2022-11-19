#include "gof.h"
void allocate_grid(){
    int i;
    grid = malloc(sizeof(struct cell*)*sizey);
    for(i=0;i<sizey;i++){
        grid[i] = malloc(sizeof(struct cell)*sizex);
    }
}
void add_cell(int token){
//add cell to grid in the first available position
    struct cell temp;
    if(jx==sizex&&iy==sizey){
        printf("Error: grid is full\n");
        return;
    }
    if(token==32){
        temp.dead=1;
        temp.aliveneighbours=0;
        grid[iy][jx]=temp;
    }else{
        temp.dead=0;
        temp.aliveneighbours=0;
        grid[iy][jx]=temp;
    }
    if(jx==sizex-1){
        jx=0;
        iy++;
    }else{
        jx++;
    }    
    return;
            
            
        
}
//function to print grid to fout
void print_grid(){
    int i,j;
    for(i=0;i<sizey;i++){
        fprintf(fout,"|");
        for(j=0;j<sizex;j++){
            if(grid[i][j].dead==1){
                fprintf(fout," |");
            }
            else if(grid[i][j].dead==0){
                fprintf(fout,"*|");
            }
        }
        fprintf(fout,"\n");
    }
}
void free_grid(){
//free grid
    int i;
    for(i=0;i<sizey;i++){
        free(grid[i]);
    }
    free(grid);
}
void update_neighbours(int i,int j){
//update the deadneighbours of cell i,j
    grid[i][j].aliveneighbours=0;
    if(i>0&&j>0&&grid[i-1][j-1].dead==0){
        grid[i][j].aliveneighbours++;
    }
    if(i>0&&grid[i-1][j].dead==0){
        grid[i][j].aliveneighbours++;
    }
    if(i>0&&j<sizex-1&&grid[i-1][j+1].dead==0){
        grid[i][j].aliveneighbours++;
    }
    if(j>0&&grid[i][j-1].dead==0){
        grid[i][j].aliveneighbours++;
    }
    if(j<sizex-1&&grid[i][j+1].dead==0){
        grid[i][j].aliveneighbours++;
    }
    if(i<sizey-1&&j>0&&grid[i+1][j-1].dead==0){
        grid[i][j].aliveneighbours++;
    }
    if(i<sizey-1&&grid[i+1][j].dead==0){
        grid[i][j].aliveneighbours++;
    }
    if(i<sizey-1&&j<sizex-1&&grid[i+1][j+1].dead==0){
        grid[i][j].aliveneighbours++;
    }  
}
void calculate_game(){
    int days=0;
    int i,j;
    while(days!=N){
        gettimeofday(&start_parallel, NULL);
        #pragma omp parallel for private(i) shared(grid)
            for(i=0;i<sizey;i++){
            #pragma omp parallel for private(j) shared(grid)
                for(j=0;j<sizex;j++){
                    update_neighbours(i,j);
                    if(grid[i][j].dead==0){
                        if(grid[i][j].aliveneighbours!=2&&grid[i][j].aliveneighbours!=3){
                            grid[i][j].dead=1;
                        }
                    }else{
                        if(grid[i][j].aliveneighbours==3){
                            grid[i][j].dead=0;
                        }
                    }
                    
                }
           
            }
        gettimeofday(&end_parallel, NULL);
        cpu_time_used_parallel = cpu_time_used_parallel+(double)((end_parallel.tv_sec - start_parallel.tv_sec) * 1000000u + end_parallel.tv_usec - start_parallel.tv_usec);
        days++;
    }
    printf("Parallel time: %lf ...(if -fopenmp is used)\n",cpu_time_used_parallel/ CLOCKS_PER_SEC);
}
int main(int argc, char **argv){
    if(argc!=4){
        printf("Arguments need to be 3\n");
        return 1;
    }
    FILE *f1 = fopen(argv[1], "r");
    N=atoi(argv[2]);
    if(strcmp(argv[3],"stdout")==0){
        fout=stdout;
    }else{
    fout = fopen(argv[3], "w");
        if(fout==NULL)
        {
            printf("Error opening file 2\n");
            return 1;
        }
    }
    //check fopen errors
    if(f1==NULL)
    {
        printf("Error opening file 1\n");
        return 1;
    }
    //compute size of array from file
    char *line = NULL;
    size_t len = 0;
    int i=0;
    if(getline(&line, &len, f1)!=-1){
        while(line[i]!='\n'){
            if(line[i]==' '){
                line[i]='\0';
                sizex=atoi(line);
                i++;
                break;
            }
            i++;
        }
    sizey=atoi(line);        
    }
    int token;
    allocate_grid();
    while((token=fgetc(f1))!=EOF){
        if(token==' '||token=='*'){
            add_cell(token);
        }
    }
    calculate_game();
    print_grid();
    free_grid();
    return 0;
}