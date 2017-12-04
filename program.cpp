#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
using namespace std;

int main(int argc, char** argv){
        int size, threads, i, j, k;
        double t1,t2,temp;
        ifstream file(argv[1]);
        file >> size >>threads;
        double **matrix = new double *[size];

        for (i = 0; i < size; i++)
                matrix[i] = new double [size];
        srand(time(NULL));
        for (i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                        matrix[i][j] = rand() % 101;
                }
        }

        double **E = new double *[size];

        for (i = 0; i < size; i++)
                E[i] = new double [size];

        for ( i = 0; i < size; i++){
                for (j = 0; j < size; j++){
                    E[i][j] = 0.0;
                    if (i == j)
                        E[i][j] = 1.0;
                }
        }
        t1 = omp_get_wtime();
        for ( k = 0; k < size; k++){
                temp = matrix[k][k];
                for ( j = 0; j < size; j++){
                        matrix[k][j] /= temp;
                        E[k][j] /= temp;
                }
                #pragma omp parallel for private (i, j, temp) num_threads(threads)
                for ( i = k + 1; i < size; i++){
                        temp = matrix[i][k];
                        for ( j = 0; j < size; j++){
                                matrix[i][j] -= matrix[k][j] * temp;
                                E[i][j] -= E[k][j] * temp;
                        }
                }
        }
        for (k = size - 1; k > 0; k--){
                #pragma omp parallel for private (i,j) num_threads(threads)
                for (i = k - 1; i >= 0; i--){
                        temp = matrix[i][k];
                        for (j = 0; j < size; j++){
                                matrix[i][j] -= matrix[k][j] * temp;
                                E[i][j] -= E[k][j] * temp;
                        }
                }
        }
        t2 = omp_get_wtime();   
        //for (i = 0; i < size; i++){
        //      for (int j = 0; j < size; j++){
        //          cout << E[i][j] << ' ';
        //      }
        //      cout << endl;
        //}

        for (int i = 0; i < size; i++){
                delete [] E[i];
        }
        delete [] E;
        for (int i = 0; i < size; i++)
                delete [] matrix[i];
        delete [] matrix;

        file.close();
        cout << t2-t1 <<endl;
        return 0;
}

