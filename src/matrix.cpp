#include"matrix.h"

void Matrix::getInitialize()
{
    //создаем массивы
    for(int k = 0; k < SIZE_IMG; k++)
        layerIMG[k] = new double[SIZE_IMG];
    for(int k = 0; k < SIZE_CONV_1; k++)
        layerConv1[k] = new double[SIZE_CONV_1];
    for(int k = 0; k < SIZE_CONV_2; k++)
        layerConv2[k] = new double[SIZE_CONV_2];
    for(int k = 0; k < SIZE_POOL_1; k++)
        layerPool1[k] = new double[SIZE_POOL_1];
    for(int k = 0; k < SIZE_POOL_2; k++)
        layerPool2[k] = new double[SIZE_POOL_2];

    for(int k = 0; k < SIZE_WEIGHT; k++)
    {
        weightConv1[k] = new double[SIZE_WEIGHT];
        weightConv2[k] = new double[SIZE_WEIGHT];
    }

    for(int k = 0; k < SIZE_CONV_1; k++)
        error_Conv1[k] = new double[SIZE_CONV_1];
    for(int k = 0; k < SIZE_CONV_2; k++)
        error_Conv2[k] = new double[SIZE_CONV_2];
    for(int k = 0; k < SIZE_POOL_1; k++)
        error_Pool1[k] = new double[SIZE_POOL_1];
    for(int k = 0; k < SIZE_POOL_2; k++)
        error_Pool2[k] = new double[SIZE_POOL_2];
}

void Matrix::getConvolution(double **matrix,int matrixConvSize, double **convMatrix, double **weight, double shift)
{
    //сверточный слой
    int x = 0;
    int y = 0;
    double max = 0.0;
    for(int k = 0; k < matrixConvSize; k++)
    {
        for(int j = 0; j < matrixConvSize; j++)
            {
                double val = 0.0;
                for(int m = 0; m < SIZE_WEIGHT; m++)
                {
                    for(int n = 0; n < SIZE_WEIGHT; n++)
                    {
                        val += matrix[y][x] * weight[m][n];
                        x++;
                    }
                    y++;
                    x -= SIZE_WEIGHT;
                }
                y -= SIZE_WEIGHT;
                x++;
                val += shift;
                if(val > 0.0)
                    convMatrix[k][j] = val;
                else
                    convMatrix[k][j] = 0.0;
                if(convMatrix[k][j] > max)
                    max = convMatrix[k][j];
            }
            y++;
            x = 0;
     }
    //слой нормализации
    if(max != 0.0)
        for(int k = 0; k < matrixConvSize; k++)
            for(int j = 0; j < matrixConvSize; j++)
                convMatrix[k][j] /= max;
}

void Matrix::getPooling(double **matrix, int matrixPoolSize, double **poolMatrix)
{
    //подвыборочный слой
    int x = 0;
    int y = 0;
    int max_x = 2;
    int max_y = 2;
    for(int k = 0; k < matrixPoolSize; k++)
    {
        for(int j = 0; j < matrixPoolSize; j++)
        {
            double max = 0.0;
            while(y < max_y)
            {
                while(x < max_x)
                {
                    if(matrix[y][x] > max)
                        max = matrix[y][x];
                    x++;
                }
                y++;
                x -= 2;
            }
            y -= 2;
            x += 2;
            max_x += 2;
            poolMatrix[k][j] = max;
        }
        max_y += 2;
        max_x = 2;
        x = 0;
        y += 2;
    }
}

void Matrix::getBackPooling(int matrixSize, double **matrix, int matrixPoolSize, double **poolMatrix, double **errorConv, double **errorPool)
{
    //подвыборочный слой
    int x = 0;
    int y = 0;
    int max_x = 2;
    int max_y = 2;
    for(int k = 0; k < matrixPoolSize; k++)
    {
        if(max_y > matrixSize)
            max_y = 2;
        for(int j = 0; j < matrixPoolSize; j++)
        {
            if(max_x > matrixSize)
                max_x = 2;
            while(y < max_y)
            {
                while(x < max_x)
                {
                    if(matrix[y][x] == poolMatrix[k][j])
                        errorConv[y][x] = errorPool[k][j];
                    else
                        errorConv[y][x] = 0.0;
                    x++;
                }
                y++;
                x -= 2;
            }
            y -= 2;
            x += 2;
            max_x += 2;
        }
            max_y += 2;
        }
}

void Matrix::getBackConvolution()
{
    //переворачиваем ядро на 180
    double **weightReverse = new double*[SIZE_WEIGHT];
    for(int k = 0; k < SIZE_WEIGHT; k++)
        weightReverse[k] = new double[SIZE_WEIGHT];

    int x = SIZE_WEIGHT - 1;
    int y = SIZE_WEIGHT - 1;
    for(int k = 0; k < SIZE_WEIGHT; k++)
    {
        if(y < 0)
            y = SIZE_WEIGHT - 1;
        for(int j = 0; j < SIZE_WEIGHT; j++)
        {
            if(x < 0)
                x = SIZE_WEIGHT - 1;
            weightReverse[k][j] = weightConv2[y][x];
            x--;
        }
        y--;
    }
    //обратная свертка
    for(int k = 0; k < SIZE_POOL_1; k++)
        for(int j = 0; j < SIZE_POOL_1; j++)
            error_Pool1[k][j] = 0;
    x = 0;
    y = 0;
    for(int k = 0; k < SIZE_CONV_2; k++)
    {
        for(int j = 0; j < SIZE_CONV_2; j++)
        {
            for(int m = 0; m < SIZE_WEIGHT; m++)
                for(int n = 0; n < SIZE_WEIGHT; n++)
                {
                    error_Pool1[y + m][x + n] += error_Conv2[k][j] * weightReverse[m][n];
                }
            x++;
        }
        x = 0;
        y++;
    }
}

void Matrix::updateWeightConv(double **matrix,int errorConvSize, double **errorMatrix, double **weight, double learning_rate)
{
   int x = 0;
    int y = 0;
    for(int k = 0; k < errorConvSize; k++)
    {
        for(int j = 0; j < errorConvSize; j++)
        {
            for(int m = 0; m < SIZE_WEIGHT; m++)
                for(int n = 0; n < SIZE_WEIGHT; n++)
                    weight[m][n] += matrix[y + m][x + n] * errorMatrix[k][j] * learning_rate;
            x++;
        }
        x = 0;
        y++;
    }
}

Matrix::~Matrix()
{
    for(int k = 0; k < SIZE_IMG; k++)
        delete[] layerIMG[k];
    delete[] layerIMG;

    for(int k = 0; k < SIZE_CONV_1; k++)
        delete[] layerConv1[k];
    delete[] layerConv1;

    for(int k = 0; k < SIZE_CONV_2; k++)
        delete[] layerConv2[k];
    delete[] layerConv2;

    for(int k = 0; k < SIZE_POOL_1; k++)
        delete[] layerPool1[k];
    delete[] layerPool1;

    for(int k = 0; k < SIZE_POOL_2; k++)
        delete[] layerPool2[k];
    delete[] layerPool2;

    for(int k = 0; k < SIZE_WEIGHT; k++)
    {
        delete[] weightConv1[k];
        delete[] weightConv2[k];
    }
    delete[] weightConv1;
    delete[] weightConv2;

    for(int k = 0; k < SIZE_CONV_1; k++)
        delete[] error_Conv1[k];
    delete[] error_Conv1;

    for(int k = 0; k < SIZE_CONV_2; k++)
        delete[] error_Conv2[k];
    delete[] error_Conv2;

    for(int k = 0; k < SIZE_POOL_1; k++)
        delete[] error_Pool1[k];
    delete[] error_Pool1;

    for(int k = 0; k < SIZE_POOL_2; k++)
        delete[] error_Pool2[k];
    delete[] error_Pool2;
}

