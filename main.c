#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define inputFile "input.txt"
#define outputFile "output.txt"
#define isFree 0
#define isVirus 1
#define isCadidate 2
#define defaultFood 1

#define SIZE 500
#define maxNVirus 10000
#define w 1.5
#define n 2

typedef struct Point
{
    long x;
    long y;
}Point;

int Grid[SIZE][SIZE];
int nVirus;
double c[SIZE][SIZE];

Point readFile(){
    FILE* input = fopen(inputFile, "r");
    Point newVirus;
    fscanf(input, "%d", &newVirus.x);
    fscanf(input, "%d", &newVirus.y);
    fclose(input);
    return newVirus;
}

void writeFile(FILE* output){
    
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++)
            fprintf(output, "%.5lf \t", c[i][j]);
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
    
}

void write(){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++)
            printf("%d\t", Grid[i][j]);
        printf("\n");
    }
    printf("\n");
}

void addCandidate(Point newVirus){
    int x = newVirus.x;
    int y = newVirus.y;

    if(x - 1 >= 0 && Grid[x-1][y] != isVirus)
        Grid[x-1][y] = isCadidate;

    if(x + 1 < SIZE && Grid[x+1][y] != isVirus)
        Grid[x+1][y] = isCadidate;

    if(y - 1 >= 0 && Grid[x][y-1] != isVirus)
        Grid[x][y-1] = isCadidate;

    if(y + 1 < SIZE && Grid[x][y+1] != isVirus)
        Grid[x][y+1] = isCadidate;

}

void addVirus(Point newVirus){
    int x = newVirus.x;
    int y = newVirus.y;

    nVirus++;
    Grid[x][y] = isVirus;
    addCandidate(newVirus);
} 

void init(){
    nVirus = 0;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            Grid[i][j] = isFree;

    Point firstVirus = readFile();
    addVirus(firstVirus);

    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            c[i][j] = defaultFood;

    c[firstVirus.x][firstVirus.y] = 0;
    srand(time(NULL));
}

void SOR(){
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++){
            if(i == 0 || j == 0 || i+1 == SIZE || j+1 == SIZE)
                c[i][j] = defaultFood;
            else{
                c[i][j] = w/4 * (c[i+1][j] + c[i][j+1] + c[i-1][j] + c[i][j-1]) + (1-w) * c[i][j];
            }
        }
}

void eat(){
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            if(Grid[i][j] == isVirus)
                c[i][j] = 0;
}

void RandomGrowth(Point candidate, double Pdeno){
    int x = candidate.x;
    int y = candidate.y;
    double Pnum = pow(c[x][y], n);
    double posibility = Pnum / Pdeno;

    double random = rand()/(double)(RAND_MAX);
    
    if(random < posibility)
        addVirus(candidate);
}

void growth(){
    Point allCandidate[SIZE * SIZE];
    double Pdeno = 0;
    int count = 0;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++){
            if(Grid[i][j] == isCadidate){
                Point new = {i, j};
                allCandidate[count] = new;
                count++;
                Pdeno += pow(c[i][j], n);
            }
        }
    for(int i = 0; i < count; i++)
        RandomGrowth(allCandidate[i], Pdeno);
}

void update(){
    SOR();
    eat();
    growth();
}

int main(){
    init();
    FILE* output = fopen(outputFile, "w");
    while(nVirus < maxNVirus)
        update();
    writeFile(output);
    fclose(output);
    return 0;
}