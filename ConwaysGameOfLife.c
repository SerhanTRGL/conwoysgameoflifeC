#include <stdlib.h>
#include <stdio.h>
//#include <windows.h>
#define BUFFER 1024

void birth(char **space, int x_pos, int y_pos){
    space[y_pos][x_pos] = '#';
}

void death(char **space, int x_pos, int y_pos){
    space[y_pos][x_pos] = '.';
}

int countNeighbours(char **space, int x_pos, int y_pos){
    int neighbours=0;
    if(space[y_pos-1][x_pos-1]=='#'){
        neighbours++;
    }
    if(space[y_pos-1][x_pos]=='#'){
        neighbours++;
    }
    if(space[y_pos-1][x_pos+1]=='#'){
        neighbours++;
    }
    if(space[y_pos][x_pos-1]=='#'){
        neighbours++;
    }
    if(space[y_pos][x_pos+1]=='#'){
        neighbours++;
    }
    if(space[y_pos+1][x_pos-1]=='#'){
        neighbours++;
    }
    if(space[y_pos+1][x_pos]=='#'){
        neighbours++;
    }
    if(space[y_pos+1][x_pos+1]=='#'){
        neighbours++;
    }
    return neighbours;
}

int lineCounter(void){
    int lines=0;
    FILE *savefile;
    char chr;

    savefile = fopen("save.txt", "r");
    chr = getc(savefile);
    while(chr!=EOF){
        if(chr == '\n'){
            lines++;
        }
        chr = getc(savefile);
    }
    if(chr == EOF){
        lines++;
    }
    fclose(savefile);
    return lines;
}

int charCounter(int lines){
    int characters = 0;
    FILE *savefile;
    char chr;
    savefile = fopen("save.txt", "r");
    chr = getc(savefile);
    while(chr!=EOF){
        if((chr=='.')||(chr=='#')){
            characters++;
        }
        chr = getc(savefile);
    }
    characters = characters / lines;
    fclose(savefile);

    return characters;
}


void loadGenerationPopulation(int *generation, int *population){
    FILE *savefile;
    savefile = fopen("save.txt", "r");
    char line[BUFFER];
    int lines = lineCounter();

    while(fgets(line, BUFFER, savefile)!=NULL){
        lines--;
        if(lines==1){
            *generation = atoi(line);
        }
        if(lines==0){
            *population = atoi(line);
        }
    }
}

void loadSave(char **space){
    system("cls");
    FILE *savefile;
    savefile = fopen("save.txt", "r");
    if(savefile!=NULL){
        printf("File opened successfuly!\n");
    }
    else{
        printf("Failed to load file!\n");
        exit(EXIT_FAILURE);
    }

    int lines = lineCounter()-2;
    int charperline = charCounter(lines);

    int i, j;
    i = j = 1;
    char chr = getc(savefile);
    while(chr!=EOF){
        if((chr == '.')||(chr == '#')){
            printf("space[%d][%d] loaded\n", i, j);
            space[i][j] = chr;
            j++;
        }

        if(j%5 == 0){
            system("cls");
            printf("Lines: %d ", lines);
            printf("Characters: %d ", charperline*lines);
            printf("Characters per line: %d\n", charperline);
        }

        if(chr=='\n'){
            j=1;
            i++;
        }
        chr = getc(savefile);
    }

    system("cls");
    printf("Map loaded successfully!\n");

    for(i=1; i<lines; i++){
        for(j=1; j<charperline; j++){
            printf("%c", space[i][j]);
        }
        printf("\n");
    }
    system("cls");
}

int main(int argc, char **argv){
    int generation, population;
    int user_choice;
    int lines, charperline;
    char **space;
    char **space_cmp;

    printf("-Welcome to the Conway's Game of Life!-\n");
    printf("---------------------------------------\n");
    printf("What would you like to do?\n");
    printf("1. Load a save file\n");
    printf("2. Start a new game\n");
    printf("3. Exit\n");
    scanf("%d", &user_choice);
    if(user_choice==1){
        lines = lineCounter()-2;
        charperline = charCounter(lines);

        space = (char **)malloc((lines+2) * sizeof(char *));
        space_cmp = (char **)malloc((lines+2) * sizeof(char *));
        int i;
        for(i=0; i<lines+2; i++){
            space[i] = (char *)malloc((charperline+2) * sizeof(char));
            space_cmp[i] = (char *)malloc((charperline+2) * sizeof(char));
        }

        loadSave(space);
        loadGenerationPopulation(&generation, &population);
    }

    else if(user_choice==2){
        printf("pass\n");
    }

    else if(user_choice==3){
        printf("Understandable, have a nice day!\n");
        exit(EXIT_SUCCESS);
    }

    else{
        printf("Invalid Input\n");
    }

    for(int i=1; i<lines; i++){
        for(int j=1; j<charperline; j++){
            if(space[i][j]=='#'){
                population++;
            }
        }
    }

    while(population!=-1){
        //getchar();
        //system("cls");
        int i, j;
        for(i=1; i<lines; i++){
            for(j=1; j<charperline; j++){
                printf("%c", space[i][j]);
                space_cmp[i][j] = space[i][j];
            }
            printf("\n");
        }

        printf("\nGeneration: %d\n", generation);
        printf("Population: %d\n", population);
        //printf("Press ENTER to proceed to the next generation\n\n");

        int neighbours;
        for(i=1; i<lines; i++){
            for(j=1; j<charperline; j++){
                neighbours = countNeighbours(space_cmp, j, i);
                /*if(neighbours!=0){
                    printf("cell [%d][%d] has %d neighbours %c\n", i, j, neighbours, space[i][j]);
                }*/
                if((neighbours==3)&&(space_cmp[i][j]=='.')){
                    birth(space, j, i);
                    population++;
                }
                else if(((neighbours<=1)||(neighbours>=4))&&(space_cmp[i][j]=='#')){
                    death(space, j, i);
                    population--;
                }
            }
        }
        Sleep(50);
        system("cls");
        generation++;
   }
}
