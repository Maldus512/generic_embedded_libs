/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: digin.h                                                           */
/*                                                                            */
/*      gestione INPUT                                                        */
/*                                                                            */
/*  Autore: Massimo ZANNA & Maldus (Mattia MALDINI)                           */
/*                                                                            */
/*  Data  : 22/07/2003      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 12/05/2018      REV  : 01.0                                       */
/*                                                                            */
/******************************************************************************/

#ifndef DIGIN_H
#define DIGIN_H

typedef struct _input_filter
{
    unsigned int is_counter;
    unsigned int active;
    unsigned int previous_input;
    unsigned int filters[16];
    unsigned int value[16];
} debounce_filter_t;


static inline int digital_read(int i, debounce_filter_t *di_filter)
{
    return di_filter->value[i];
}

void init_digin_filter(debounce_filter_t *filter, unsigned short active, unsigned short type);
int  digin_filter(debounce_filter_t *filter, unsigned short input, unsigned long debounce);

#endif /* DIGIN_H */
