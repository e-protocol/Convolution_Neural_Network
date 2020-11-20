#ifndef NEURON_H
#define NEURON_H

#define ERA 10000 // макс кол-во эпох
#define LEARN 0.025 //задаем шаг обучения
#include<vector>

#include <cstdlib>
#include <random>
#include "matrix.h"

class Neuron: public Matrix
{
public:
    Matrix *matrix = new Matrix[SIZE_ARRAY_IN];
    //слои перцептрона
    double *layerIN = new double[SIZE_IN];
    double *layer1 = new double[SIZE_ARRAY_1];
    double *layerOUT = new double[SIZE_ARRAY_OUT];
    //веса
    double **weight_1 = new double*[SIZE_ARRAY_1];
    double **weight_OUT = new double*[SIZE_ARRAY_OUT];
    //сдвиги
    double *shift_3 = new double[SIZE_ARRAY_1];
    double *shift_4 = new double[SIZE_ARRAY_OUT];
    //ошибки
    double *error_OUT = new double[SIZE_ARRAY_OUT];
    double *error_1 = new double[SIZE_ARRAY_1];
    double *error_IN = new double[SIZE_IN];
    double total_error; // общая ошибка сети
    int example_count = 0; //счетчик примеров

    double *softMaxDerivate = new double[SIZE_ARRAY_OUT];
    double sum = 0.0;

    void backPropagation(int symbol);
    int feedForward(std::vector<unsigned int> pxl);
    void getRandomWeights();
    double getRandom();
    void uploadWeights(std::vector<double> &number_in);
    void getInitializeWeights();
    ~Neuron();
};

#endif // NEURON_H

