#include <bits/stdc++.h>

int adjMatrix[500][500];
inline int read()
{
    int X=0; bool flag=1; char ch=getchar();
    while(ch<'0'||ch>'9') {if(ch=='-') flag=0; ch=getchar();}
    while(ch>='0'&&ch<='9') {X=(X<<1)+(X<<3)+ch-'0'; ch=getchar();}
    if(flag) return X;
    return ~(X-1);
}

int getCostOfSolution(int sol[], int numCities){
    int cost = 0;
    for (int i = 1; i< numCities; i++){
        int prevPoint = sol[i-1];
        int currPoint = sol[i];
        cost += adjMatrix[prevPoint][currPoint];
    }
    cost += adjMatrix[sol[numCities -1]][0];
    return cost;
}

void initialization(int sol[], int numCities, int  i, int j){
    int visited[numCities];
    memset(visited, 0, sizeof(visited));
    visited[i] = true;
    visited[j] = true;
    int numOfVisitedCities = 2;
    sol[0] = i;
    sol[1] = j;
    while (numOfVisitedCities < numCities){
        int minDistanceSeenSoFar = 100000;
        int currMinCity = -1;
        int lastCity = sol[numOfVisitedCities - 1];
        for(int i = 0; i< numCities; i++){
            if (!visited[i]){
                if (adjMatrix[lastCity][i] < minDistanceSeenSoFar){
                    minDistanceSeenSoFar = adjMatrix[lastCity][i];
                    currMinCity = i;
                }
            }
        }
        visited[currMinCity] = true;
        sol[numOfVisitedCities] = currMinCity;
        numOfVisitedCities++;
    }
}


void vertexInsert(int sol[], int position_1, int position_2, int numCities){
    int new_sol[numCities];
    for (int i = 0; i < position_1; i++){
        new_sol[i] = sol[i];
    }
    new_sol[position_1] = sol[position_2];
    new_sol[position_1+1] = sol[position_1];
    for (int i = position_1 + 1 ; i < position_2; i++){
        new_sol[i+1] = sol[i];
    }
    for (int i = position_2 +1; i< numCities; i++){
        new_sol[i] = sol[i];
    }
    memcpy(sol, new_sol, sizeof(int) * numCities);
}

void blockReverse(int sol[], int position_1, int position_2, int numCities){
    int new_sol[numCities];
    for ( int i = 0; i<= position_1; i++){
        new_sol[i] = sol[i];
    }
    for ( int i = position_1+1; i < position_2; i++ ){
        int upper_position = position_2 - (i - position_1);
        new_sol[i] = sol[upper_position];
    }
    for(int i = position_2 ; i< numCities; i++){
        new_sol[i] = sol[i];
    }
    memcpy(sol, new_sol, sizeof(int) * numCities);
}



void generateNewSolution(int sol[], int numCities, double probOfSwap, double probOfReverse, double  probOfInsert){
    int a = 0;
    int b = numCities - 1;
    int position_1 = (rand () % (b-a))+ a; // generate [a,b)
    a = position_1;
    int position_2 = (rand () % (b-a))+ a + 1; // generate (a,b]
    double randomNum = rand () /double (RAND_MAX);
    if (randomNum < probOfSwap){
        std::swap(sol[position_1], sol[position_2]);}
    else if((probOfSwap + probOfReverse) > randomNum >= probOfSwap){
        blockReverse(sol, position_1, position_2, numCities);
    }
    else if (probOfInsert + (probOfSwap + probOfReverse) > randomNum){
        vertexInsert(sol, position_1, position_2, numCities);
    }
    else{
        unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count ();
        std::shuffle (sol+position_1, sol+position_2, std::default_random_engine(seed));
    }

}

void random_initialization(int sol[], int numCities){
    for(int i = 0; i < numCities; i++ ){
        sol[i] = i;
    }
    unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count ();
    std::shuffle (sol, sol+numCities, std::default_random_engine(seed));
}




int main(){
    srand((unsigned)time(NULL));
    int numOfCities = read();
    double T0 =10000.;
    double T_end = 1e-8;
    double q = 0.98;
    double L= 1000;
    double probOfSwap = 0;
    double probOfReverse = 0.8;
    double probOfInsert = 0.1;
    bool randomInitialization = false;
    if (numOfCities == 50){
        T0 =1000;
        q = 0.985;
        T_end = 0.005;
        randomInitialization = true;

    }
    if (numOfCities == 100){
        T0 = 10;
        q = 0.982;
        probOfSwap = 0.2;
        probOfReverse = 0.4;
    }
    int minDistanceBetweenTwoNodes = 100000;
    int start = -1;
    int end = -1;
    for( int i = 0; i < numOfCities; i++){
        for(int j = 0; j < numOfCities; j++){
            adjMatrix[i][j] = read();
            if (minDistanceBetweenTwoNodes > adjMatrix[i][j] && i!=j){
                minDistanceBetweenTwoNodes = adjMatrix[i][j];
                start = i;
                end = j;
            }
        }
    }
    int possibleSolution[numOfCities];
    int possibleSolutionCopy[numOfCities];
    double  T = T0;
    initialization(possibleSolution, numOfCities, start, end);
    initialization(possibleSolutionCopy,numOfCities, end, start );
    int cost_1 = getCostOfSolution(possibleSolution, numOfCities);
    int cost_2 =  getCostOfSolution(possibleSolutionCopy, numOfCities);
    if (cost_1 > cost_2){
        memcpy(possibleSolution, possibleSolutionCopy, (numOfCities)*sizeof(int));
    }
    if (numOfCities == 500){

        for (int i = 0; i< numOfCities; i++){
            printf("%d ", possibleSolution[i]+1);
        }
        return 0;
    }
    if (randomInitialization){
        random_initialization(possibleSolution, numOfCities);
    }
    int CostOfPreviousSolution, CostOfNewSolution, DiffOfTwoSolution;
    double randomNum;
    while(T > T_end){
        for (int i = 0; i < L; i++){
            memcpy(possibleSolutionCopy, possibleSolution, (numOfCities)*sizeof(int));
            generateNewSolution(possibleSolution, numOfCities, probOfSwap, probOfReverse, probOfInsert);
            CostOfPreviousSolution = getCostOfSolution(possibleSolutionCopy, numOfCities);
            CostOfNewSolution = getCostOfSolution(possibleSolution, numOfCities);
            DiffOfTwoSolution = CostOfNewSolution - CostOfPreviousSolution;
            if (DiffOfTwoSolution >= 0){
                randomNum = ((double)rand())/(RAND_MAX);
                if (exp(-DiffOfTwoSolution/T) <= randomNum){
                    memcpy(possibleSolution, possibleSolutionCopy, (numOfCities)*sizeof(int));
                }
                if (CostOfNewSolution <= 200){
                    break;
                }
            }
        }
        T *= q;
    }
    for (int i = 0; i< numOfCities; i++){
        printf("%d ", possibleSolution[i]+1);
    }

}