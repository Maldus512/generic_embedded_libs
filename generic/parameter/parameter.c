#include <stdio.h>
#include <string.h>
#include "parameter.h"

static int check_level(int plvl, int rlvl)
{
    return (plvl & rlvl) > 0;
}

static int checkSCH(parameter_data_t p, char v)
{
    char min, max;
    min = p.d.sch.pmin != NULL ? *p.d.sch.pmin : p.d.sch.min;
    max = p.d.sch.pmax != NULL ? *p.d.sch.pmax : p.d.sch.max;

    return (((v >= min) && (v <= max)) ? 1 : 0);
}



static int checkUCH(parameter_data_t p, unsigned char v)
{
    unsigned char min, max;
    min = p.d.uch.pmin != NULL ? *p.d.uch.pmin : p.d.uch.min;
    max = p.d.uch.pmax != NULL ? *p.d.uch.pmax : p.d.uch.max;

    return (((v >= min) && (v <= max)) ? 1 : 0);
}



static int checkSINT(parameter_data_t p, int v)
{
    int min, max;
    min = p.d.sint.pmin != NULL ? *p.d.sint.pmin : p.d.sint.min;
    max = p.d.sint.pmax != NULL ? *p.d.sint.pmax : p.d.sint.max;

    return (((v >= min) && (v <= max)) ? 1 : 0);
}



static int checkUINT(parameter_data_t p, unsigned int v)
{
    unsigned int min, max;
    min = p.d.uint.pmin != NULL ? *p.d.uint.pmin : p.d.uint.min;
    max = p.d.uint.pmax != NULL ? *p.d.uint.pmax : p.d.uint.max;

    return (((v >= min) && (v <= max)) ? 1 : 0);
}



static int checkSL(parameter_data_t p, long v)
{
    long min, max;
    min = p.d.sl.pmin != NULL ? *p.d.sl.pmin : p.d.sl.min;
    max = p.d.sl.pmax != NULL ? *p.d.sl.pmax : p.d.sl.max;

    return (((v >= min) && (v <= max)) ? 1 : 0);
}



static int checkUL(parameter_data_t p, unsigned long v)
{
    unsigned long min, max;
    min = p.d.ul.pmin != NULL ? *p.d.ul.pmin : p.d.ul.min;
    max = p.d.ul.pmax != NULL ? *p.d.ul.pmax : p.d.ul.max;

    return (((v >= min) && (v <= max)) ? 1 : 0);
}

static int checkFT(parameter_data_t p, float v)
{
    float min, max;
    min = p.d.ft.pmin != NULL ? *p.d.ft.pmin : p.d.ft.min;
    max = p.d.ft.pmax != NULL ? *p.d.ft.pmax : p.d.ft.max;

    return (((v >= min) && (v <= max)) ? 1 : 0);
}

static int _parameter_level(parameter_data_t p)
{
    return p.runtime_operator != NULL ? p.runtime_operator(&p, 0)->lvl : p.lvl;
}

void init_to_default(parameter_data_t *ps, int len) {
    int i = 0;
    for (i = 0; i < len; i++) {
        switch (ps[i].t)
        {
            case signed_char:
                (*(ps[i].d.sch.var)) = ps[i].d.sch.def;
                break;

            case unsigned_char:
                (*(ps[i].d.uch.var)) = ps[i].d.uch.def;
                break;

            case signed_int:
                (*(ps[i].d.sint.var)) = ps[i].d.sint.def;
                break;

            case unsigned_int:
                (*(ps[i].d.uint.var)) = ps[i].d.uint.def;
                break;

            case signed_long:
                (*(ps[i].d.sl.var)) = ps[i].d.sl.def;
                break;

            case unsigned_long:
                (*(ps[i].d.ul.var)) = ps[i].d.ul.def;
                break;

            case signed_float:
                (*(ps[i].d.ft.var)) = ps[i].d.ft.def;
                break;
        }
    }
}

int first_parameter(parameter_data_t *p, int len, int level)
{
    int index = 0;

    while (index < len && (_parameter_level(p[index]) < 0 || !check_level(_parameter_level(p[index]), level)))
        index++;

    if (index >= len)
        return -1;
    else
        return index;
}

int get_parameter_index_from_num(parameter_data_t *p, int len, int num, int level)
{
    int index   = 0;
    int counter = 0;

    if (num >= number_of_parameters(p, len, level))
        return -1;

    index = first_parameter(p, len, level);

    while (counter++ < num)
    {
        next_parameter(p, len, &index, level);
    }

    return index;
}

int get_parameter_num_from_index(parameter_data_t *p, int len, int index, int level)
{
    int num     = 0;
    int counter = 0;

    counter = first_parameter(p, len, level);

    while (counter < index)
    {
        next_parameter(p, len, &counter, level);
        num++;

        if (num > len)
            return -1;
    }

    return num;
}


int number_of_parameters(parameter_data_t *p, int len, int level)
{
    int num = 0, i = 0;

    for (i = 0; i < len; i++)
    {
        if (_parameter_level(p[i]) < 0 || check_level(_parameter_level(p[i]), level))
            num++;
    }

    return num;
}


int next_parameter(parameter_data_t *p, int len, int *i, int level) // TODO: a cosa serve il puntatore?
{
    int initial = *i;

    do
    {
        *i = (*i + 1) % len;
    } while (*i != initial && ((_parameter_level(p[*i]) < 0 || !check_level(_parameter_level(p[*i]), level))));

    return *i;
}

int prev_parameter(parameter_data_t *p, int len, int *i, int level)
{
    int initial = *i;

    do
    {
        *i = *i > 0 ? *i - 1 : len - 1;
    } while (*i != initial && ((_parameter_level(p[*i]) < 0 || !check_level(_parameter_level(p[*i]), level))));

    return *i;
}

int check_for_defaults(parameter_data_t *ps, int len) {
    int i = 0, res = 0;
    
    for (i = 0; i < len; i++)
    {
        switch (ps[i].t)
        {
            case signed_char:
                if (checkSCH(ps[i], (char)(*(ps[i].d.sch.var))) != 1) {
                    (*(ps[i].d.sch.var)) = ps[i].d.sch.def;
                    res = 1;
                }
                break;

            case unsigned_char:
                if (checkUCH(ps[i], *(ps[i].d.uch.var)) != 1) {
                    (*(ps[i].d.uch.var)) = ps[i].d.uch.def;
                    res = 1;
                }
                break;

            case signed_int:
                if (checkSINT(ps[i], (int)(*(ps[i].d.sint.var))) != 1) {
                    (*(ps[i].d.sint.var)) = ps[i].d.sint.def;
                    res = 1;
                }
            break;

            case unsigned_int:
                if (checkUINT(ps[i], *(ps[i].d.uint.var)) != 1) {
                    (*(ps[i].d.uint.var)) = ps[i].d.uint.def;
                    res = 1;
                }
            break;

            case signed_long:
                if (checkSL(ps[i], (long)(*(ps[i].d.sl.var))) != 1) {
                    (*(ps[i].d.sl.var)) = ps[i].d.sl.def;
                    res = 1;
                }
            break;

            case unsigned_long:
                if (checkUL(ps[i], *(ps[i].d.ul.var)) != 1) {
                    (*(ps[i].d.ul.var)) = ps[i].d.ul.def;
                    res = 1;
                }
            break;

            case signed_float:
                if (checkFT(ps[i], *(ps[i].d.ft.var)) != 1) {
                    (*(ps[i].d.ft.var)) = ps[i].d.ft.def;
                    res = 1;
                }
            break;
        }
    }

    return res;
}

void parameter_operator(parameter_data_t *ps, int len, int i, int mod)
{
    float ftmod;

    if (i > len || i < 0)
        return;

    if (ps[i].runtime_operator != NULL)
    {
        ps[i].runtime_operator(&ps[i], mod);
    }
    else
    {
        if (ps[i].multiplier > 1)
            mod *= ps[i].multiplier;

        switch (ps[i].t)
        {
            case signed_char:
            {
                char v = (char)(*(ps[i].d.sch.var));

                if (checkSCH(ps[i], v) != 1)
                    (*(ps[i].d.sch.var)) = ps[i].d.sch.def;
                break;
            }

            case unsigned_char:
            {
                if (checkUCH(ps[i], *(ps[i].d.uch.var)) != 1)
                    (*(ps[i].d.uch.var)) = ps[i].d.uch.def;
                break;
            }

            case signed_int:
            {
                int v = (int)(*(ps[i].d.sint.var));

                if (checkSINT(ps[i], v) != 1)
                    (*(ps[i].d.sint.var)) = ps[i].d.sint.def;
                break;
            }

            case unsigned_int:
            {
                if (checkUINT(ps[i], *(ps[i].d.uint.var)) != 1)
                    (*(ps[i].d.uint.var)) = ps[i].d.uint.def;
                break;
            }

            case signed_long:
            {
                long v = (long)(*(ps[i].d.sl.var));

                if (checkSL(ps[i], v) != 1)
                    (*(ps[i].d.sl.var)) = ps[i].d.sl.def;
                break;
            }

            case unsigned_long:
            {
                if (checkUL(ps[i], *(ps[i].d.ul.var)) != 1)
                    (*(ps[i].d.ul.var)) = ps[i].d.ul.def;
                break;
            }

            case signed_float:
            {
                if (checkFT(ps[i], *(ps[i].d.ft.var)) != 1)
                    (*(ps[i].d.ft.var)) = ps[i].d.ft.def;

                ftmod = (float)mod;
                if (ps[i].multiplier > 1)
                    mod *= ps[i].multiplier;
                break;
            }
        }
        //TODO: this switch is identical to the one before. Merge them
        switch (ps[i].t)
        {
            case signed_char:
            {
                char min, max;
                min    = ps[i].d.sch.pmin != NULL ? *ps[i].d.sch.pmin : ps[i].d.sch.min;
                max    = ps[i].d.sch.pmax != NULL ? *ps[i].d.sch.pmax : ps[i].d.sch.max;
                char v = (char)(*(ps[i].d.sch.var)) + mod;

                if (checkSCH(ps[i], v) != 1)
                    (*(ps[i].d.sch.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.sch.var)) = v;
            }
            break;

            case unsigned_char:
            {
                unsigned char min, max;
                min             = ps[i].d.uch.pmin != NULL ? *ps[i].d.uch.pmin : ps[i].d.uch.min;
                max             = ps[i].d.uch.pmax != NULL ? *ps[i].d.uch.pmax : ps[i].d.uch.max;
                unsigned char v = (unsigned char)(*(ps[i].d.uch.var)) + mod;

                if (checkUCH(ps[i], v) != 1)
                    (*(ps[i].d.uch.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.uch.var)) = v;
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

            case signed_float:
            {
                float min, max;
                min      = ps[i].d.ft.pmin != NULL ? *ps[i].d.ft.pmin : ps[i].d.ft.min;
                max      = ps[i].d.ft.pmax != NULL ? *ps[i].d.ft.pmax : ps[i].d.ft.max;
                float v = (*(ps[i].d.ft.var)) + ftmod;

                if (checkUL(ps[i], v) != 1)
                    (*(ps[i].d.ft.var)) = ftmod > 0 ? min : max;
                else
                    (*(ps[i].d.ft.var)) = v;
            }
            break;
        }
    }
}

static int _int_value(parameter_data_t *p)
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
        case signed_float:
            return (int)(*p->d.ft.var);
    }
    return 0;
}

int string_to_display(parameter_data_t *ps, int len, int i, char *string, int language)
{
    char buffer[128];

    if (i < 0 || i > len)
        return -1;

    char *(*pt)[LANGUAGES] = (char *(*)[LANGUAGES])ps[i].string_value;

    if (ps[i].string_value != NULL)
    {
        sprintf(buffer, ps[i].format[language], pt[_int_value(&ps[i])][language]);
    }
    else
    {
        switch (ps[i].t)
        {
            case signed_char:
                sprintf(buffer, ps[i].format[language], (char)*ps[i].d.sch.var);
                break;
            case unsigned_char:
                sprintf(buffer, ps[i].format[language], (unsigned char)*ps[i].d.uch.var);
                break;
            case signed_int:
                sprintf(buffer, ps[i].format[language], (int)*ps[i].d.sint.var);
                break;
            case unsigned_int:
                sprintf(buffer, ps[i].format[language], (unsigned int)*ps[i].d.uint.var);
                break;
            case signed_long:
                sprintf(buffer, ps[i].format[language], (long)*ps[i].d.sl.var);
                break;
            case unsigned_long:
                sprintf(buffer, ps[i].format[language], (unsigned long)*ps[i].d.ul.var);
                break;
            case signed_float:
                sprintf(buffer, ps[i].format[language], (float)*ps[i].d.ft.var);
                break;
        }
    }
    strcpy(string, buffer);
    return strlen(string);
}