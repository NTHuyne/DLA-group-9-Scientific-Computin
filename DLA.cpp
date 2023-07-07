#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cstdlib>
using namespace std;
#define N 256
#define OMEGA 1.9
#define TOLERANCE 0.1
#define n  2  // n nam trong khoang (0.5-2)

struct VirusPlace
{
    long x;
    long y;
    double prob;
};

// chua danh sach cac virus da xuat hien
// khoi tao voi 1 virus duy nhat
vector<VirusPlace> virus;

// chua so virus hien co
int nVirus;

// chua danh sach cac UCV co the lay nhiem virus trong buoc hien tai
vector<VirusPlace> candidate;

// mang c bieu dien thuc an con lai trong khu vuc, dc khoi tao = 1
// tru o duy nhat chua virus dau tien
double c[N][N];

// xac suat cua cac UCV, so luong = nCandidate
// vector<double> chance;

// danh dau vi tri cua cac virus, candidate
int grow[N][N] = {0};


void addCandidate(int u, int v)
{
    VirusPlace newCandidate;
    newCandidate.x = u;
    newCandidate.y = v;
    // check if the cell is virus or candidate before.
    if (grow[u][v] == 1)
        return;
    if (grow[u][v] == 2)
        return;
    candidate.push_back(newCandidate);
    grow[u][v] = 2;
    // cout << "add " << u << "-" << v << endl;
}

void addVirus(int u, int v)
{
    // add a new virus to the list of virus
    VirusPlace newVirus;
    newVirus.x = u;
    newVirus.y = v;
    newVirus.prob = 1;
    virus.push_back(newVirus);
    grow[u][v] = 1;

    // remove candidate to virus list
    for (int i = 0; i < candidate.size(); i++)
        if (candidate[i].x == u && candidate[i].y == v)
            candidate.erase(candidate.begin() + i);

    // update new candidate
    VirusPlace newCandidate;

    // check condition if the neighbor of newvirus is in the matrix
    if (u - 1 >= 0)
        addCandidate(u - 1, v);
    if (v - 1 >= 0)
        addCandidate(u, v - 1);
    if (u + 1 < N)
        addCandidate(u + 1, v);
    if (v + 1 < N)
        addCandidate(u, v + 1);
}

void initialize()
{
    for (int i = 1; i < N - 1; i++)
        for (int j = 1; j < N - 1; j++)
            c[i][j] = 1;

    // khoi tao gia tri bien cua ma tran
    for (int i = 0; i < N; i++)
    {
        c[0][i] = 1;
        c[N - 1][i] = 0;
    }
    for (int i = 0; i < N; i++)
    {
        c[i][0] = 1.0f - ((double)i / (N - 1));
        c[i][N - 1] = 1.0f - ((double)i / (N - 1));
    }
    // biến ô ở đáy thành virus
    addVirus(N - 2, N / 2);
}

// tinh nong do ma tran C - calculate concentration of food of matrix C
void sor()
{
    double tmp[N][N];
    double delta = 0;
    double max_delta = 0;
    // luu ma tran hien tai vao tmp, ma tran o buoc lap k
    do
    {
        max_delta = 0;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                tmp[i][j] = c[i][j];

        // tinh lai voi ma tran moi o buoc lap k + 1
        for (int i = 1; i < N - 1; i++)
        {
            for (int j = 1; j < N - 1; j++)
            {
                if ((i + j) % 2 == 0)
                {
                    c[i][j] = OMEGA / 4 * (c[i - 1][j] + c[i + 1][j] + c[i][j + 1] + c[i][j - 1]) + (1 - OMEGA) * c[i][j];
                }
            }
        }

        for (int i = 1; i < N - 1; i++)
        {
            for (int j = 1; j < N - 1; j++)
            {
                if ((i + j) % 2 == 1)
                {
                    c[i][j] = OMEGA / 4 * (c[i - 1][j] + c[i + 1][j] + c[i][j + 1] + c[i][j - 1]) + (1 - OMEGA) * c[i][j];
                }
            }
        }

        for (int i = 1; i < N - 1; i++)
        {
            for (int j = 1; j < N - 1; j++)
            {
                if (c[i][j] < 0)
                {
                    c[i][j] = 0;
                }
            }
        }
        // print();
        //  check sai so
        for (int i = 1; i < N - 1; i++)
        {
            for (int j = 1; j < N - 1; j++)
            {
                max_delta = max(max_delta, abs(c[i][j] - tmp[i][j]));
            }
        }
    } while (max_delta > TOLERANCE);
}

// dua o vua moi them vao list virus ve nong do thuc an = 0
void eat()
{
    for (VirusPlace temp : virus)
    {
        c[temp.x][temp.y] = 0;
    }
}

void computeProbality()
{
    double sum = 0;
    for (VirusPlace cand : candidate)
    {
        int u = cand.x;
        int v = cand.y;
        sum += pow(c[u][v], n);
    }
    for (int i = 0; i < candidate.size(); i++)
    {
        int u = candidate[i].x;
        int v = candidate[i].y;
        candidate[i].prob = pow(c[u][v], n) / sum;
    }
}

void growth()
{
    // random a number in range (0-1), if probability of candidate
    // is greater than number we random then add it to virus list
    int nCandidate = candidate.size();
    double randomNum ;
    //if (nCandidate <= 100)
        randomNum = (rand() % 100) * 1.0 / 100;
    /*  else if(nCandidate <= 200)
        randomNum = 0.25 * (float)rand() / RAND_MAX;
    else if(nCandidate <= 400)
        randomNum = 1/32 *(float)rand() / RAND_MAX;
    else if(nCandidate <= 800)
        randomNum = 1 / 128 * (float)rand() / RAND_MAX;
    else
        randomNum = 1 / 500 * (float)rand() / RAND_MAX; */
    for (int i = 0; i < nCandidate; i++)
    {
        vector<VirusPlace> PossibleVirus;
        if (randomNum <= candidate[i].prob)
            addVirus(candidate[i].x, candidate[i].y);
    }
    // cout << endl;
}

int main()
{
    // number of virus needed
    int numberOfVirus = 2000;
    initialize();
    srand((int)time(0));
    freopen("result.txt", "w+", stdout);
    while (virus.size() < numberOfVirus)
    {
        sor();
        eat();
        computeProbality();
        growth();
        /* for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << c[i][j] << " ";
        cout << endl;
    } */
    }

    eat();
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << grow[i][j] << " ";
        cout << endl;
    }

    return 0;
}