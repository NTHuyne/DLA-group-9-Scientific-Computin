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

#define SIZE 500 // size of the square board
#define maxNVirus 10000 // maximum viruses we want
#define w 1.5 // correction parameter w in (1;2) but in default we choose w = 1.5
#define n 1 // exponential n (0.5;2) we change it for various test cases, n = 0, 1, 2


// modeling for a virus point
typedef struct Point
{
    long x;
    long y;
}Point;

// create the board
int Grid[SIZE][SIZE];
int nVirus;

// the food in the area
double c[SIZE][SIZE];

// read the first virus coordinates
Point readFile(){
    FILE* input = fopen(inputFile, "r");
    Point newVirus;
    fscanf(input, "%d", &newVirus.x);
    fscanf(input, "%d", &newVirus.y);
    fclose(input);
    return newVirus;
}

// write the out put
void writeFile(FILE* output){
    
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++)
            fprintf(output, "%.5lf \t", c[i][j]);
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
    
}

// write the first board
void write(){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++)
            printf("%d\t", Grid[i][j]);
        printf("\n");
    }
    printf("\n");
}

// add a possible new virus
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

// add virus
void addVirus(Point newVirus){
    int x = newVirus.x;
    int y = newVirus.y;

    nVirus++;
    Grid[x][y] = isVirus;
    addCandidate(newVirus);
} 

// initilize everything
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

// using SOR iteration method to compute all c[][] matrix
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

// mark the spot to 0 
void eat(){
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            if(Grid[i][j] == isVirus)
                c[i][j] = 0;
}

// calculate possibility of the virus growth
void computeProbability(Point candidate, double Pdeno){
    int x = candidate.x;
    int y = candidate.y;
    double Pnum = pow(c[x][y], n);
    double posibility = Pnum / Pdeno;

    double random = rand()/(double)(RAND_MAX);
    
    if(random < posibility)
        addVirus(candidate);
}

// compute the growth of the virus
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
        computeProbability(allCandidate[i], Pdeno);
}

//loop
void solve(){
    SOR();
    eat();
    growth();
}

int main(){
    init();
    FILE* output = fopen(outputFile, "w");
    while(nVirus < maxNVirus)
        solve();
    writeFile(output);
    fclose(output);
    return 0;
}