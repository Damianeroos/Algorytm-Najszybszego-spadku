#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "exprtk.hpp"
#include "qcustomplot.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pWczytaj_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_5_valueChanged(double arg1);

    void on_doubleSpinBox_4_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_pStart_clicked();




    void on_iteracjeL_valueChanged(int arg1);

    void on_epsilon_valueChanged(double arg1);

    void on_epsilon1_valueChanged(double arg1);

    void on_epsilon2_valueChanged(double arg1);

    void on_comboBox_currentIndexChanged(int index);



    void on_warstwaX1_valueChanged(double arg1);

    void on_warstwaY1_valueChanged(double arg1);

    void on_warstwaX2_valueChanged(double arg1);

    void on_warstwaY2_valueChanged(double arg1);


private:



    typedef exprtk::symbol_table<double> zmienne_t;
    typedef exprtk::expression<double> wyrazenie_t;
    typedef exprtk::parser<double>        parser_t;


    zmienne_t zmienne;
    wyrazenie_t wyrazenie;
    parser_t parser;

    std::string wyrazenie_string;

    double obliczAlpha(QVector<double> gradient,double dokladnosc);

    double x1,x2,x3,x4,x5;
    double xw1,xw2,yw1,yw2; //wykres warstwicowy

    int L;
    double eps,eps1,eps2;

    void rysujWykresPoziomicowy();

    bool punktSiodlowy(void);

    QCPColorScale *colorScale;
    QCPMarginGroup *marginGroup;
    QCPColorMap *colorMap;

    QString wzory[8];


    double pochodna_dx_dy(double h1,double h2);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
