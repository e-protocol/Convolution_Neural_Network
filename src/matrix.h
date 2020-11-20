#ifndef MATRIX_H
#define MATRIX_H
#include<cmath>

#define SIZE_IMG 48
#define SIZE_CONV_1 44
#define SIZE_POOL_1 22
#define SIZE_CONV_2 18
#define SIZE_POOL_2 9

#define SIZE_IN 810 // SIZE_ARRAY_IN * SIZE_POOL_2 * SIZE_POOL_2
#define SIZE_ARRAY_IN 10
#define SIZE_ARRAY_1 70
#define SIZE_ARRAY_OUT 10
#define SIZE_WEIGHT 5

class Matrix
{
public:
    //входной сверточный слой
     double **layerIMG = new double*[SIZE_IMG];
     //первый сверточный слой
     double **layerConv1 = new double*[SIZE_CONV_1];
     //первый подвыборочный слой
     double **layerPool1 = new double*[SIZE_POOL_1];
     //второй сверточный слой
     double **layerConv2 = new double*[SIZE_CONV_2];
     //пвторой подвыборочный слой
     double **layerPool2 = new double*[SIZE_POOL_2];
     //первое ядро весов
     double **weightConv1 = new double*[SIZE_WEIGHT];
     //второе ядро весов
     double **weightConv2 = new double*[SIZE_WEIGHT];
     //ошибки
     double **error_Pool2 = new double*[SIZE_POOL_2];
     double **error_Conv2 = new double*[SIZE_CONV_2];
     double **error_Pool1 = new double*[SIZE_POOL_1];
     double **error_Conv1 = new double*[SIZE_CONV_1];
     //сдвиги
     double shift_1;
     double shift_2;

     void getInitialize();
     void getConvolution(double **matrix, int matrixConvSize, double **convMatrix, double **weight, double shift);
     void getPooling(double **matrix, int matrixPoolSize, double **poolMatrix);
     void getBackPooling(int matrixSize, double **matrix, int matrixPoolSize, double **poolMatrix, double **errorConv, double **errorPool);
     void getBackConvolution();
     void updateWeightConv(double **matrix,int errorConvSize, double **errorMatrix, double **weight, double learning_rate);
     ~Matrix();
};

#endif // MATRIX_H
