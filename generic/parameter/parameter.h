/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: parameter_mac.h                                                   */
/*                                                                            */
/*      gestione IMPOSTAZIONE PARAMETRI MACCHINA                              */
/*                                                                            */
/*  Autore: Alessio Samiolo & Mattia Maldini & Massimo Zanna                  */
/*                                                                            */
/*  Data  : 30/06/2016      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 02/11/2018      REV  : 02.6                                       */
/*                                                                            */
/******************************************************************************/

#ifndef PARAMETER_MAC_H
#define PARAMETER_MAC_H

typedef enum
{
    compreso = 0,
    maggiore = 1,
    minore   = 2,
} tipocontrollo;



typedef enum
{
    unsigned_long = 0,
    signed_long   = 1,
    unsigned_char = 2,
    signed_char   = 3,
    unsigned_int  = 4,
    signed_int    = 5
} tipovariabile;



typedef struct __sch
{
    char         min, max, def;
    char *       pmin, *pmax;
    signed char *var;
} _sch;

typedef struct __uch
{
    unsigned char  min, max, def;
    unsigned char *pmin, *pmax;
    unsigned char *var;
} _uch;

typedef struct __sint
{
    int  min, max, def;
    int *pmin, *pmax;
    int *var;
} _sint;

typedef struct __uint
{
    unsigned int  min, max, def;
    unsigned int *pmin, *pmax;
    unsigned int *var;
} _uint;

typedef struct __sl
{
    long  min, max, def;
    long *pmin, *pmax;
    long *var;
} _sl;

typedef struct __ul
{
    unsigned long  min, max, def;
    unsigned long *pmin, *pmax;
    unsigned long *var;
} _ul;



typedef union __d {
    _sch  sch;
    _uch  uch;
    _sint sint;
    _uint uint;
    _sl   sl;
    _ul   ul;
} _data_type_t;

typedef struct _parameter_data_t
{
    tipovariabile t;
    tipocontrollo c;
    _data_type_t  d;

    int len;

    char ** format;
    char ***string_value;
    int     lvl;
    struct _parameter_data_t *(*runTime)(struct _parameter_data_t *, int);
} parameter_data_t;

int  string_to_display(parameter_data_t *ps, int len, int i, char *string, int language);
int  next_parameter(parameter_data_t *ps, int len, int *i, int level);
int  prev_parameter(parameter_data_t *ps, int len, int *i, int level);
int  number_of_parameters(parameter_data_t *ps, int len, int level);
int  first_parameter(parameter_data_t *ps, int len, int level);
void parameter_operator(parameter_data_t *ps, int len, int i, int mod);

#endif /* PARAMETER_MAC_H */
