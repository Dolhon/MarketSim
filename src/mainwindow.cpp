/*
 * Created by: Sami Antila
 * Created: 2013/10/25
 * Last updated: 2015/10/25
 * Copyright 2013+, Sami Antila
 * Description:
 * Contains all Qt MainWindow UI and translation logic
*/

#include "mainwindow.h"


//### Qt GUI ###

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //costructor
    p_simu_changed = false; //simulator variables changes -> false
    ui->setupUi(this);
    this->language = 0; //set language 0 = English
    this->alustaSimu(); //initiates the simulator
    this->translate_to_english(); //translates UI to english
    QTimeLine *timer = new QTimeLine(60000); //
    timer->setFrameRange(0, 100); //timer frame range
    timer->setLoopCount(10000); //timer loop count
    timer->start(); //start timer
    connect(timer, SIGNAL(frameChanged(int)),this,SLOT(on_pushButton_clicked()));// signal slot
}
MainWindow::~MainWindow()
{
    delete ui;
}

//translate_to_english() - Changes GUI text to English.
void MainWindow::translate_to_english()
{
    this->language = 0; //sets active language to 0 = English
    this->setWindowTitle(tr("MartketSim")); //Changes window title
    ui->grpBoxCashRegister->setTitle(tr("Cash registers")); //sets every gui item text to English
    ui->grpBoxCharts->setTitle("Charts");
    ui->grpBoxCustomerDensityMultipliers->setTitle("Customer density multipliers (%)");
    ui->grpBoxCustomers->setTitle("Customers");
    ui->checkBox_kassa_auki->setText(tr("Open cash registers"));
    ui->checkBox_kassa_sulku->setText(tr("Close cash registers"));
    ui->label->setText(tr("Opening limit: "));
    ui->label_2->setText(tr("Customer density: "));
    ui->label_4->setText(tr("Service density: "));
    ui->pushButton->setText(tr("Simulate"));
    //ui->label_atiheys->setText(tr("Customer density"));
    //ui->label_palvelutiheys->setText(tr("Service density"));
    p_simu_changed = true;
    on_pushButton_clicked(); //runs simulation again to refresh text in chart
}

//translate_to_finnish() - Changes GUI text to Finnish.
void MainWindow::translate_to_finnish()
{
    this->language = 1;
    //this->setWindowTitle(tr("MartketSim test"));
    ui->grpBoxCashRegister->setTitle(tr("Kassat"));
    ui->grpBoxCharts->setTitle("Kuvaajat");
    ui->grpBoxCustomerDensityMultipliers->setTitle("Asiakkakastiheyden kertoimet (%)");
    ui->grpBoxCustomers->setTitle("Asiakkaat");
    ui->checkBox_kassa_auki->setText(tr("Avaa kassoja"));
    ui->checkBox_kassa_sulku->setText(tr("Sulje kassoja"));
    ui->label->setText(tr("Avaus raja"));
    ui->label_2->setText(tr("Asiakas tiheys"));
    ui->label_4->setText(tr("Palvelu tiheys"));
    ui->pushButton->setText(tr("Simuloi"));
    p_simu_changed = true;
    on_pushButton_clicked(); //runs simulation again to refresh text in chart
}

//on_pushButton - Simulate/run button clicked. Runs the simulation. Disabled, only used when language changed.
void MainWindow::on_pushButton_clicked()
{
    if (this->p_simu_changed) {
        if (ui->checkBox_kassa_auki->isChecked()) {
            p_simu.kassan_avaus = ui->slider_kassa_auki_raja->value();  // (min) raja täyttyy -> avaa uusia kassoja
        } else {
            p_simu.kassan_avaus = 100000;
        }
        if (ui->checkBox_kassa_sulku->isChecked()) {
            p_simu.kassan_sulku_raja = 0.01;    // (min) raja täyttyy -> sulkee kassan
        } else {
             p_simu.kassan_sulku_raja = 0;
        }

        /*p_simu.asiakas_tiheys = ui->slider_atiheys->value();
        p_simu.palvelu_tiheys = ui->slider_palvelutiheys->value();
        p_simu.asiakas_kertoimet[0] = ((float)ui->slider_kerroin_1->value())/100+1;
        p_simu.asiakas_kertoimet[1] = ((float)ui->slider_kerroin_2->value())/100+1;
        p_simu.asiakas_kertoimet[2] = ((float)ui->slider_kerroin_3->value())/100+1;
        p_simu.asiakas_kertoimet[3] = ((float)ui->slider_kerroin_4->value())/100+1;
        p_simu.asiakas_kertoimet[4] = ((float)ui->slider_kerroin_5->value())/100+1;
        p_simu.asiakas_kertoimet[5] = ((float)ui->slider_kerroin_6->value())/100+1;

        */
        this->paivitaChart(ui->customPlot);
        ui->customPlot->replot();
        this->p_simu_changed = false;
    }
}

//initiate simulation
void MainWindow::alustaSimu()
{

    p_simu.kassan_avaus = ui->slider_kassa_auki_raja->value();  // (min) limit exceeds -> open new cash registers
    p_simu.kassan_sulku_raja = 0.01;    // (min) raja täyttyy -> sulkee kassan
    float kerroin = 1;
    p_simu.pituus = simu_pituus;
    p_simu.alku_kello = 800;            // mihin klo aikaan simu aloitetaan
    p_simu.asiakas_tiheys = ui->slider_atiheys->value();          // kassoille saapuu asiakasta/min
    p_simu.palvelu_tiheys = ui->slider_palvelutiheys->value();          // asiakasta/min/kassa valmistuu
    p_simu.palvelu_aika = 1/p_simu.palvelu_tiheys;

    for (int i = 0;i<kerroin_taulu_MAX;i++)
    {
        p_simu.asiakas_kertoimet[i] = kerroin;
        kerroin = kerroin + 0.1;
    }
    p_simu.kaavio_tyyppi = ui->comboBox_kuvaaja->currentIndex();

    alustaChart(ui->customPlot);
}

//update Chart
void MainWindow::paivitaChart(QCustomPlot *customPlot)
{
    switch(p_simu.kaavio_tyyppi) {
        case 0 :    Chart0(customPlot);
                    break;
        case 1 :    Chart1(customPlot);
                    break;
        case 2 :    Chart2(customPlot);
                break;
        default :   Chart0(customPlot);
                    break;
    }
}

//initiate Chart
void MainWindow::alustaChart(QCustomPlot *customPlot)
{
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    Simulaatio *simu;
    simu = new Simulaatio (p_simu);
    Tilanne * current = simu->palauta_first();

    QVector<double> x(simu_pituus), y(simu_pituus), y2(simu_pituus); // 0..simu_pituus

    double y_max = 0;
    double y2_max = 0;

    for (int i=0; i<simu_pituus; ++i)
    {
        x[i] = i;       //x = aika = i (minuutteina)
        y[i] = current->palauta_max_jono_a();   //y = max jonotus aika (minuutteina)
        if (y_max < y[i])
        {
            y_max = y[i];
        }
        y2[i] = current->palauta_kassoja();
        if (y2_max < y2[i])
        {
            y2_max = y2[i];
        }
        current = current->palauta_next();
    }

    // luodaan kuvaaja ja asetetaan data
    customPlot->addGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->graph(0)->setData(x, y);

    // luodaan 2. kuvaaja ja asetetaan data
    //2. kuvaaja
    customPlot->addGraph(customPlot->xAxis, customPlot->yAxis2);
    customPlot->graph(1)->setData(x, y2);
    QPen greenDotPen;
    greenDotPen.setColor(QColor(107, 142, 35, 150)); //107-142-35 (30, 40, 255, 150)
    greenDotPen.setStyle(Qt::DotLine);
    greenDotPen.setWidthF(2);
    customPlot->graph(1)->setPen(greenDotPen);
    customPlot->yAxis2->setVisible(true);
    switch(language){
        case 0:
            customPlot->graph(0)->setName("MAX time in queue (min)");
            //customPlot->graph(0)->setName("Queue time");

            // akseleiden otsikko asetukset
            customPlot->xAxis->setLabel("Time (min)");
            customPlot->yAxis->setLabel("MAX time in queue (min)");
            customPlot->yAxis2->setLabel("Cash register amount");
            customPlot->graph(1)->setName("Cash register amount");
            break;
        case 1:
            customPlot->graph(0)->setName("MAX jonotus aika (min)");
            // akseleiden otsikko asetukset
            customPlot->xAxis->setLabel("Aika (min)");
            customPlot->yAxis->setLabel("MAX jonotusaika (min)");
            customPlot->yAxis2->setLabel("Kassa lkm");
            customPlot->graph(1)->setName("Kassa lkm");
            break;
    }
    // akseleiden pituus asetukset
    customPlot->xAxis->setRange(0, simu_pituus-1);
    customPlot->yAxis->setRange(0, 1.4*y_max);
    customPlot->yAxis2->setRange(0, 9+y2_max);
    customPlot->yAxis2->setTickLength(0, 1);
    customPlot->yAxis2->setSubTickLength(0, 0);

    customPlot->legend->setVisible(true);

    //statusBar()->clearMessage();
    ui->customPlot->replot();

}

//setup chart 0
void MainWindow::Chart0(QCustomPlot *customPlot) {
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    Simulaatio *simu;
    simu = new Simulaatio (p_simu);
    Tilanne * current = simu->palauta_first();

    QVector<double> x(simu_pituus), y(simu_pituus), y2(simu_pituus); // 0..simu_pituus

    double y_max = 0;
    double y2_max = 0;

    for (int i=0; i<simu_pituus; ++i)
    {
        x[i] = i;       //x = aika = i (minuutteina)
        y[i] = current->palauta_max_jono_a();   //y = max jonotus aika (minuutteina)
        if (y_max < y[i])
        {
            y_max = y[i];
        }
        y2[i] = current->palauta_kassoja();
        if (y2_max < y2[i])
        {
            y2_max = y2[i];
        }
        current = current->palauta_next();
    }

    // luodaan kuvaaja ja asetetaan data
    customPlot->graph(0)->setData(x, y);
    //customPlot->graph(0)->setName("maxjonotusaika (min)");

    // luodaan 2. kuvaaja ja asetetaan data
    //2. kuvaaja
    //customPlot->addGraph(customPlot->xAxis, customPlot->yAxis2);
    customPlot->graph(1)->setData(x, y2);

    // akseleiden otsikko asetukset

    // akseleiden pituus asetukset
    customPlot->xAxis->setRange(0, simu_pituus-1);
    customPlot->yAxis->setRange(0, 1.4*y_max);
    customPlot->yAxis2->setRange(0, 9+y2_max);
    customPlot->yAxis2->setTickLength(0, 1);
    customPlot->yAxis2->setSubTickLength(0, 1);
    //customPlot->yAxis2->setTickStep(1);

    switch(language){
        case 0:
            customPlot->graph(0)->setName("MAX time in queue (min)");
            //customPlot->graph(0)->setName("Queue time");

            // akseleiden otsikko asetukset
            customPlot->xAxis->setLabel("Time (min)");
            customPlot->yAxis->setLabel("MAX time in queue (min)");
            customPlot->yAxis2->setLabel("Cash register amount");
            customPlot->graph(1)->setName("Cash register amount");
            break;
        case 1:
            customPlot->graph(0)->setName("MAX jonotus aika (min)");
            // akseleiden otsikko asetukset
            customPlot->xAxis->setLabel("Aika (min)");
            customPlot->yAxis->setLabel("MAX jonotusaika (min)");
            customPlot->yAxis2->setLabel("Kassa lkm");
            customPlot->graph(1)->setName("Kassa lkm");
            break;
    }
    customPlot->legend->setVisible(true);

    //customPlot->yAxis2->setLabel("Kassa lkm");
    customPlot->yAxis2->setVisible(true);

}

//setup chart 1
void MainWindow::Chart1(QCustomPlot *customPlot) {
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    Simulaatio *simu;
    simu = new Simulaatio (p_simu);
    Tilanne * current = simu->palauta_first();

    int p_simu_pituus_10 = 11;

    QVector<double> x(p_simu_pituus_10), y(p_simu_pituus_10), y2(p_simu_pituus_10);// 0..simu_pituus

    double y_max = 0;
    double y2_max = 0;

    for (int i=0; i<p_simu_pituus_10; ++i)
    {
        x[i] = i;
        y[i] = p_simu.asiakas_tiheys-i;
        if (y_max < y[i])
        {
            y_max = p_simu.asiakas_tiheys-i;
        }
        y2[i] = -10;
        if (y2_max < y2[i])
        {
            y2_max = -10;
        }
        current = current->palauta_next();
    }

    // luodaan kuvaaja ja asetetaan data
    customPlot->graph(0)->setData(x, y);

    // luodaan 2. kuvaaja ja asetetaan data
    //2. kuvaaja
    //customPlot->addGraph(customPlot->xAxis, customPlot->yAxis2);
    customPlot->graph(1)->setData(x, y2);

    // akseleiden otsikko asetukset

    // akseleiden pituus asetukset
    customPlot->xAxis->setRange(0, p_simu_pituus_10-1);
    customPlot->yAxis->setRange(0, 1.4*y_max);
    customPlot->yAxis2->setRange(0, 9+y2_max);
    customPlot->yAxis2->setTickLength(0, 1);
    customPlot->yAxis2->setSubTickLength(0, 1);
    //customPlot->yAxis2->setTickStep(1);
    customPlot->yAxis->setLabel("Maksimijonotusaika (min)");
    customPlot->xAxis->setLabel("Palvelutiheys (a/min)");
    switch(language){
        case 0:
            // akseleiden otsikko asetukset
            customPlot->yAxis->setLabel("Max time in queue (min)");
            customPlot->xAxis->setLabel("Service density (customer/min)");
            break;
        case 1:
            // akseleiden otsikko asetukset
            customPlot->yAxis->setLabel("Maksimijonotusaika (min)");
            customPlot->xAxis->setLabel("Palvelutiheys (asiakasta/min)");
            break;
    }
    customPlot->legend->setVisible(false);
    customPlot->yAxis2->setVisible(false);
}

//setup chart 2
void MainWindow::Chart2(QCustomPlot *customPlot) {
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    Simulaatio *simu;
    simu = new Simulaatio (p_simu);
    Tilanne * current = simu->palauta_first();

    int p_simu_pituus_10 = 11;

    QVector<double> x(p_simu_pituus_10), y(p_simu_pituus_10), y2(p_simu_pituus_10);// 0..simu_pituus

    double y_max = 0;
    double y2_max = 0;

    for (int i=0; i<p_simu_pituus_10; ++i)
    {
        x[i] = i;
        y[i] = i-p_simu.palvelu_tiheys;
        if (y_max < y[i])
        {
            y_max = i-p_simu.palvelu_tiheys;
        }
        y2[i] = -10;
        if (y2_max < y2[i])
        {
            y2_max = -10;
        }
        current = current->palauta_next();
    }

    // luodaan kuvaaja ja asetetaan data
    customPlot->graph(0)->setData(x, y);
    //customPlot->graph(0)->setName("maxjonotusaika (min)");

    // luodaan 2. kuvaaja ja asetetaan data
    //2. kuvaaja
    //customPlot->addGraph(customPlot->xAxis, customPlot->yAxis2);
    customPlot->graph(1)->setData(x, y2);

    // akseleiden otsikko asetukset

    // akseleiden pituus asetukset
    customPlot->xAxis->setRange(0, p_simu_pituus_10-1);
    customPlot->yAxis->setRange(0, 1.4*y_max);
    customPlot->yAxis2->setRange(0, 9+y2_max);
    customPlot->yAxis2->setTickLength(0, 1);
    customPlot->yAxis2->setSubTickLength(0, 1);
    //customPlot->yAxis2->setTickStep(1);
    switch(language){
        case 0:
            // akseleiden otsikko asetukset
            customPlot->yAxis->setLabel("Max time in queue (min)");
            customPlot->xAxis->setLabel("Customer density (customer/min)");
            break;
        case 1:
            // akseleiden otsikko asetukset
            customPlot->yAxis->setLabel("Maksimijonotusaika (min)");
            customPlot->xAxis->setLabel("Asiakastiheys (a/min)");
            break;
    }
    //customPlot->graph(0)->setName("Maxjonoaika/palvelutiheys");
    customPlot->legend->setVisible(false);
    customPlot->yAxis2->setVisible(false);
}

/*
 * FORMS actions
*/
void MainWindow::on_slider_palvelutiheys_valueChanged(int value)
{
    ui->label_palvelutiheys->setNum(value);
    p_simu.palvelu_tiheys = value;
    p_simu_changed = true;
}

void MainWindow::on_slider_kerroin_1_valueChanged(int value)
{
    ui->label_kerroin_1->setNum(value);
    p_simu.asiakas_kertoimet[0] = ((float)value)/100+1;
    p_simu_changed = true;
}

void MainWindow::on_slider_kerroin_2_valueChanged(int value)
{
    ui->label_kerroin_2->setNum(value);
    p_simu.asiakas_kertoimet[1] = ((float)value)/100+1;
    p_simu_changed = true;
}

void MainWindow::on_slider_kerroin_3_valueChanged(int value)
{
    ui->label_kerroin_3->setNum(value);
    p_simu.asiakas_kertoimet[2] = ((float)value)/100+1;
    p_simu_changed = true;
}

void MainWindow::on_slider_kerroin_4_valueChanged(int value)
{
    ui->label_kerroin_4->setNum(value);
    p_simu.asiakas_kertoimet[3] = ((float)value)/100+1;
    p_simu_changed = true;
}

void MainWindow::on_slider_kerroin_5_valueChanged(int value)
{
    ui->label_kerroin_5->setNum(value);
    p_simu.asiakas_kertoimet[4] = ((float)value)/100+1;
}

void MainWindow::on_slider_kerroin_6_valueChanged(int value)
{
    ui->label_kerroin_6->setNum(value);
    p_simu.asiakas_kertoimet[5] = ((float)value)/100+1;
    p_simu_changed = true;
}

void MainWindow::on_slider_atiheys_valueChanged(int value)
{
    ui->label_atiheys->setNum(value);
    p_simu.asiakas_tiheys = value;
    p_simu_changed = true;
}

void MainWindow::on_actionEnglish_triggered()
{
    translate_to_english();
    ui->actionEnglish->setEnabled(false);
    ui->actionFinnish->setEnabled(true);
}

void MainWindow::on_actionFinnish_triggered()
{
    translate_to_finnish();
    ui->actionEnglish->setEnabled(true);
    ui->actionFinnish->setEnabled(false);
}

void MainWindow::on_comboBox_kuvaaja_currentIndexChanged(int index)
{
    p_simu.kaavio_tyyppi = index;
    p_simu_changed = true;
}

void MainWindow::on_slider_kassa_auki_raja_valueChanged(int value)
{
    ui->value_kassa_auki_raja->setNum(value);
    p_simu.kassan_sulku_raja = value;
    p_simu_changed = true;
}
