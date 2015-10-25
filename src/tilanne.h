/*
 * Created by: Sami Antila
 * Created: 2013/10/25
 * Last updated: 2015/10/25
 * Copyright 2013+, Sami Antila
 * Description:
 * Tilanne = Situation in simulation, which shows data of that specific moment (1 situation/min)
*/

#ifndef TILANNE_H
#define TILANNE_H
//#include "ui_mainwindow.h"
#include <iostream>


class Tilanne {
public:
    Tilanne(int,float,float,float,float,float); //situation TODO: make a struct
    Tilanne(Tilanne*,int,float,float,float,float,float,int,float); //situation TODO: make a struct
    void muuta_next(Tilanne*);      //Change next situation/moment in simulation
    int palauta_kassoja();          //return amount of cash registers
    float palauta_a_yht();          //return total amount of customers
    float palauta_max_jono_a();     // return max queue lenght (time)
    float palauta_max_a_jono();     //return max customers in queue
    float palauta_a_palveltu();     //return amount of customers served
    int palauta_aika();             //return time
    float palauta_a_tiheys();       //return customer density
    float palauta_mja_p_palvk();    //max queue time/service availability
    float palauta_mja_p_atiheys();  //max queue time/customer density
    Tilanne *palauta_next();        //return next
    void tulosta();                 //print moment/situation
    ~Tilanne();                     //destructor
private:
    Tilanne *next;                  //Doubly linked list next pointer
    Tilanne *prev;                  //Doubly linked lsit prev pointer
    int kello;                      //clock
    int	simu_aika;                  //time in simulation

    int kassoja;                    //amount of cash registers
    float max_asiakkaita_jonossa;   //max customers in queue
    float max_jonotus_aika;         //max queue time
    float asiakkaita_palveltu;      //amount of customers served
    float asiakasta_yht;            //total customers
    float palvelu_tiheys;           //service density

    float asiakas_kerroin;          //customer multiplier
    float asiakasta_per_min;        //customers/min
};

#endif // TILANNE_H
