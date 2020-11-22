#include"neuron.h"
#include<QWidget>

double Neuron::getRandom()
{
    double f = (double)rand() / RAND_MAX;
    return -1.0 + f * (1.0 - ( -1.0));
}

void Neuron::getInitializeWeights()
{
    for(int k = 0; k < SIZE_ARRAY_1; k++)
        weight_1[k] = new double[SIZE_IN];
    for(int k = 0; k < SIZE_ARRAY_OUT; k++)
        weight_OUT[k] = new double[SIZE_ARRAY_1];
}

void Neuron::getRandomWeights()
{
    //инициализируем веса рандомом
    for(int i = 0; i < SIZE_ARRAY_IN; i++)
    {
        for(int k = 0; k < SIZE_WEIGHT; k++)
            for(int j = 0; j < SIZE_WEIGHT; j++)
            {
                matrix[i].weightConv1[k][j] = getRandom();
                matrix[i].weightConv2[k][j] = getRandom();
            }
    }

    for(int k = 0; k < SIZE_ARRAY_1; k++)
        for(int j = 0; j < SIZE_IN; j++)
            weight_1[k][j] = getRandom();

    for(int k = 0; k < SIZE_ARRAY_OUT; k++)
        for(int j = 0; j < SIZE_ARRAY_1; j++)
            weight_OUT[k][j] = getRandom();
    //инициализируем сдвиги рандомом
    for(int k = 0; k < SIZE_ARRAY_IN; k++)
    {
        matrix[k].shift_1 = getRandom();
        matrix[k].shift_2 = getRandom();
    }
    for(int k = 0; k < SIZE_ARRAY_1; k++)
        shift_3[k] = getRandom();
    for(int k = 0; k < SIZE_ARRAY_OUT; k++)
        shift_4[k] = getRandom();
}

//**************************************** Feed Forward ****************************************
int Neuron::feedForward(std::vector<unsigned int> pxl)
{
    //входной сверточный слой
    int z = 0;
    for(int i = 0; i < SIZE_ARRAY_IN; i++)
    {
        for (int k = 0; k < SIZE_IMG; k++)
            for(int j = 0; j < SIZE_IMG; j++)
            {
                if(pxl[z] == 4294967295) // exclude background, this is value of white pixel long int
                    matrix[i].layerIMG[k][j] = -1.0;
                else
                {
                    QColor color(pxl[z]);
                    matrix[i].layerIMG[k][j] = static_cast<double>((color.red() * 0.3 + color.green() * 0.59 + color.blue() * 0.11) / 3.0 / 255.0); //for grey pixel change SIZE_ARRAY_IN to 2

                    //for color pixel change SIZE_ARRAY_IN to 6
                   /* QColor color(pxl[z]);
                    if(i == 0 || i == 1)
                        matrix[i].layerIMG[k][j] = static_cast<double>(color.red() / 255.0);
                    if(i == 2 || i == 3)
                        matrix[i].layerIMG[k][j] = static_cast<double>(color.green()/ 255.0);
                    if(i == 4 || i == 5)
                        matrix[i].layerIMG[k][j] = static_cast<double>(color.blue()/ 255.0);*/
                }

                    z++;
            }
        z = 0;
    }


    //сверточные слои
    for(int k = 0; k < SIZE_ARRAY_IN; k++)
    {
        //первый сверточный слой
        matrix[k].getConvolution(matrix[k].layerIMG, SIZE_CONV_1, matrix[k].layerConv1, matrix[k].weightConv1, matrix[k].shift_1);
        //первый подвыборочный слой
        matrix[k].getPooling(matrix[k].layerConv1, SIZE_POOL_1, matrix[k].layerPool1);
        //второй сверточный слой
        matrix[k].getConvolution(matrix[k].layerPool1,SIZE_CONV_2, matrix[k].layerConv2, matrix[k].weightConv2, matrix[k].shift_2);
        //второй подвыборочный слой
        matrix[k].getPooling(matrix[k].layerConv2, SIZE_POOL_2, matrix[k].layerPool2);
    }

    //входной слой перцептрона (плоский слой) и первый слой перцептрона
    double max = 0.0;
    for(int k = 0; k < SIZE_ARRAY_1; k++)
    {
        double val = 0.0;
        int x = 0;

        for(int m = 0; m < SIZE_ARRAY_IN; m++)
            for(int i = 0; i < SIZE_POOL_2; i++)
                for(int j = 0; j < SIZE_POOL_2; j++)
                {
                    layerIN[x] = matrix[m].layerPool2[i][j];
                    if(layerIN[x] < 0.0)
                        layerIN[x] = 0.0;
                    val += layerIN[x] * weight_1[k][x];
                    x++;
                }

        val += shift_3[k];
        if(val > 0.0)
            layer1[k] = val;
        else
            layer1[k] = 0.0;
        if(layer1[k] > max)
            max = layer1[k];
    }

    //слой нормализации
    if(max != 0.0)
        for(int k = 0; k < SIZE_ARRAY_1; k++)
            layer1[k] /= max;

    //выходной слой
    sum = 0.0;
    for(int k = 0; k < SIZE_ARRAY_OUT; k++)
    {
        double val = 0.0;
        for(int j = 0; j < SIZE_ARRAY_1; j++)
            val += layer1[j] * weight_OUT[k][j];
        val += shift_4[k];
        layerOUT[k] = exp(val); //SoftMax
        sum += layerOUT[k];
    }

    max = 0.0;
    int max_element = 0;
    if(sum != 0.0)
        for(int k = 0; k < SIZE_ARRAY_OUT; k++)
        {
            layerOUT[k] /= sum;
            //запоминаем элемент с максимальным значением
            if (layerOUT[k] > max)
            {
                max = layerOUT[k];
                max_element = k;
            }
        }
    return max_element;
}

//**************************************** Back Propagation ****************************************

void Neuron::backPropagation(int symbol)
{
    total_error = 0.0;
    double crossEntropy = 0.0;
    //определяем ошибку
    //выходной слой перцептрона
    for (int k = 0; k < SIZE_ARRAY_OUT; k++)
    {
        double y = 0.0;
        if (k == symbol)
        {
            y = 1.0;
            crossEntropy = log(1.0 / layerOUT[k]) * y;
        }
        else
            y = 0.0;

        error_OUT[k] = (y - layerOUT[k]);
        softMaxDerivate[k] = -log(layerOUT[k]);
        // Эти производные дают одинаковый результат вместе с функцией error_OUT[k] = (y - layerSoftMax[k]);
        //softMaxDerivate[k] = -log(layerSoftMax[k]) * (1.0 - log(layerSoftMax[k]));
        //softMaxDerivate[k] = -log(layerSoftMax[k]);
        //softMaxDerivate[k] = 1.0;
    }


    total_error += (1.0 / static_cast<double>(example_count)) * crossEntropy;

    //первый слой перцептрона
    for (int k = 0; k < SIZE_ARRAY_1; k++)
    {
        error_1[k] = 0.0;
        for (int j = 0; j < SIZE_ARRAY_OUT; j++)
            error_1[k] += weight_OUT[j][k] * error_OUT[j];
    }

    //входной слой перцептрона
    for (int k = 0; k < SIZE_IN; k++)
    {
        error_IN[k] = 0.0;
        for (int j = 0; j < SIZE_ARRAY_1; j++)
            error_IN[k] += weight_1[j][k] * error_1[j];
    }


    //сверточные слои
    //второй подвыборочный слой
    int x = 0;
    for(int i = 0; i < SIZE_ARRAY_IN; i++)
        for(int k = 0; k < SIZE_POOL_2; k++)
            for (int j = 0; j < SIZE_POOL_2; j++)
            {
                matrix[i].error_Pool2[k][j] = error_IN[x];
                x++;
            }

    for (int i = 0; i < SIZE_ARRAY_IN; i++)
    {
        //второй сверточный слой
        matrix[i].getBackPooling(SIZE_CONV_2, matrix[i].layerConv2, SIZE_POOL_2, matrix[i].layerPool2, matrix[i].error_Conv2, matrix[i].error_Pool2);
        //первый подвыборочный слой
        matrix[i].getBackConvolution();
        //первый сверточный слой
        matrix[i].getBackPooling(SIZE_CONV_1, matrix[i].layerConv1, SIZE_POOL_1, matrix[i].layerPool1, matrix[i].error_Conv1, matrix[i].error_Pool1);
    }

    //обновляем веса !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //сверточные слои
    for(int i = 0; i < SIZE_ARRAY_IN; i++)
    {
        //первый сверточный слой
        updateWeightConv(matrix[i].layerIMG,SIZE_CONV_1, matrix[i].error_Conv1, matrix[i].weightConv1, LEARN);

        double shift_1_val = 0.0;
        for(int k = 0; k < SIZE_CONV_1; k++)
            for(int j = 0; j < SIZE_CONV_1; j++)
                shift_1_val += LEARN * matrix[i].error_Conv1[k][j];
        matrix[i].shift_1 += shift_1_val;

        //второй сверточный слой
        updateWeightConv(matrix[i].layerPool1,SIZE_CONV_2, matrix[i].error_Conv2, matrix[i].weightConv2, LEARN);

        double shift_2_val = 0.0;
        for(int k = 0; k < SIZE_CONV_2; k++)
            for(int j = 0; j < SIZE_CONV_2; j++)
                shift_2_val += LEARN * matrix[i].error_Conv2[k][j];
        matrix[i].shift_2 += shift_2_val;
    }

    //первый слой
    for (int k = 0; k < SIZE_ARRAY_1; k++)
    {
        for (int j = 0; j < SIZE_IN; j++)
            weight_1[k][j] += LEARN * error_1[k] * layerIN[j];
        shift_3[k] += LEARN * error_1[k];
    }
    //выходной слой
    for (int k = 0; k < SIZE_ARRAY_OUT; k++)
    {
        for (int j = 0; j < SIZE_ARRAY_1; j++)
            weight_OUT[k][j] += LEARN * layer1[j] * error_OUT[k] * softMaxDerivate[k];
        shift_4[k] += LEARN * error_OUT[k] * softMaxDerivate[k];
    }
}

//**************************************** Upload Weights ****************************************
void Neuron::uploadWeights(std::vector<double> &number_in)
{
    //заменяем веса
    int count = 0;
    for(int i = 0; i < SIZE_ARRAY_IN; i++)
        for(int k = 0; k < SIZE_WEIGHT; k++)
            for(int j = 0; j < SIZE_WEIGHT; j++)
            {
                matrix[i].weightConv1[k][j] = number_in[count];
                count++;
            }
    for(int i = 0; i < SIZE_ARRAY_IN; i++)
        for(int k = 0; k < SIZE_WEIGHT; k++)
            for(int j = 0; j < SIZE_WEIGHT; j++)
            {
                matrix[i].weightConv2[k][j] = number_in[count];
                count++;
            }
    for(int k = 0; k < SIZE_ARRAY_1; k++)
        for(int j = 0; j < SIZE_IN; j++)
        {
            weight_1[k][j] = number_in[count];
            count++;
        }
    for(int k = 0; k < SIZE_ARRAY_OUT; k++)
        for(int j = 0; j < SIZE_ARRAY_1; j++)
        {
            weight_OUT[k][j] = number_in[count];
            count++;
        }

    for(int i = 0; i < SIZE_ARRAY_IN; i++)
    {
        matrix[i].shift_1 = number_in[count];
        count++;
    }
    for(int i = 0; i < SIZE_ARRAY_IN; i++)
    {
        matrix[i].shift_2 = number_in[count];
        count++;
    }
    for(int k = 0; k < SIZE_ARRAY_1; k++)
    {
        shift_3[k] = number_in[count];
        count++;
    }
    for(int k = 0; k < SIZE_ARRAY_OUT; k++)
    {
        shift_4[k] = number_in[count];
        count++;
    }
}

Neuron::~Neuron()
{
    delete[] matrix;
    delete[] layerIN;
    delete[] layer1;
    delete[] layerOUT;

    for(int k = 0; k < SIZE_ARRAY_1; k++)
        delete[] weight_1[k];
    delete[] weight_1;

    for(int k = 0; k < SIZE_ARRAY_OUT; k++)
        delete[] weight_OUT[k];
    delete[] weight_OUT;

    delete[] shift_3;
    delete[] shift_4;
    delete[] error_OUT;
    delete[] error_IN;
    delete[] error_1;
    delete[] softMaxDerivate;
}


