# MarketSim
Queue simulator

This simulator can be used as a tool to estimate the amount of service and space needed to meet the demand. Including amount of cash register, space for the queues and the amount of staff at given time. See how customer density affects the queue length and the maximum waiting time.

## Introduction
The aim of this project is to make a working cash register queue simulator even better by refreshing old project to more representative state. Which will showcase development skills to future employers. The aim is also to gain experience in graphical user interface design and development in Qt.

Project originally started in 2013 and was part of object-oriented programming 2 and algorithms courses. Simulation was first created in excel, then created as C++ console application and finally bolted into Qt for graphical user interface. The original project had problems with Qt and knowledge about Qt was lacking.

The objective of the simulator is to see how the density of customers affects to queue length and the maximum waiting time. Also to create a rule which keeps the maximum waiting time in given tolerances by increasing and decreasing the amount of opened cash registers. This simulator can be used as a tool to estimate the amount of service and space needed to meet the demand. Including amount of cash register, space for the queues and the amount of staff at given time.

This project doesn’t concentrate in the algorithm, but more to improving the user experience. This includes localization, improving the Qt implementations and making the graphical user interface to work in real-time.

## Situation

![alt text](https://github.com/Dolhon/MarketSim/blob/master/img/situation.JPG "")

Problem of demand and service.

Simulation tries to answer on how the density of customers affects to queue length and the maximum waiting time. Simulation tries to create a rule which keeps the maximum waiting time in given tolerances by increasing and decreasing the amount of opened cash registers. 

## Technologies

Excel
Visual Studio 2010
QtCreator
Qt 5.1.1 UI Framework - http://qt-project.org/	
QCustomPlot 1.1.0 Qt C++ widget for plotting - http://www.qcustomplot.com

## Console application

![alt text](https://github.com/Dolhon/MarketSim/blob/master/img/console.JPG "")

## GUI Prototypes

![alt text](https://github.com/Dolhon/MarketSim/blob/master/img/guiproto.JPG "")

## GUI Version 0.7

![alt text](https://github.com/Dolhon/MarketSim/blob/master/img/gui07.JPG "")

## GUI Version 1.0

![alt text](https://github.com/Dolhon/MarketSim/blob/master/img/gui10.JPG "")

## Localization

There were research about translating the application from Finnish to English. QtTranslator was clearly the way to go. After much tries and prototyping it proved too problematic with the way old version was built and instead more simpler hard coded method was used. This method works and suits the need for this project, but it doesn’t scale well. If more features or languages would be implemented there should be added tr() function around every string in the application.

## Real-time implementation

Using QtTime and signal-slot application check every second if the parameters have changed. If the parameters changed flag is true, simulation will be updated, if the flag is false simulation update will be skipped.

## Future development ideas

- Delay for opening/closing cash registers
- Monte Carlo method
- Saving results to excel
- Using RNG with customer and service density

## Classes

1. Situation
  * Situation in simulation.
  * Saves all data of current moment.
  * Inner part of algorithm has been placed here.
  * Return processed data.
  * Two constructors for simulation time 0 and >0.
2. Simulation
  * Initialization of simulation
  * The simulation for-loop.
  * Simulation algorithms and array of Situations(time).
  * Simulation creates new moment(Situation) every minute until given time limit.
3. MainWindow
  * Handles Qt, Chart(QCustomPlot) and GUI related code
4. QCustomPlot
  * QCustomPlot 1.1.0 Qt C++ widget for plotting
  * Used for charts.

