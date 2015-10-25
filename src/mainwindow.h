/*
 * Created by: Sami Antila
 * Created: 2013/10/25
 * Last updated: 2015/10/25
 * Copyright 2013+, Sami Antila
 * Description:
 * Contains all Qt MainWindow UI and translation logic
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "qcustomplot.h"
#include "simulaatio.h"
#include "QMainWindow"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <QTimeLine>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void alustaSimu(); //Initiate simulator
    void translate_to_english(); //Change GUI text to English
    void translate_to_finnish(); //Change GUI text to Finnish
    void paivitaChart(QCustomPlot *customPlot); //Update QCustomPlot Chart (Main chart)
    void alustaChart(QCustomPlot *customPlot); //Initiate QCustomPlot Chart (Main chart)
    void Chart0(QCustomPlot *customPlot); //QCustomPlot Chart 0 used
    void Chart1(QCustomPlot *customPlot); //QCustomPlot Chart 1 disabled
    void Chart2(QCustomPlot *customPlot); //QCustomPlot Chart 2 disabled
    parametrit palauta_p_simu(); //Return simulation (parameter struct)

private slots:
    void on_slider_kassa_auki_raja_valueChanged(int value); //UI - Cash register opening limit valueChanged
    void on_pushButton_clicked(); //UI - Run/Simulater button clicked - disabled (simulator runs real-time now)
    void on_slider_palvelutiheys_valueChanged(int value); //UI - ServiceDensity slider valueChanged
    void on_slider_kerroin_1_valueChanged(int value); //UI - CustomerDensity multiplied sliders valueChanged 1-6
    void on_slider_kerroin_2_valueChanged(int value);
    void on_slider_kerroin_3_valueChanged(int value);
    void on_slider_kerroin_4_valueChanged(int value);
    void on_slider_kerroin_5_valueChanged(int value);
    void on_slider_kerroin_6_valueChanged(int value);
    void on_slider_atiheys_valueChanged(int value); //UI - CustomerDensity slider valueChanged
    void on_actionEnglish_triggered(); //UI - Trigger translate ui to English
    void on_actionFinnish_triggered(); //UI - Trigger translate ui to Finnish
    void on_comboBox_kuvaaja_currentIndexChanged(int index); //UI - Change Chart type IndexChanged

private:
    Ui::MainWindow *ui; //UI MainWindow pointer
    parametrit p_simu; //parameters struct p_simu
    //QTranslator translator; //disabled
    int language; //0=en, 1=fi //language variable to save current language
    bool p_simu_changed; //boolean used to check if simulator variables have changed, if used simulation will ran.
};





#endif // MAINWINDOW_H
