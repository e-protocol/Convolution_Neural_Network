#include "paint.h"
#include "ui_paint.h"
#include <QTextBrowser>
#include <QLCDNumber>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <QMessageBox>
#include <QtGui>
#include <QtWidgets>


Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);

    scene = new paintScene();       // Инициализируем графическую сцену
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену
    ui->graphicsView->setStyleSheet("background: white");

    //отрисовываем сцену по центру экрана
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(this);
    QPoint center = rect.center();
    int x = center.x() - (width()/2);
    int y = center.y() - (height()/2);
    center.setX(x);
    center.setY(y);
    move(center);

    timer = new QTimer();       // Инициализируем таймер
    connect(timer, &QTimer::timeout, this, &Paint::slotTimer);
    timer->start(100);          // Запускаем таймер
    if(example_count == 0)
    {
        for(int k = 0; k < SIZE_ARRAY_IN; k++)
            matrix[k].getInitialize();
        getInitializeWeights();
        getRandomWeights();
    }
    //устанавливаем стиль объектов
    ui->lcdNumber->setStyleSheet("background: black;");
    ui->lcdNumber->setPalette(Qt::green);
    ui->lcdNumber_2->setStyleSheet("background: black;");
    ui->lcdNumber_2->setPalette(Qt::green);
    ui->textBrowser_2->setStyleSheet("background: #EEEEEC; border: none; color: black");
    ui->textBrowser_3->setStyleSheet("background: #EEEEEC; border: none; color: black");
    ui->textBrowser_4->setStyleSheet("background: #EEEEEC; border: none; color: black");
    ui->textBrowser_5->setStyleSheet("background: #EEEEEC; border: none; color: black");
    //менюБар
    QGridLayout *gridLayout = new QGridLayout;
        setLayout(gridLayout);
        QMenu*   pmnu   = new QMenu("&Menu");
        QMenu*   pmnu_2   = new QMenu("&Examples");
        QMenu*   pmnu_3   = new QMenu("&Weights");
        QMenuBar *mb = new QMenuBar;
        mb->addMenu(pmnu);
        mb->addMenu(pmnu_2);
        mb->addMenu(pmnu_3);
        //Информация
        pmnu->addAction("&About",this, SLOT(about()));
        //Выход
        pmnu->addAction("&Quit",this, SLOT(quit()));
        //сохранить примеры
        pmnu_2->addAction("Save &Examples",this, SLOT(saveExamples()));
        //загрузить примеры
        pmnu_2->addAction("&Load Examples",this, SLOT(readExamples()));
        //сбросить текущие примеры
        pmnu_2->addAction("&Clear Examples",this, SLOT(clearExamples()));
        //сохранить веса
        pmnu_3->addAction("Save &Weights",this, SLOT(saveWeights()));
        //сбросить веса
        pmnu_3->addAction("&Drop Weights",this, SLOT(dropWeights()));
        //загрузить веса
        pmnu_3->addAction("L&oad Weights",this, SLOT(readWeights()));
        layout()->setMenuBar(mb);
    //вывод статистики
        int answer = 10;
        viewStatistic(answer);
}

Paint::~Paint()
{
    delete ui;
    delete timer;
    delete scene;
}

void Paint::slotTimer()
{
    // Переопределяем размеры графической сцены в зависимости от размеров окна
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void Paint::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QWidget::resizeEvent(event);
}

void Paint::on_pushButton_1_clicked()
{
        //Очистка рисунка
            scene->clear();
            ui->graphicsView->items().clear();
            ui->textEdit->clear();
            ui->lcdNumber->display(0);
}

void Paint::on_pushButton_2_clicked()
{
    //работа с пикселями
    std::vector<unsigned int>pixel_in;
    QPixmap pixmap = ui->graphicsView->grab();
    QImage img = pixmap.toImage();
    img = img.scaled(48,48,Qt::KeepAspectRatio); // SIZE_ARRAY_IMG 48
    for(int k = 0; k < img.height(); k++)
        for(int j = 0; j < img.width();j++)
            pixel_in.push_back(img.pixel(k,j));
    //очистка текста
    ui->textEdit->clear();
    //вывод текста
    int answer = feedForward(pixel_in);
    ui->textEdit->insertPlainText(QString::number(answer));
    ui->textEdit->update();
    ui->textEdit->show();
    //вывод статистики
    viewStatistic(answer);
}

void Paint::on_pushButton_3_clicked()
{
    int era = 1;
    //очистка текста
    ui->textEdit->clear();
    if(example_count == 0)
    {
        //очистка текста
        ui->textEdit->clear();
        //вывод текста
        ui->textEdit->insertPlainText("no data") ;
        ui->textEdit->update();
        ui->textEdit->show();
    }
    else
    {
        //обучение сети
        ui->lcdNumber->display(0);
        while(true)
        {
            //проверка на завершение программы
            if(exitProccess)
                break;

            unsigned int check = 0;
            for(unsigned int k = 0; k < pxl.size(); k++)
            {
                int answer = feedForward(pxl[k]);
                if( answer != number[k])
                    backPropagation(number[k]);
                else
                    check++;
                viewStatistic(answer);
            }
            era++;
            qApp->processEvents();
            ui->lcdNumber->display(era);
            ui->lcdNumber->update();
            if(check == number.size() || era == ERA)
                break;
    }

    //вывод текста
    if(era == ERA)
        ui->textEdit->insertPlainText("out of range");
    else
        ui->textEdit->insertPlainText("educated");
    ui->textEdit->update();
    ui->textEdit->show();
    }
}

void Paint::on_pushButton_4_clicked()
{
    //проверка ввода на число
    bool ok;
    QString str= ui->textEdit->toPlainText();
    int num = str.toInt(&ok,10);
    if((num > 9 || num < 0) || !ok)
    {
        //очистка текста
        ui->textEdit->clear();
        //вывод текста
        ui->textEdit->insertPlainText("invalid input");
        ui->textEdit->update();
        ui->textEdit->show();
    }
    else
    {
        //работа с пикселями
        std::vector<unsigned int>pixel_in;
        QPixmap pixmap= ui->graphicsView->grab();
        QImage img = pixmap.toImage();
        img = img.scaled(48,48,Qt::KeepAspectRatio);
        for(int k = 0; k < img.height(); k++)
        {
            for(int j = 0; j < img.width();j++)
                pixel_in.push_back(img.pixel(k,j));
        }
        pxl.push_back(pixel_in);
        number.push_back(num);
        example_count++;
        //очистка текста
        ui->textEdit->clear();
        //вывод текста
        ui->textEdit->insertPlainText("added");
        ui->textEdit->update();
        ui->textEdit->show();
        ui->lcdNumber_2->display(example_count);
        ui->lcdNumber_2->update();
    }
}

void Paint::viewStatistic(int answer)
{
    ui->textBrowser_5->clear();
    QString stat = "Statistics: <br>";
    QString colorRed = "<font color=\"Red\">";
    QString colorBlack = "<font color=\"Black\">";
    for(int k = 0; k < 10; k++)
    {
        QString arr = QString::number(k) + ": " + QString("%1").arg(layerOUT[k] * 100.0,0,'f',2) + "%" + "<br>";

        if(k == answer && answer < 10)
            arr = colorRed % arr;
        else
            arr = colorBlack % arr;
        stat += arr;
    }
    QString err = "Error: " + QString("%1").arg(total_error,0,'f',6);
    stat += colorBlack % err;
    ui->textBrowser_5->insertHtml(stat);
}

void Paint::saveExamples()
{
    //подтверждение команды
    QMessageBox msgBox;
    msgBox.setWindowTitle("Save Examples");
    msgBox.setText("Overwrite examples?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        //запись картинки
        std::ofstream out; // поток для записи
        out.open("examples.txt"); // окрываем файл для записи
        if (out.is_open())
        {
            for(unsigned int k = 0; k < pxl.size(); k++)
            {
                for(unsigned int j = 0; j < pxl[k].size();j++)
                    out << pxl[k][j] << " ";
                out << "\n";
            }
        out.close();
        }
        else
            QMessageBox::critical(this, "Error","Can't write examples.txt");

        // запись правильных ответов
        out.open("answers.txt"); // окрываем файл для записи
        if (out.is_open())
        {
            for(unsigned int k = 0; k < pxl.size(); k++)
                   out << number[k] << " ";
        out.close();
        //очистка текста
        ui->textEdit->clear();
        //вывод текста
        ui->textEdit->insertPlainText("saved");
        ui->textEdit->update();
        ui->textEdit->show();
        }
        else
            QMessageBox::critical(this, "Error","Can't write answers.txt");
    }
}

void Paint::readExamples()
{
    //подтверждение команды
    QMessageBox msgBox;
    msgBox.setWindowTitle("Upload Examples");
    msgBox.setText("Do you want to upload examples? This action will overwrite current examples!");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        //очистка текста
        ui->textEdit->clear();
        //сбрасываем переменные
        example_count = 0;
        pxl.clear();
        number.clear();
        ui->lcdNumber_2->display(0);
        ui->lcdNumber_2->update();

        //считываем правильные ответы
        std::ifstream in_num("answers.txt"); // поток для чтения
        std::string input_num;
        if (in_num.is_open()) // окрываем файл для чтения
        {
            while (getline(in_num, input_num))
            {
                for(unsigned int k = 0; k < input_num.size(); k++)
                    if(input_num[k] == ' ')
                        number.push_back(input_num[k - 1] - '0');
            }

        }
        else
            QMessageBox::critical(this, "Error","Can't read answers.txt");
        in_num.close();
        //считываем пиксели из файла
        std::string input;
        std::string upload;
        std::ifstream in("examples.txt"); // поток для чтения
        if (in.is_open()) // окрываем файл для чтения
        {
            while (getline(in, input))
            {
                int min = 0; // номер первого символа
                int max = 0; // номер последнего символа = пробела
                std::vector<unsigned int> number_in;
                for(unsigned int k = 0; k < input.size(); k++)
                {
                    if(input[k] == ' ' && max == 0)
                    {
                        max = k;
                        min = k - 10;
                    }
                    if(max != 0)
                    {
                        for(int j = min; j < max; j++)
                            upload.push_back(input[j]);
                        min = 0;
                        max = 0;
                        // конвертируем строку в число
                        std::istringstream ss(upload);
                        unsigned int num_pxl;
                        ss >> num_pxl;
                        upload.clear(); // сбрасываем строку

                        number_in.push_back(num_pxl); //добавляем пиксель
                        //если все пиксели картинки считаны
                        int pixel_count = SIZE_IMG * SIZE_IMG; // кол-во пикселей
                        if(number_in.size() % pixel_count  == 0)
                        {
                            pxl.push_back(number_in);
                            example_count++;
                            qApp->processEvents();
                            ui->lcdNumber_2->display(example_count);
                            ui->lcdNumber_2->update();
                        }
                    }
                }
            }
            //вывод текста
            ui->textEdit->insertPlainText("loaded");
            ui->textEdit->update();
            ui->textEdit->show();
        }
        else
            QMessageBox::critical(this, "Error","Can't read examples.txt");
        in.close();
    }
}

void Paint::clearExamples()
{
    //подтверждение команды
    QMessageBox msgBox;
    msgBox.setWindowTitle("Clear Examples");
    msgBox.setText("Are you sure you want to clear current examples?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        pxl.clear();
        number.clear();
        example_count = 0;
        ui->lcdNumber_2->display(example_count);
        ui->lcdNumber_2->update();
        //вывод текста
        ui->textEdit->clear(); //очистка текста
        ui->textEdit->insertPlainText("cleared");
        ui->textEdit->update();
        ui->textEdit->show();
    }
}

void Paint::saveWeights()
{
    //подтверждение команды
    QMessageBox msgBox;
    msgBox.setWindowTitle("Save Weights");
    msgBox.setText("Are you sure you want to save current weights?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        //запись весов
        std::ofstream out; // поток для записи
        out.open("weights.txt"); // окрываем файл для записи
        if (out.is_open())
        {
            out << " ";
            for(int i = 0; i < SIZE_ARRAY_IN; i++)
                for(int k = 0; k < SIZE_WEIGHT; k++)
                    for(int j = 0; j < SIZE_WEIGHT; j++)
                        out << matrix[i].weightConv1[k][j] << " ";
                out << "\n";
                out << " ";
            for(int i = 0; i < SIZE_ARRAY_IN; i++)
                for(int k = 0; k < SIZE_WEIGHT; k++)
                    for(int j = 0; j < SIZE_WEIGHT; j++)
                        out << matrix[i].weightConv2[k][j] << " ";
                out << "\n";
                out << " ";

            for(int k = 0; k < SIZE_ARRAY_1; k++)
                for(int j = 0; j < SIZE_IN; j++)
                    out << weight_1[k][j] << " ";
            out << "\n";
            out << " ";
            for(int k = 0; k < SIZE_ARRAY_OUT; k++)
                for(int j = 0; j < SIZE_ARRAY_1; j++)
                    out << weight_OUT[k][j] << " ";
            out << "\n";
            out << " ";

            for(int i = 0; i < SIZE_ARRAY_IN; i++)
                out << matrix[i].shift_1 << " ";
            out << "\n";
            out << " ";
            for(int i = 0; i < SIZE_ARRAY_IN; i++)
                out << matrix[i].shift_2 << " ";
            out << "\n";
            out << " ";
            for(int k = 0; k < SIZE_ARRAY_1; k++)
                out << shift_3[k] << " ";
            out << "\n";
            out << " ";
            for(int k = 0; k < SIZE_ARRAY_OUT; k++)
                out << shift_4[k] << " ";
            out << "\n";
        }
        else
            QMessageBox::critical(this, "Error","Can't write weights.txt");
        out.close();
        //вывод текста
        ui->textEdit->clear(); //очистка текста
        ui->textEdit->insertPlainText("saved");
        ui->textEdit->update();
        ui->textEdit->show();
    }
}

void Paint::dropWeights()
{
    //подтверждение команды
    QMessageBox msgBox;
    msgBox.setWindowTitle("Drop Weights");
    msgBox.setText("Are you sure you want to drop current weights?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        getRandomWeights();
        //вывод текста
        ui->textEdit->clear(); //очистка текста
        ui->textEdit->insertPlainText("dropped");
        ui->textEdit->update();
        ui->textEdit->show();
    }
}


void Paint::readWeights()
{
    //подтверждение команды
    QMessageBox msgBox;
    msgBox.setWindowTitle("Load Weights");
    msgBox.setText("Do you want to upload weights? This action will overwrite current weights!");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        //считываем веса из файла
        std::string input;
        std::string upload;
        std::vector<double> number_in;
        std::ifstream in("weights.txt"); // поток для чтения
        if (in.is_open()) // окрываем файл для чтения
        {
            while (getline(in, input))
            {
                int min = 0; // номер первого символа
                int max = 0; // номер последнего символа = пробела
                for(unsigned int k = 0; k < input.size(); k++)
                {
                    if(min == 0 && input[k] == ' ')
                    {
                        min = k + 1;
                        continue;
                    }
                    if(min != 0 && max == 0 && input[k] == ' ')
                        max = k;
                    if(max != 0)
                    {
                        for(int j = min; j < max; j++)
                            upload.push_back(input[j]);
                        min = 0;
                        max = 0;
                        // конвертируем строку в число
                        std::istringstream os(upload);
                        double val = 0.0;
                        os >> val;
                        upload.clear();
                        number_in.push_back(val);
                        k--;
                    }
                }
            }
            uploadWeights(number_in);
            //вывод текста
            ui->textEdit->clear(); //очистка текста
            ui->textEdit->insertPlainText("loaded");
            ui->textEdit->update();
            ui->textEdit->show();
        }
        else
            QMessageBox::critical(this, "Error","Can't read weights.txt");
        in.close();
    }
}

void Paint::quit()
{
    exitProccess = true;
    qApp->quit();
}

void Paint::about()
{
    QMessageBox::information(this, "About","This is the CNIRP - a  Convolution neural network Numeric Image Recognition Programm. Created by E-Protocol");
}


