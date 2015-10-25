/*
 * Created by: Sami Antila
 * Created: 2013/10/25
 * Last updated: 2015/10/25
 * Copyright 2013+, Sami Antila
 * Description:
 * Takes parameters struct. Uses parameters to run the simulation with wanted length.
 * Palauta_first() method returns pointer to first moment of simulation.
*/
#ifndef SIMULAATIO_H
#define SIMULAATIO_H
#include <QTimer>
#include "tilanne.h"
using namespace std;

const int kerroin_taulu_MAX = 6;        //amount of customer density multipliers
const int simu_pituus = 360;            //simulator lenght(min) 360 = 6h

struct parametrit{
    int pituus;                         // simulation lenght (time: mins)
    int alku_kello;                     // simulation starting time (1100 = 11:00am) TODO limit minutes to 59
    float asiakas_tiheys;				// how many customers arrive to queueue (customers/min)
    float palvelu_tiheys;				// how many cucustomers served/cash register in one minute
    float palvelu_aika;                 // Service density
    float kassan_avaus;                 // (min) limit exceeds -> open new cash registers
    float kassan_sulku_raja;			// (min) limit exceeds -> close cash register
    float asiakas_kertoimet[kerroin_taulu_MAX];  //Customer density multipliers
    int kaavio_tyyppi;                  //type of Chart

};

class Simulaatio {
public:
    Simulaatio(parametrit);
    void suorita();							//run simulaation
    void tulosta();							//print simulation
    Tilanne * palauta_first();              //return first minute of simulation
    ~Simulaatio();                          //destructor
private:
    Tilanne* first;							// pointer to first minute/moment of simulation
    int alku_kello;							// simulation starting time (1100 = 11:00am) TODO limit minutes to 59
    int pituus;								// simulation lenght (time: mins, 60 = 1h)
    float asiakas_tiheys;					// how many customers arrive to queueue (customers/min)
    float palvelu_tiheys;					// how many cucustomers served/cash register in one minute
    float palvelu_aika;						// kassoille saapuu asiakasta/min
    float kassan_avaus_raja;				// (min) limit exceeds -> open new cash registers
    float kassan_sulku_raja;				// (min) limit exceeds -> close cash register
    float asiakas_kertoimet[kerroin_taulu_MAX]; //Customer density multipliers
};

#endif // SIMULAATIO_H
