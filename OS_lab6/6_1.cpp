#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <fstream>
void showMatrix(double* C, int N, int K);
void randominit();
void manualinit();
void* multMat(void* arg);
void printAB();

constexpr int N = 10;
constexpr int M = 10;
constexpr int K = 10;
int i_in[N*K];
int j_in[N*K];
double A[N*M];
double B[M*K];
double C[N*K] = {0};

int main(){
    srand(time(NULL));
    for (int i = 0; i < N; ++i)
    for (int j = 0; j < K; ++j) {
            i_in[i*K+j] = i;
            j_in[i*K+j] = j;
    }
    int arr[N*K];
    for(int i = 0; i < N * K; ++i)
    arr[i] = i;
    pthread_t th[N*K];
    int option;
    std::cout << "Write 1 to create random matrices, 2 to set elements manually: ";
    std::cin >> option;
    if(option == 1)
    randominit();
    else
    manualinit();
    printAB();
    for (int i = 0; i < N * K; ++i) {
        if (pthread_create(&th[i], NULL, &multMat, &(arr[i]))){
            return 1;
        }
    }
    
    for(int i = 0; i < N * K; ++i)
    pthread_join(th[i], NULL);
    std::cout << '\n';
    showMatrix(C, N, K);

}
void randominit(){
    for(int i = 0; i < N * M; ++i)
    A[i] = rand() % 10 + 1;
    for(int i = 0; i < M * K; ++i)
    B[i] = rand() % 10 + 1;
}

void manualinit(){
    std::cout << "Matrix A is " << N << "x" << M << '\n'
    << "Insert elements";
    
    double temp;
    for (int i = 0; i < N * M; ++i){ 
        std::cin >> temp;
        A[i] = temp;

    }
    std::cout << "Matrix B is " << M << "x" << K << '\n'
    << "Insert elements";
    for(int i = 0; i < M * K; ++i){
        std::cin >> temp;
        B[i] = temp;
    }
}

void showMatrix(double* C, int N, int K) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < K; ++j) {
            std::cout << C[i * K + j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "----------------\n";
}
void* multMat(void* arg){
    int id = *(int *)arg;                                                                                                                      
    usleep(rand() % 1000000);
    for (int m = 0; m < M; ++m) {
    C[i_in[id] * K + j_in[id]] += A[i_in[id] * M + m] * B[m * K + j_in[id]];
    }
    showMatrix(C, N, K);
    return NULL;

}

void printAB(){
    std::cout << "Matrix A\n";
    showMatrix(A, N, M);
    //std::cout << "----------------\n";
    std::cout << "Matrix B\n";
    showMatrix(B, M, K);
    std::ofstream myfile ("example.txt");
    if (myfile.is_open())
    {
        myfile << N << " " << M << " " << K << '\n';
        for(int count = 0; count < N * M; count ++){
            myfile << A[count] << " " ;
        }
        myfile << '\n';
        for(int count = 0; count < M * K; count ++){
            myfile << B[count] << " " ;
        }
        myfile.close();
    }
}