#include <stdio.h>
#include <string.h>
#include <math.h>
#include "parameter.h"

#define COMPARE(x, min, max)                                                                                           \
    if (x < min)                                                                                                       \
        return -1;                                                                                                     \
    else if (x > max)                                                                                                  \
        return 1;                                                                                                      \
    else                                                                                                               \
        return 0;

#define CORRECTMINMAX(p, TYPE, name, func)                                                                             \
    {                                                                                                                  \
        TYPE min, max, *var;                                                                                           \
        min     = p.d.name.pmin != NULL ? (TYPE)*p.d.name.pmin : (TYPE)p.d.name.min;                                   \
        max     = p.d.name.pmax != NULL ? (TYPE)*p.d.name.pmax : (TYPE)p.d.name.max;                                   \
        var     = (TYPE *)p.d.name.var;                                                                                \
        int res = func(p, *var);                                                                                       \
        if (res > 0)                                                                                                   \
            *var = max;                                                                                                \
        else if (res < 0)                                                                                              \
            *var = min;                                                                                                \
    }

static int is_visible(parameter_data_t *p, int rlvl) {
    if (p->runtime.runtime_consider)     // If the user has specified a runtime behavior...
        return p->runtime.runtime_consider(NULL);
    else     // Otherwise, consider only the level bitmap
        return (p->lvl & rlvl) > 0;
}

static int checkSCH(parameter_data_t p, char v) {
    char min, max;
    min = p.d.sch.pmin != NULL ? *p.d.sch.pmin : p.d.sch.min;
    max = p.d.sch.pmax != NULL ? *p.d.sch.pmax : p.d.sch.max;

    COMPARE(v, min, max);
}



static int checkUCH(parameter_data_t p, unsigned char v) {
    unsigned char min, max;
    min = p.d.uch.pmin != NULL ? *p.d.uch.pmin : p.d.uch.min;
    max = p.d.uch.pmax != NULL ? *p.d.uch.pmax : p.d.uch.max;

    COMPARE(v, min, max);
}



static int checkSINT(parameter_data_t p, int v) {
    int min, max;
    min = p.d.sint.pmin != NULL ? *p.d.sint.pmin : p.d.sint.min;
    max = p.d.sint.pmax != NULL ? *p.d.sint.pmax : p.d.sint.max;

    COMPARE(v, min, max);
}



static int checkUINT(parameter_data_t p, unsigned int v) {
    unsigned int min, max;
    min = p.d.uint.pmin != NULL ? *p.d.uint.pmin : p.d.uint.min;
    max = p.d.uint.pmax != NULL ? *p.d.uint.pmax : p.d.uint.max;

    COMPARE(v, min, max);
}



static int checkSL(parameter_data_t p, long v) {
    long min, max;
    min = p.d.sl.pmin != NULL ? *p.d.sl.pmin : p.d.sl.min;
    max = p.d.sl.pmax != NULL ? *p.d.sl.pmax : p.d.sl.max;

    COMPARE(v, min, max);
}



static int checkUL(parameter_data_t p, unsigned long v) {
    unsigned long min, max;
    min = p.d.ul.pmin != NULL ? *p.d.ul.pmin : p.d.ul.min;
    max = p.d.ul.pmax != NULL ? *p.d.ul.pmax : p.d.ul.max;

    COMPARE(v, min, max);
}

static int checkFT(parameter_data_t p, float v) {
    float min, max;
    min = p.d.ft.pmin != NULL ? *p.d.ft.pmin : p.d.ft.min;
    max = p.d.ft.pmax != NULL ? *p.d.ft.pmax : p.d.ft.max;

#ifdef NAN
    if (isnan(v))
        return 1;
#endif

    COMPARE(v, min, max);
}

void init_to_default(parameter_data_t *ps, int len) {
    int i = 0;
    for (i = 0; i < len; i++) {
        switch (ps[i].t) {
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

int first_parameter(parameter_data_t *p, int len, int level) {
    int index = 0;

    while (index < len && !is_visible(&p[index], level))
        index++;

    if (index >= len)
        return -1;
    else
        return index;
}

int get_parameter_index_from_num(parameter_data_t *p, int len, int num, int level) {
    int index   = 0;
    int counter = 0;

    if (num >= number_of_parameters(p, len, level))
        return -1;

    index = first_parameter(p, len, level);

    while (counter++ < num) {
        next_parameter(p, len, &index, level);
    }

    return index;
}

int get_parameter_num_from_index(parameter_data_t *p, int len, int index, int level) {
    int num     = 0;
    int counter = 0;

    counter = first_parameter(p, len, level);

    while (counter < index) {
        next_parameter(p, len, &counter, level);
        num++;

        if (num > len)
            return -1;
    }

    return num;
}


int number_of_parameters(parameter_data_t *p, int len, int level) {
    int num = 0, i = 0;

    for (i = 0; i < len; i++) {
        if (is_visible(&p[i], level))
            num++;
    }

    return num;
}


int next_parameter(parameter_data_t *p, int len, int *i, int level) {
    int initial = *i;

    do {
        *i = (*i + 1) % len;
    } while (*i != initial && (!is_visible(&p[*i], level)));

    return *i;
}

int prev_parameter(parameter_data_t *p, int len, int *i, int level) {
    int initial = *i;

    do {
        *i = *i > 0 ? *i - 1 : len - 1;
    } while (*i != initial && (!is_visible(&p[*i], level)));

    return *i;
}

int parameter_is_inside_limits(parameter_data_t *ps, int len, int i) {
    if (i < 0 || i >= len)
        return 0;

    switch (ps[i].t) {
        case signed_char:
            return checkSCH(ps[i], *ps[i].d.sch.var);
        case unsigned_char:
            return checkUCH(ps[i], *ps[i].d.uch.var);
        case signed_int:
            return checkSINT(ps[i], *ps[i].d.sint.var);
        case unsigned_int:
            return checkUINT(ps[i], *ps[i].d.uint.var);
        case signed_long:
            return checkSL(ps[i], *ps[i].d.sl.var);
        case unsigned_long:
            return checkUL(ps[i], *ps[i].d.ul.var);
        case signed_float:
            return checkFT(ps[i], *ps[i].d.ft.var);
    }

    return 0;
}

int parameter_correct(parameter_data_t *ps, int len, int i) {
    if (i < 0 || i >= len)
        return -1;

    switch (ps[i].t) {
        case signed_char:
            CORRECTMINMAX(ps[i], char, sch, checkSCH);
            break;
        case unsigned_char:
            CORRECTMINMAX(ps[i], unsigned char, uch, checkUCH);
            break;
        case signed_int:
            CORRECTMINMAX(ps[i], int, sint, checkSINT);
            break;
        case unsigned_int:
            CORRECTMINMAX(ps[i], unsigned int, uint, checkUINT);
            break;
        case signed_long:
            CORRECTMINMAX(ps[i], signed long, sl, checkSL);
            break;
        case unsigned_long:
            CORRECTMINMAX(ps[i], unsigned long, ul, checkUL);
            break;
        case signed_float:
            CORRECTMINMAX(ps[i], float, ft, checkFT);
            break;
    }

    return 0;
}

int check_for_defaults(parameter_data_t *ps, int len) {
    int i = 0, res = 0;

    for (i = 0; i < len; i++) {
        switch (ps[i].t) {
            case signed_char:
                if (checkSCH(ps[i], (char)(*(ps[i].d.sch.var)))) {
                    (*(ps[i].d.sch.var)) = ps[i].d.sch.def;
                    res                  = 1;
                }
                break;

            case unsigned_char:
                if (checkUCH(ps[i], *(ps[i].d.uch.var))) {
                    (*(ps[i].d.uch.var)) = ps[i].d.uch.def;
                    res                  = 1;
                }
                break;

            case signed_int:
                if (checkSINT(ps[i], (int)(*(ps[i].d.sint.var)))) {
                    (*(ps[i].d.sint.var)) = ps[i].d.sint.def;
                    res                   = 1;
                }
                break;

            case unsigned_int:
                if (checkUINT(ps[i], *(ps[i].d.uint.var))) {
                    (*(ps[i].d.uint.var)) = ps[i].d.uint.def;
                    res                   = 1;
                }
                break;

            case signed_long:
                if (checkSL(ps[i], (long)(*(ps[i].d.sl.var)))) {
                    (*(ps[i].d.sl.var)) = ps[i].d.sl.def;
                    res                 = 1;
                }
                break;

            case unsigned_long:
                if (checkUL(ps[i], *(ps[i].d.ul.var))) {
                    (*(ps[i].d.ul.var)) = ps[i].d.ul.def;
                    res                 = 1;
                }
                break;

            case signed_float:
                if (checkFT(ps[i], *(ps[i].d.ft.var))) {
                    (*(ps[i].d.ft.var)) = ps[i].d.ft.def;
                    res                 = 1;
                }
                break;
        }
    }

    return res;
}

int parameter_get_userdata(parameter_data_t *ps, int len, int i, parameter_user_data_t *data) {
    if (i > len || i < 0)
        return -1;
    *data = ps[i].runtime.userdata;
    return 0;
}

parameter_variable_type_t parameter_type(parameter_data_t *ps, int len, int i) {
    if (i > len || i < 0)
        return -1;

    return ps[i].t;
}

void parameter_operator(parameter_data_t *ps, int len, int i, int mod) {
    float multiplier = ps[i].multiplier > 0 ? ps[i].multiplier : 1;

    if (i > len || i < 0)
        return;

    if (ps[i].runtime.runtime_operator != NULL) {
        ps[i].runtime.runtime_operator(&ps[i], mod);
    } else {
        switch (ps[i].t) {
            case signed_char: {
                char v = (char)(*(ps[i].d.sch.var));

                if (checkSCH(ps[i], v))
                    v = ps[i].d.sch.def;

                char min, max;
                min = ps[i].d.sch.pmin != NULL ? *ps[i].d.sch.pmin : ps[i].d.sch.min;
                max = ps[i].d.sch.pmax != NULL ? *ps[i].d.sch.pmax : ps[i].d.sch.max;
                v += mod * multiplier;

                if (checkSCH(ps[i], v))
                    (*(ps[i].d.sch.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.sch.var)) = v;

                break;
            }

            case unsigned_char: {
                unsigned char v = (unsigned char)(*(ps[i].d.uch.var));
                if (checkUCH(ps[i], v))
                    v = ps[i].d.uch.def;

                unsigned char min, max;
                min = ps[i].d.uch.pmin != NULL ? *ps[i].d.uch.pmin : ps[i].d.uch.min;
                max = ps[i].d.uch.pmax != NULL ? *ps[i].d.uch.pmax : ps[i].d.uch.max;
                v += mod * multiplier;

                if (checkUCH(ps[i], v))
                    (*(ps[i].d.uch.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.uch.var)) = v;

                break;
            }

            case signed_int: {
                int v = (int)(*(ps[i].d.sint.var));

                if (checkSINT(ps[i], v))
                    v = ps[i].d.sint.def;

                int min, max;
                min = ps[i].d.sint.pmin != NULL ? *ps[i].d.sint.pmin : ps[i].d.sint.min;
                max = ps[i].d.sint.pmax != NULL ? *ps[i].d.sint.pmax : ps[i].d.sint.max;
                v += mod * multiplier;

                if (checkSINT(ps[i], v))
                    (*(ps[i].d.sint.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.sint.var)) = v;
                break;
            }

            case unsigned_int: {
                unsigned int v = (unsigned int)(*(ps[i].d.uint.var));
                if (checkUINT(ps[i], v))
                    v = ps[i].d.uint.def;

                unsigned int min, max;
                min = ps[i].d.uint.pmin != NULL ? *ps[i].d.uint.pmin : ps[i].d.uint.min;
                max = ps[i].d.uint.pmax != NULL ? *ps[i].d.uint.pmax : ps[i].d.uint.max;
                v += mod * multiplier;

                if (checkUINT(ps[i], v))
                    (*(ps[i].d.uint.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.uint.var)) = v;
                break;
            }

            case signed_long: {
                long v = (long)(*(ps[i].d.sl.var));

                if (checkSL(ps[i], v))
                    v = ps[i].d.sl.def;

                long min, max;
                min = ps[i].d.sl.pmin != NULL ? *ps[i].d.sl.pmin : ps[i].d.sl.min;
                max = ps[i].d.sl.pmax != NULL ? *ps[i].d.sl.pmax : ps[i].d.sl.max;
                v += mod * multiplier;

                if (checkSL(ps[i], v))
                    (*(ps[i].d.sl.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.sl.var)) = v;
                break;
            }

            case unsigned_long: {
                unsigned long v = (*(ps[i].d.ul.var));
                if (checkUL(ps[i], v))
                    v = ps[i].d.ul.def;

                unsigned long min, max;
                min = ps[i].d.ul.pmin != NULL ? *ps[i].d.ul.pmin : ps[i].d.ul.min;
                max = ps[i].d.ul.pmax != NULL ? *ps[i].d.ul.pmax : ps[i].d.ul.max;
                v += mod * multiplier;

                if (checkUL(ps[i], v))
                    (*(ps[i].d.ul.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.ul.var)) = v;
                break;
            }

            case signed_float: {
                float v = (*(ps[i].d.ft.var));
                if (checkFT(ps[i], v))
                    v = ps[i].d.ft.def;

                float min, max;
                min = ps[i].d.ft.pmin != NULL ? *ps[i].d.ft.pmin : ps[i].d.ft.min;
                max = ps[i].d.ft.pmax != NULL ? *ps[i].d.ft.pmax : ps[i].d.ft.max;
                v += mod * multiplier;

                if (checkFT(ps[i], v))
                    (*(ps[i].d.ft.var)) = mod > 0 ? min : max;
                else
                    (*(ps[i].d.ft.var)) = v;
            } break;
        }
    }
}

static int _int_value(parameter_data_t *p) {
    switch (p->t) {
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

unsigned long parameter_get_num_values(parameter_data_t *ps, int len, int i) {
    unsigned long res;
    if (i >= len)
        return -1;

    switch (ps[i].t) {
        case signed_char: {
            char min, max;
            min = ps[i].d.sch.pmin != NULL ? *ps[i].d.sch.pmin : ps[i].d.sch.min;
            max = ps[i].d.sch.pmax != NULL ? *ps[i].d.sch.pmax : ps[i].d.sch.max;
            res = max - min;
            break;
        }
        case unsigned_char: {
            unsigned char min, max;
            min = ps[i].d.uch.pmin != NULL ? *ps[i].d.uch.pmin : ps[i].d.uch.min;
            max = ps[i].d.uch.pmax != NULL ? *ps[i].d.uch.pmax : ps[i].d.uch.max;
            res = max - min + 1;
            break;
        }

        case signed_int: {
            int min, max;
            min = ps[i].d.sint.pmin != NULL ? *ps[i].d.sint.pmin : ps[i].d.sint.min;
            max = ps[i].d.sint.pmax != NULL ? *ps[i].d.sint.pmax : ps[i].d.sint.max;
            res = max - min + 1;
            break;
        }

        case unsigned_int: {
            unsigned int min, max;
            min = ps[i].d.uint.pmin != NULL ? *ps[i].d.uint.pmin : ps[i].d.uint.min;
            max = ps[i].d.uint.pmax != NULL ? *ps[i].d.uint.pmax : ps[i].d.uint.max;
            res = max - min + 1;
            break;
        }

        case signed_long: {
            long min, max;
            min = ps[i].d.sl.pmin != NULL ? *ps[i].d.sl.pmin : ps[i].d.sl.min;
            max = ps[i].d.sl.pmax != NULL ? *ps[i].d.sl.pmax : ps[i].d.sl.max;
            res = max - min + 1;
            break;
        }

        case unsigned_long: {
            unsigned long min, max;
            min = ps[i].d.ul.pmin != NULL ? *ps[i].d.ul.pmin : ps[i].d.ul.min;
            max = ps[i].d.ul.pmax != NULL ? *ps[i].d.ul.pmax : ps[i].d.ul.max;
            res = max - min + 1;
            break;
        }

        case signed_float:
            res = 0;
            break;
    }

    return res;
}

int parameter_get_description(parameter_data_t *ps, int len, int i, char *string, int language) {
    if (i >= len)
        return -1;

    if (ps[i].display.description) {
        strcpy(string, ps[i].display.description[language]);
        return strlen(string);
    } else {
        return 0;
    }
}

int parameter_get_string_value(parameter_data_t *ps, int len, int i, char *string, unsigned int value, int language) {
    if (i >= len)
        return -1;

    if (ps[i].display.string_value != NULL) {
        char *(*pt)[LANGUAGES] = (char *(*)[LANGUAGES])ps[i].display.string_value;
        strcpy(string, pt[value][language]);
    }

    return 0;
}

int string_to_display(parameter_data_t *ps, int len, int i, char *string, int language) {
    if (i < 0 || i >= len)
        return -1;

    char *(*pt)[LANGUAGES] = (char *(*)[LANGUAGES])ps[i].display.string_value;

    if (ps[i].display.special_format != NULL) {
        return ps[i].display.special_format(&ps[i], string, language);
    } else if (ps[i].display.string_value != NULL) {
        if (ps[i].display.format)
            sprintf(string, ps[i].display.format[language], pt[_int_value(&ps[i])][language]);
        else
            strcpy(string, pt[_int_value(&ps[i])][language]);
    } else {
        switch (ps[i].t) {
            case signed_char:
                if (ps[i].display.format)
                    sprintf(string, ps[i].display.format[language], (char)*ps[i].d.sch.var);
                else
                    sprintf(string, "%i", (char)*ps[i].d.sch.var);
                break;
            case unsigned_char:
                if (ps[i].display.format)
                    sprintf(string, ps[i].display.format[language], (unsigned char)*ps[i].d.uch.var);
                else
                    sprintf(string, "%i", (unsigned char)*ps[i].d.uch.var);
                break;
            case signed_int:
                if (ps[i].display.format)
                    sprintf(string, ps[i].display.format[language], (int)*ps[i].d.sint.var);
                else
                    sprintf(string, "%i", (char)*ps[i].d.sint.var);
                break;
            case unsigned_int:
                if (ps[i].display.format)
                    sprintf(string, ps[i].display.format[language], (unsigned int)*ps[i].d.uint.var);
                else
                    sprintf(string, "%u", (unsigned int)*ps[i].d.uint.var);
                break;
            case signed_long:
                if (ps[i].display.format)
                    sprintf(string, ps[i].display.format[language], (long)*ps[i].d.sl.var);
                else
                    sprintf(string, "%ld", (long)*ps[i].d.sl.var);
                break;
            case unsigned_long:
                if (ps[i].display.format)
                    sprintf(string, ps[i].display.format[language], (unsigned long)*ps[i].d.ul.var);
                else
                    sprintf(string, "%lu", (unsigned long)*ps[i].d.ul.var);
                break;
            case signed_float:
                if (ps[i].display.format)
                    sprintf(string, ps[i].display.format[language], (float)*ps[i].d.ft.var);
                else
                    sprintf(string, "%.f", (float)*ps[i].d.ft.var);
                break;
        }
    }
    return strlen(string);
}

int parameter_set_from_data(parameter_data_t *src, parameter_data_t *ps, int len, int index) {
    if (index < 0 || index >= len)
        return -1;

    parameter_data_t *dest = &ps[index];

    switch (src->t) {
        case signed_char:
            *dest->d.sch.var = *src->d.sch.var;
            break;
        case unsigned_char:
            *dest->d.uch.var = *src->d.uch.var;
            break;
        case signed_int:
            *dest->d.sint.var = *src->d.sint.var;
            break;
        case unsigned_int:
            *dest->d.uint.var = *src->d.uint.var;
            break;
        case signed_long:
            *dest->d.sl.var = *src->d.sl.var;
            break;
        case unsigned_long:
            *dest->d.ul.var = *src->d.ul.var;
            break;
        case signed_float:
            *dest->d.ft.var = *src->d.ft.var;
            break;
    }

    return 0;
}

int parameter_clone_data(parameter_data_t *dest, parameter_data_t *ps, int len, int index, void *subst) {
    if (index < 0 || index >= len)
        return -1;

    *dest = ps[index];

    switch (dest->t) {
        case signed_char:
            *((signed char *)subst) = *(dest->d.sch.var);
            dest->d.sch.var         = subst;
            break;
        case unsigned_char:
            *((unsigned char *)subst) = *(dest->d.uch.var);
            dest->d.uch.var           = subst;
            break;
        case signed_int:
            *((signed int *)subst) = *(dest->d.sint.var);
            dest->d.sint.var       = subst;
            break;
        case unsigned_int:
            *((unsigned int *)subst) = *(dest->d.uint.var);
            dest->d.uint.var         = subst;
            break;
        case signed_long:
            *((signed long *)subst) = *(dest->d.sl.var);
            dest->d.sl.var          = subst;
            break;
        case unsigned_long:
            *((unsigned long *)subst) = *(dest->d.ul.var);
            dest->d.ul.var            = subst;
            break;
        case signed_float:
            *((float *)subst) = *(dest->d.ft.var);
            dest->d.ft.var    = subst;
            break;
    }
    return 0;
}

int parameter_set_uint_value(parameter_data_t *ps, int len, int index, unsigned int value) {
    if (index < 0 || index >= len)
        return -1;

    *ps[index].d.uint.var = value;
    return 0;
}