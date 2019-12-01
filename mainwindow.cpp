#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::setWindowTitle("Algorytm najszybszego spadku");

    ui->label_6->setPixmap(QPixmap("/home/damian/Pulpit/QT/Algorymt_NN/eps.png"));
    ui->label_8->setPixmap(QPixmap("/home/damian/Pulpit/QT/Algorymt_NN/eps1.png"));
    ui->label_7->setPixmap(QPixmap("/home/damian/Pulpit/QT/Algorymt_NN/eps2.png"));


    ui->comboBox->addItem("----");
    ui->comboBox->addItem("Rosenbrock");
    ui->comboBox->addItem("Himmelblau");
    ui->comboBox->addItem("Sfera");
    ui->comboBox->addItem("Rastrigin");
    ui->comboBox->addItem("Three-hump Camel");
    ui->comboBox->addItem("(x1-2)^2+(x1-x2^2)^2");
    ui->comboBox->addItem("2x1^2-2x1*x2^2+x2^4-8x1+16");

    wzory[0] = "";
    wzory[1] = "(1-x1)^2+100(x2-x1^2)^2";
    wzory[2] = "(x1^2+x2-11)^2+(x1+x2^2-7)^2";
    wzory[3] = "x1^2+x2^2";
    wzory[4] = "20 + x1^2 + x2^2 - 10cos(2*3.14*x1) - 10cos(2*3.14*x2)";
    wzory[5] = "2x1^2-1.05*x1^4+x1^6/6+x1*x2+x2^2";
    wzory[6] = "(x1-2)^2+(x1-x2^2)^2";
    wzory[7] = "2x1^2-2x1*x2^2+x2^4-8x1+16";


    wyrazenie_string = "";
    x1=x2=x3=x4=x5=0;
    zmienne.add_variable("x1",x1);
    zmienne.add_variable("x2",x2);
    zmienne.add_variable("x3",x3);
    zmienne.add_variable("x4",x4);
    zmienne.add_variable("x5",x5);
    zmienne.add_constants();
    wyrazenie.register_symbol_table(zmienne);



    ui->customPlot->xAxis->setLabel("x1");
    ui->customPlot->yAxis->setLabel("x2");
    ui->customPlot->xAxis->setRange(-1,1);
    ui->customPlot->yAxis->setRange(-1,1);
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);

    xw1 = ui->warstwaX1->value();
    xw2 = ui->warstwaX2->value();
    yw1 = ui->warstwaY1->value();
    yw2 = ui->warstwaY2->value();

    ui->funkcjaCelu->setText("");


    L = 100;
    ui->iteracjeL->setValue(L);
    eps = 0.005;
    ui->epsilon->setValue(eps);
    eps1 = 0.00016;
    ui->epsilon1->setValue(eps1);
    eps2 = 0.000007;
    ui->epsilon2->setValue(eps2);

    colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);
    colorScale = new QCPColorScale(ui->customPlot);
    marginGroup = new QCPMarginGroup(ui->customPlot);

}

MainWindow::~MainWindow()
{
    delete ui;

    delete colorMap;
    delete colorScale;
    delete marginGroup;
}

void MainWindow::on_pWczytaj_clicked()
{
    //wczytujemy punkt startowy
    x1 = ui->doubleSpinBox->value();
    x2 = ui->doubleSpinBox_2->value();
    x3 = ui->doubleSpinBox_5->value();
    x4 = ui->doubleSpinBox_3->value();
    x5 = ui->doubleSpinBox_3->value();

    QVector<double> x(1),y(1);
    x[0] = x1;
    y[0] = x2;



    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();


    ui->customPlot->replot();
    //wczytujemy wyrażenie
    wyrazenie_string = ui->funkcjaCelu->text().toUtf8().constData();
    ui->tekst->setText(QString::fromStdString(wyrazenie_string));

    parser.compile(wyrazenie_string,wyrazenie);

    rysujWykresPoziomicowy();

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x,y);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,Qt::red,Qt::red, 4));
    ui->customPlot->replot();
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    x1 = arg1;
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    x2 = arg1;
}

void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    x3 = arg1;
}
void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
    x4 = arg1;
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    x5 = arg1;
}

void MainWindow::on_pStart_clicked()
{
    double derivative_x1,derivative_x2,derivative_x3,derivative_x4,derivative_x5;
    QVector<double> x(1),y(1),gradient(5);
    QString wynik,info;
    double Eps,Eps1,Eps2;
    double x1t,x2t,x3t,x4t,x5t;
    double fcelu_old,fcelu_new;
    int koniec_algorytmu;
    ui->pWczytaj->setEnabled(false);
    ui->pStart->setEnabled(false);
    ui->doubleSpinBox->setEnabled(false);
    ui->doubleSpinBox_2->setEnabled(false);
    ui->doubleSpinBox_3->setEnabled(false);
    ui->doubleSpinBox_4->setEnabled(false);
    ui->doubleSpinBox_5->setEnabled(false);
    ui->tekst->setText(QString(""));

    for( int g=1; g<ui->customPlot->graphCount(); g++ )
    {
        ui->customPlot->graph(g)->data()->clear();
    }

    x1 = ui->doubleSpinBox->value();
    x2 = ui->doubleSpinBox_2->value();
    x3 = ui->doubleSpinBox_5->value();
    x4 = ui->doubleSpinBox_4->value();
    x5 = ui->doubleSpinBox_3->value();

    double alpha;


    for(int i = 0; i< L; i++){
        koniec_algorytmu = 0;

        gradient[0] = derivative_x1 = exprtk::derivative(wyrazenie,x1);
        gradient[1] = derivative_x2 = exprtk::derivative(wyrazenie,x2);
        gradient[2] = derivative_x3 = exprtk::derivative(wyrazenie,x3);
        gradient[3] = derivative_x4 = exprtk::derivative(wyrazenie,x4);
        gradient[4] = derivative_x5 = exprtk::derivative(wyrazenie,x5);
        //tutaj musimy minimalizowac alpha np.metoda zlotego podzialu
        alpha = MainWindow::obliczAlpha(gradient,0.0001);

        //
        x1t = x1;
        x2t = x2;
        x3t = x3;
        x4t = x4;
        x5t = x5;
        fcelu_old = wyrazenie.value();

        //qDebug()<<QString::number(alpha);


        x1 = x1 - alpha*derivative_x1;
        x2 = x2 - alpha*derivative_x2;
        x3 = x3 - alpha*derivative_x3;
        x4 = x4 - alpha*derivative_x4;
        x5 = x5 - alpha*derivative_x5;
        fcelu_new = wyrazenie.value();
        //rysujemy grafy
        x[0]=x1;
        y[0]=x2;
        ui->customPlot->addGraph();
        ui->customPlot->graph(i+1)->setData(x,y);
        ui->customPlot->graph(i+1)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot->graph(i+1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,Qt::red,Qt::red, 5));

        // add the arrow:
        QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
        arrow->start->setCoords(x1t,x2t);
        arrow->end->setCoords(x1, x2); // point to (4, 1.6) in x-y-plot coordinates
        arrow->setHead(QCPLineEnding::esSpikeArrow);



        ui->customPlot->replot();

        QCoreApplication::processEvents();

        //liczymy warunki stopu app
        Eps = 0;
        for(int j = 0 ; j < 5;j++){
            Eps += gradient[j]*gradient[j];
        }
        //
        Eps1 = 0;
        x1t = x1 - x1t;
        x2t = x2 - x2t;
        x3t = x3 - x3t;
        x4t = x4 - x4t;
        x5t = x5 - x5t;
        Eps1 = sqrt(x1t*x1t+x2t*x2t+x3t*x3t+x4t*x4t+x5t*x5t);
        //
        Eps2 = 0;
        Eps2 = abs(fcelu_new-fcelu_old);

        info = "it: " + QString::number(i+1) + ", eps: " + QString::number(Eps)+", eps1: " + QString::number(Eps1) + ", eps2: " + QString::number(Eps2)
                + ", f(x) = " + QString::number(fcelu_old);
        ui->tekst->append(info);

        if(Eps <= eps){
            koniec_algorytmu = 1;
        }
        if(Eps1 <= eps1){
            koniec_algorytmu = 2;
        }
        if(Eps2 <= eps2){
            koniec_algorytmu = 3;
        }


        if(koniec_algorytmu){
            break;
        }

    }

    switch (koniec_algorytmu) {
    case 1 :
        info = "Spełniono warunek eps";
        break;
    case 2 :
        info = "Spełniono warunek eps1";
        break;
    case 3 :
        info = "Spełniono warunek eps2";
        break;
    default:
        info = "";
        break;
    }
    ui->tekst->append(info);
    wynik = "x1 = " + QString::number(x1)+ ", x2 = " + QString::number(x2) + ", x3 = " + QString::number(x3)+ ", x4 = " + QString::number(x4) + ", x5 = " + QString::number(x5);
    ui->tekst->append( wynik);

    if(punktSiodlowy()){
        ui->tekst->append("<font color=\"Red\">Algorytm wyliczył punkt siodłowy. Zmień punk startowy.</font><br>");
    }


    ui->pWczytaj->setEnabled(true);
    ui->pStart->setEnabled(true);
    ui->doubleSpinBox->setEnabled(true);
    ui->doubleSpinBox_2->setEnabled(true);
    ui->doubleSpinBox_3->setEnabled(true);
    ui->doubleSpinBox_4->setEnabled(true);
    ui->doubleSpinBox_5->setEnabled(true);
}


double MainWindow::obliczAlpha(QVector<double> gradient,double dokladnosc){
    double a,b,k;
    a =0.0001;
    b = 1;
    k = (sqrt(5)-1)/2;
    //zapisujemy poczatkowe wartosc xn
    double x1t,x2t,x3t,x4t,x5t;
    x1t = x1;
    x2t = x2;
    x3t = x3;
    x4t = x4;
    x5t = x5;

    double xL = b - k * (b - a);
    double xR = a + k * (b - a);
    double fxl,fxr;

    while((b-a) > dokladnosc){
        x1 = x1t - xL*gradient[0];
        x2 = x2t - xL*gradient[1];
        x3 = x3t - xL*gradient[2];
        x4 = x4t - xL*gradient[3];
        x5 = x5t - xL*gradient[4];
        fxl = wyrazenie.value();

        x1 = x1t - xR*gradient[0];
        x2 = x2t - xR*gradient[1];
        x3 = x3t - xR*gradient[2];
        x4 = x4t - xR*gradient[3];
        x5 = x5t - xR*gradient[4];
        fxr = wyrazenie.value();

        if(fxl < fxr){
            b = xR;
            xR = xL;
            xL = b - k * (b-a);
        }
        else{
            a = xL;
            xL = xR;
            xR = a + k * (b - a);
        }
    }

    x1 = x1t;
    x2 = x2t;
    x3 = x3t;
    x4 = x4t;
    x5 = x5t;
    return (a + b)/2;
}




void MainWindow::on_iteracjeL_valueChanged(int arg1)
{
    L = arg1;
}


void MainWindow::rysujWykresPoziomicowy(){

    //przepisujemy poczatkowe wartosci
    double x1t,x2t;
    x1t = x1;
    x2t = x2;

    ui->customPlot->axisRect()->setupFullAxesBox(true);
    // set up the QCPColorMap:
    //colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);

    int nx = 500;
    int ny = 500;


    colorMap->data()->clear();
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(xw1, xw2), QCPRange(yw1, yw2)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions

    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    double x, y, z;
    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
        for (int yIndex=0; yIndex<ny; ++yIndex)
        {
            colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
            x1 = x;
            x2 = y;
            z = wyrazenie.value();
            //ui->tekst->append(QString::number());
            colorMap->data()->setCell(xIndex, yIndex, z);
        }
    }
    x1 = x1t;
    x2 = x2t;

    // add a color scale



    //colorScale = new QCPColorScale(ui->customPlot);
    if(!ui->customPlot->plotLayout()->hasElement(0,1)){
        ui->customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    }

    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    colorScale->axis()->setLabel("wartość funkcji celu");

    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpPolar);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    // marginGroup = new QCPMarginGroup(ui->customPlot);
    ui->customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();



}

bool MainWindow::punktSiodlowy()
{
//    if(ui->pochodna->text() == ""){ //jezeli chcemy sprawdzac hesjan to musimy wpisac cos do pola df(x)/dx1
//        return false;
//    }

//    double wynik;
//    wyrazenie_t wyrazenieP;
//    parser_t parserP;

//    wyrazenieP.register_symbol_table(zmienne);
//    parserP.compile(ui->pochodna->text().toUtf8().constData(),wyrazenieP);

//    wynik = exprtk::second_derivative(wyrazenie,x1) * exprtk::second_derivative(wyrazenie,x2) - exprtk::derivative(wyrazenieP,x2) * exprtk::derivative(wyrazenieP,x2);
//    //qDebug()<<"metoda1: " + QString::number(wynik);
//    qDebug()<<"metoda1: " + QString::number(wynik);
//    qDebug()<<"metoda2: " + QString::number(pochodna_dx_dy(0.0001,0.0001));
//    if(wynik < 0){
//        return true;
//    }

    double wynik;

    wynik = exprtk::second_derivative(wyrazenie,x1) * exprtk::second_derivative(wyrazenie,x2) - pochodna_dx_dy(0.0001,0.0001)*pochodna_dx_dy(0.0001,0.0001);
    qDebug()<<"detH: " + QString::number(wynik);
    if(wynik < 0){
        return true;
    }

    return false;
}

double MainWindow::pochodna_dx_dy(double h1, double h2)
{
    double x1_t,x2_t,f11,fm1,f1m,fmm;
    x1_t = x1;
    x2_t = x2;

    //liczymy f11
    x1 += h1;
    x2 += h2;
    f11 = wyrazenie.value();
    x1 = x1_t;
    x2 = x2_t;
    //liczymy f1m
    x1 += h1;
    x2 -= h2;
    f1m = wyrazenie.value();
    x1 = x1_t;
    x2 = x2_t;
    //liczymy fm1
    x1 -= h1;
    x2 += h2;
    fm1 = wyrazenie.value();
    x1 = x1_t;
    x2 = x2_t;
    //liczymy fmm
    x1 -= h1;
    x2 -= h2;
    fmm = wyrazenie.value();
    x1 = x1_t;
    x2 = x2_t;

    return (f11-f1m-fm1+fmm)/(4*h1*h2);
}

void MainWindow::on_epsilon_valueChanged(double arg1)
{
    eps = arg1;
}

void MainWindow::on_epsilon1_valueChanged(double arg1)
{
    eps1 = arg1;
}

void MainWindow::on_epsilon2_valueChanged(double arg1)
{
    eps2 = arg1;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index){
        ui->funkcjaCelu->setText(wzory[index]);

    }
}


void MainWindow::on_warstwaX1_valueChanged(double arg1)
{
    xw1 = arg1;
}

void MainWindow::on_warstwaY1_valueChanged(double arg1)
{
    yw1 = arg1;
}

void MainWindow::on_warstwaX2_valueChanged(double arg1)
{
    xw2 = arg1;
}

void MainWindow::on_warstwaY2_valueChanged(double arg1)
{
    yw2 = arg1;
}


