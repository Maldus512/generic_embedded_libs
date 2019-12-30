/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: parameter_mac.c                                                   */
/*                                                                            */
/*      gestione IMPOSTAZIONE PARAMETRI MACCHINA                              */
/*                                                                            */
/*  Autore: Alessio Samiolo & Mattia Maldini & Massimo Zanna                  */
/*                                                                            */
/*  Data  : 30/06/2016      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 16/02/2019      REV  : 03.1                                       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "parameter.h"


/*parameter_data_t p[] = {
    (parameter_data_t){
        unsigned_char, compreso, {.uch = {0, 9, 0, (&parmac.lingua)}}, 1, lingue, NULL, UTENTE},     // C
};*/


int checkSCH(parameter_data_t p, char v);
int checkUCH(parameter_data_t p, unsigned char v);

int checkSINT(parameter_data_t p, int v);
int checkUINT(parameter_data_t p, unsigned int v);

int checkSL(parameter_data_t p, long v);
int checkUL(parameter_data_t p, unsigned long v);



int checkSCH(parameter_data_t p, char v)
{
    char min, max;
    min = p.d.sch.pmin != NULL ? *p.d.sch.pmin : p.d.sch.min;
    max = p.d.sch.pmax != NULL ? *p.d.sch.pmax : p.d.sch.max;

    switch (p.c)
    {
        case compreso:
            return (((v >= min) && (v <= max)) ? 1 : 0);

        case maggiore:
            return ((v >= min) ? 1 : 0);

        case minore:
            return ((v <= max) ? 1 : 0);

        default:
            return -1;
    }
}



int checkUCH(parameter_data_t p, unsigned char v)
{
    unsigned char min, max;
    min = p.d.sch.pmin != NULL ? *p.d.sch.pmin : p.d.sch.min;
    max = p.d.sch.pmax != NULL ? *p.d.sch.pmax : p.d.sch.max;

    switch (p.c)
    {
        case compreso:
            return (((v >= min) && (v <= max)) ? 1 : 0);

        case maggiore:
            return ((v >= min) ? 1 : 0);

        case minore:
            return ((v <= max) ? 1 : 0);

        default:
            return -1;
    }
}



int checkSINT(parameter_data_t p, int v)
{
    int min, max;
    min = p.d.sch.pmin != NULL ? *p.d.sch.pmin : p.d.sch.min;
    max = p.d.sch.pmax != NULL ? *p.d.sch.pmax : p.d.sch.max;

    switch (p.c)
    {
        case compreso:
            return (((v >= min) && (v <= max)) ? 1 : 0);

        case maggiore:
            return ((v >= min) ? 1 : 0);

        case minore:
            return ((v <= max) ? 1 : 0);

        default:
            return -1;
    }
}



int checkUINT(parameter_data_t p, unsigned int v)
{
    unsigned int min, max;
    min = p.d.sch.pmin != NULL ? *p.d.sch.pmin : p.d.sch.min;
    max = p.d.sch.pmax != NULL ? *p.d.sch.pmax : p.d.sch.max;

    switch (p.c)
    {
        case compreso:
            return (((v >= min) && (v <= max)) ? 1 : 0);

        case maggiore:
            return ((v >= min) ? 1 : 0);

        case minore:
            return ((v <= max) ? 1 : 0);

        default:
            return -1;
    }
}



int checkSL(parameter_data_t p, long v)
{
    long min, max;
    min = p.d.sch.pmin != NULL ? *p.d.sch.pmin : p.d.sch.min;
    max = p.d.sch.pmax != NULL ? *p.d.sch.pmax : p.d.sch.max;

    switch (p.c)
    {
        case compreso:
            return (((v >= min) && (v <= max)) ? 1 : 0);

        case maggiore:
            return ((v >= min) ? 1 : 0);

        case minore:
            return ((v <= max) ? 1 : 0);

        default:
            return -1;
    }
}



int checkUL(parameter_data_t p, unsigned long v)
{
    unsigned long min, max;
    min = p.d.sch.pmin != NULL ? *p.d.sch.pmin : p.d.sch.min;
    max = p.d.sch.pmax != NULL ? *p.d.sch.pmax : p.d.sch.max;

    switch (p.c)
    {
        case compreso:
            return (((v >= min) && (v <= max)) ? 1 : 0);

        case maggiore:
            return ((v >= min) ? 1 : 0);

        case minore:
            return ((v <= max) ? 1 : 0);

        default:
            return -1;
    }
}

int _parameter_level(parameter_data_t p)
{
    return p.runTime != NULL ? p.runTime(&p, 0)->lvl : p.lvl;
}

int first_parameter(parameter_data_t *p, int len, int level)
{
    int index = 0;

    while (index < len && (_parameter_level(p[index]) < 0 || _parameter_level(p[index]) > level))
        index++;

    if (index >= len)
        return -1;
    else
        return index;
}

int number_of_parameters(parameter_data_t *p, int len, int level)
{
    int num = 0;

    while (num < len && (_parameter_level(p[num]) < 0 || _parameter_level(p[num]) > level))
        num++;

    return num;
}


int next_par(parameter_data_t *p, int len, int *i, int level)
{
    int initial = *i;

    do
    {
        *i = (*i + 1) % len;
    } while (*i != initial && ((_parameter_level(p[*i]) < 0 || _parameter_level(p[*i]) > level)));

    return *i;
}

int prev_par(parameter_data_t *p, int len, int *i, int level)
{
    int initial = *i;

    do
    {
        *i = *i > 0 ? *i - 1 : len - 1;
    } while (*i != initial && ((_parameter_level(p[*i]) < 0 || _parameter_level(p[*i]) > level)));

    return *i;
}

void parameter_operator(parameter_data_t *ps, int len, int i, int mod)
{
    if (i > len || i < 0)
        return;

    if (ps[i].runTime != NULL)
    {
        ps[i].runTime(&ps[i], mod);
    }
    else
    {
        switch (ps[i].t)
        {
            case signed_char:
            {
                char v = (char)(*(ps[i].d.sch.var));

                if (checkSCH(ps[i], v) != 1)
                {
                    (*(ps[i].d.sch.var)) = ps[i].d.sch.def;
                }
            }
            break;

            case unsigned_char:
            {
                if (checkUCH(ps[i], *(ps[i].d.uch.var)) != 1)
                {
                    (*(ps[i].d.uch.var)) = ps[i].d.uch.def;
                }
            }
            break;

            case signed_int:
            {
                int v = (int)(*(ps[i].d.sint.var));

                if (checkSINT(ps[i], v) != 1)
                {
                    (*(ps[i].d.sint.var)) = ps[i].d.sint.def;
                }
            }
            break;

            case unsigned_int:
            {
                if (checkUINT(ps[i], *(ps[i].d.uint.var)) != 1)
                {
                    (*(ps[i].d.uint.var)) = ps[i].d.uint.def;
                }
            }
            break;

            case signed_long:
            {
                long v = (long)(*(ps[i].d.sl.var));

                if (checkSL(ps[i], v) != 1)
                {
                    (*(ps[i].d.sl.var)) = ps[i].d.sl.def;
                }
            }
            break;

            case unsigned_long:
            {
                if (checkUL(ps[i], *(ps[i].d.ul.var)) != 1)
                {
                    (*(ps[i].d.ul.var)) = ps[i].d.ul.def;
                }
            }
            break;
        }

        switch (ps[i].t)
        {
            case signed_char:
            {
                char min, max;
                min    = ps[i].d.sch.pmin != NULL ? *ps[i].d.sch.pmin : ps[i].d.sch.min;
                max    = ps[i].d.sch.pmax != NULL ? *ps[i].d.sch.pmax : ps[i].d.sch.max;
                char v = (char)(*(ps[i].d.sch.var)) + mod;

                if (checkSCH(ps[i], v) != 1)
                {
                    (*(ps[i].d.sch.var)) = mod > 0 ? min : max;
                }
                else
                {
                    (*(ps[i].d.sch.var)) = v;
                }
            }
            break;

            case unsigned_char:
            {
                unsigned char min, max;
                min             = ps[i].d.uch.pmin != NULL ? *ps[i].d.uch.pmin : ps[i].d.uch.min;
                max             = ps[i].d.uch.pmax != NULL ? *ps[i].d.uch.pmax : ps[i].d.uch.max;
                unsigned char v = (unsigned char)(*(ps[i].d.uch.var)) + mod;

                if (checkUCH(ps[i], v) != 1)
                {
                    (*(ps[i].d.uch.var)) = mod > 0 ? min : max;
                }
                else
                {
                    (*(ps[i].d.uch.var)) = v;
                }
            }
            break;

            case signed_int:
            {
                int min, max;
                min   = ps[i].d.sint.pmin != NULL ? *ps[i].d.sint.pmin : ps[i].d.sint.min;
                max   = ps[i].d.sint.pmax != NULL ? *ps[i].d.sint.pmax : ps[i].d.sint.max;
                int v = (int)(*(ps[i].d.sint.var)) + mod;

                if (checkSINT(ps[i], v) != 1)
                    (*(ps[i].d.sint.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.sint.var)) = v;
            }
            break;

            case unsigned_int:
            {
                unsigned int min, max;
                min            = ps[i].d.uint.pmin != NULL ? *ps[i].d.uint.pmin : ps[i].d.uint.min;
                max            = ps[i].d.uint.pmax != NULL ? *ps[i].d.uint.pmax : ps[i].d.uint.max;
                unsigned int v = (unsigned int)(*(ps[i].d.uint.var)) + mod;

                if (checkUINT(ps[i], v) != 1)
                    (*(ps[i].d.uint.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.uint.var)) = v;
            }
            break;

            case signed_long:
            {
                long min, max;
                min    = ps[i].d.sl.pmin != NULL ? *ps[i].d.sl.pmin : ps[i].d.sl.min;
                max    = ps[i].d.sl.pmax != NULL ? *ps[i].d.sl.pmax : ps[i].d.sl.max;
                long v = (long)(*(ps[i].d.sl.var)) + mod;

                if (checkSL(ps[i], v) != 1)
                    (*(ps[i].d.sl.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.sl.var)) = v;
            }
            break;

            case unsigned_long:
            {
                unsigned long min, max;
                min             = ps[i].d.ul.pmin != NULL ? *ps[i].d.ul.pmin : ps[i].d.ul.min;
                max             = ps[i].d.ul.pmax != NULL ? *ps[i].d.ul.pmax : ps[i].d.ul.max;
                unsigned long v = (*(ps[i].d.ul.var)) + mod;

                if (checkUL(ps[i], v) != 1)
                    (*(ps[i].d.ul.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.ul.var)) = v;
            }
            break;
        }
    }
}

int _int_value(parameter_data_t *p)
{
    switch (p->t)
    {
        case signed_char:
            return (int)*p->d.sch.var;
        case unsigned_char:
            return (int)*p->d.uch.var;
        case signed_int:
            return (int)*p->d.sint.var;
        case unsigned_int:
            return (int)*p->d.uint.var;
        case signed_long:
            return (int)*p->d.sl.var;
        case unsigned_long:
            return (int)*p->d.ul.var;
    }
    return 0;
}

int string_to_display(parameter_data_t *ps, int len, int i, char *string, int language)
{
    char buffer[128];

    if (i < 0 || i > len)
        return -1;

    if (ps[i].string_value != NULL)
    {
        strcpy(buffer, ps[i].string_value[_int_value(&ps[i])][language]);
    }
    else
    {
        switch (ps[i].t)
        {
            case signed_char:
                sprintf(buffer, ps[i].format[language], ps[i].d.sch.var);
                break;
            case unsigned_char:
                sprintf(buffer, ps[i].format[language], ps[i].d.uch.var);
                break;
            case signed_int:
                sprintf(buffer, ps[i].format[language], ps[i].d.sint.var);
                break;
            case unsigned_int:
                sprintf(buffer, ps[i].format[language], ps[i].d.uint.var);
                break;
            case signed_long:
                sprintf(buffer, ps[i].format[language], ps[i].d.sl.var);
                break;
            case unsigned_long:
                sprintf(buffer, ps[i].format[language], ps[i].d.ul.var);
                break;
        }
    }
    strcpy(string, buffer);
    return strlen(string);
}