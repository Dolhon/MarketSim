/*
 * Created by: Sami Antila
 * Created: 2013/10/25
 * Last updated: 2015/10/25
 * Copyright 2013+, Sami Antila
 * Description:
 * Tilanne = Situation in simulation, which shows data of that specific moment (1 situation/min)
*/

#include "tilanne.h"
using namespace std;

//simu_time = 0 konstruktori
Tilanne::Tilanne(int p_alku_kello, float p_asiakas_tiheys,
                        float p_palvelu_tiheys, float p_kassan_avaus_raja,
                        float p_palvelu_aika, float p_asiakas_kerroin) {

    this->next = NULL;
    this->prev = NULL;
    this->asiakas_kerroin = p_asiakas_kerroin;
    this->kello = p_alku_kello;
    this->simu_aika = 0;
    this->asiakasta_per_min = this->asiakas_kerroin*p_asiakas_tiheys;
    this->asiakasta_yht = p_asiakas_tiheys*this->asiakas_kerroin;
    this->kassoja = 1;
    this->palvelu_tiheys = p_palvelu_tiheys;

    //tarkista onko asiakkaita ->jono
    if (this->asiakasta_per_min==0) {
        this->asiakkaita_palveltu = 0;

    //tarkista onko asiakkaita enemmän kuin pystytään palvelemaan aika välillä ->jono
    } else if (this->asiakasta_per_min > this->kassoja*p_palvelu_tiheys) {
        this->asiakkaita_palveltu = this->kassoja*p_palvelu_tiheys;
    } else {
        this->asiakkaita_palveltu = this->asiakasta_per_min;
    }

    //tarkista onko asikkaita enemmän kuin ehditään palvella ->jono
    if (this->asiakasta_yht < this->asiakkaita_palveltu) {
        this->max_asiakkaita_jonossa = 0;
    } else {
        this->max_asiakkaita_jonossa =
        this->asiakasta_yht - this->asiakkaita_palveltu;
    }

    this->max_jonotus_aika = this->max_asiakkaita_jonossa*p_palvelu_aika;

}

//simu_time > 0 konstruktori
Tilanne::Tilanne(Tilanne* p_prev, int p_alku_kello, float p_asiakas_tiheys,
                    float p_palvelu_tiheys, float p_kassan_avaus_raja,
                    float p_palvelu_aika, float p_kassan_sulku_raja, int p_simu_aika, float p_asiakas_kerroin) {

    this->next = NULL;
    this->prev = p_prev;

        this->asiakas_kerroin = p_asiakas_kerroin;
        this->kello = p_alku_kello + p_simu_aika;
        this->palvelu_tiheys = p_palvelu_tiheys;
        this->simu_aika = p_simu_aika;
        float asiakasta_per_min_tmp = this->asiakas_kerroin * p_asiakas_tiheys;

        //luodaan +/- 25% satunnaine nvaihtelu asiakasta/min luvulle
        float tole = 0.5*asiakasta_per_min_tmp; //tmp 40->20
        float korjaus = 0.25*asiakasta_per_min_tmp; //tmp 40 -> 10
        //RAND_MAX => 0-1
        float hajonta = ((float)rand()) / RAND_MAX * tole - korjaus; // tmp 40 -> 0-20 -10 -> -10 to 10
        this->asiakasta_per_min = asiakasta_per_min_tmp + hajonta;

        this->asiakasta_yht =	this->asiakasta_per_min
                                    + this->prev->palauta_a_yht();

        //tarkistaa tarvitaanko onko kassojen määrä sopiva
        if	(this->prev->palauta_max_jono_a() < p_kassan_sulku_raja) {
            if (this->prev->palauta_kassoja() == 1) {
                this->kassoja = 1;
            } else {
                this->kassoja = this->prev->palauta_kassoja() - 1;
            }
        } else if (this->prev->palauta_max_jono_a() > p_kassan_avaus_raja) {
            this->kassoja = this->prev->palauta_kassoja() + 1;
        } else {
            this->kassoja = this->prev->palauta_kassoja();
        }

        //tarkista onko asiakkaita
        if (this->asiakasta_per_min+this->prev->palauta_max_a_jono()==0) {
            this->asiakkaita_palveltu = 0;

        //tarkista onko asiakkaita enemmän kuin pystytään palvelemaan aika välillä
        } else if (	this->asiakasta_per_min+this->prev->max_asiakkaita_jonossa
                    > this->kassoja*p_palvelu_tiheys) {
            this->asiakkaita_palveltu =	this->kassoja*p_palvelu_tiheys
                                            + this->prev->palauta_a_palveltu();
        } else {
            this->asiakkaita_palveltu =	this->asiakasta_per_min
                                            + this->prev->palauta_a_palveltu();
        }

        //tarkista onko asikkaita enemmän kuin ehditään palvella
        if (this->asiakasta_yht < this->asiakkaita_palveltu) {
            this->max_asiakkaita_jonossa = 0;
        } else {
            this->max_asiakkaita_jonossa = this->asiakasta_yht - this->asiakkaita_palveltu;
        }

        this->max_jonotus_aika = this->max_asiakkaita_jonossa*p_palvelu_aika/this->kassoja;
}

void Tilanne::tulosta() {
    cout<<this->kello<<"\t"	<<this->asiakas_kerroin<<"\t"
        <<this->asiakasta_per_min<<"\t" <<this->kassoja<<"\t"
        <<this->asiakasta_yht<<"\t" <<this->asiakkaita_palveltu
        <<"\t"<<this->max_asiakkaita_jonossa<<"\t" <<this->max_jonotus_aika<<endl;
}

void Tilanne::muuta_next(Tilanne *p_next) {
    this->next = p_next;
}

Tilanne * Tilanne::palauta_next() { return this->next; }
int	Tilanne::palauta_kassoja(){ return this->kassoja; }
float Tilanne::palauta_a_yht(){ return this->asiakasta_yht; }
float Tilanne::palauta_max_jono_a(){ return this->max_jonotus_aika; }
float Tilanne::palauta_max_a_jono(){ return this->max_asiakkaita_jonossa; }
float Tilanne::palauta_a_palveltu(){ return this->asiakkaita_palveltu; }

int Tilanne::palauta_aika() {
    return this->kello;
}
float Tilanne::palauta_a_tiheys() {
    return (this->asiakasta_per_min);
}

float Tilanne::palauta_mja_p_palvk(){
    return (this->max_jonotus_aika/(this->kassoja*this->palvelu_tiheys));
}
float Tilanne::palauta_mja_p_atiheys(){
    return (this->max_jonotus_aika/this->asiakasta_per_min);
}

