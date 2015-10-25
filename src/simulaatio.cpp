/*
 * Created by: Sami Antila
 * Created: 2013/10/25
 * Last updated: 2015/10/25
 * Copyright 2013+, Sami Antila
 * Description:
 * Takes parameters struct. Uses parameters to run the simulation with wanted length.
 * Palauta_first() method returns pointer to first moment of simulation.
*/
#include "simulaatio.h"

//Constructor
Simulaatio::Simulaatio(parametrit p_simu) {

    this->alku_kello = p_simu.alku_kello;
    this->pituus = p_simu.pituus;
    this->asiakas_tiheys = p_simu.asiakas_tiheys;
    this->palvelu_tiheys = p_simu.palvelu_tiheys;
    this->palvelu_aika = p_simu.palvelu_aika;
    this->kassan_avaus_raja = p_simu.kassan_avaus;
    this->kassan_sulku_raja = p_simu.kassan_sulku_raja;

    for (int i=0;i<kerroin_taulu_MAX;i++) {
        asiakas_kertoimet[i] = p_simu.asiakas_kertoimet[i];
    }

    this->first = new Tilanne(this->alku_kello, this->asiakas_tiheys,
                        this->palvelu_tiheys, this->kassan_avaus_raja,
                              this->palvelu_aika, this->asiakas_kertoimet[0]);

    this->suorita();

}

//Run simulation
void Simulaatio::suorita() {

    Tilanne *current = this->first;
    Tilanne *prev = NULL;

    //luo taulukko kassan tilanteesta minuutin välein

    for(int aika=1;aika<this->pituus;aika++) {

        prev = current;
        current->muuta_next(new Tilanne(prev, this->alku_kello, this->asiakas_tiheys,
                            this->palvelu_tiheys, this->kassan_avaus_raja,
                            this->palvelu_aika, this->kassan_sulku_raja, aika,
                            this->asiakas_kertoimet[aika/(this->pituus/kerroin_taulu_MAX)]));
        cout<<this->asiakas_kertoimet[aika/(this->pituus/kerroin_taulu_MAX)]<<endl;
        current = current->palauta_next();
    }
}

//print simulation
void Simulaatio::tulosta() {

    Tilanne* current = this->first;

    cout<<"AIKA\tKERROIN\tA/MIN\tKASSOJA\tAYHT\tADONE\tAJONOS\tJONOA"<<endl;

    for(int aika=0;aika<this->pituus;aika++) {

        current->tulosta();
        current = current->palauta_next();
    }
}

//one moment in simulation
Tilanne * Simulaatio::palauta_first()
{
    return this->first;
}
