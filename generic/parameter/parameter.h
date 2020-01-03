#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#ifndef LANGUAGES
#define LANGUAGES 1
#endif

typedef enum {
    unsigned_long = 0,
    signed_long   = 1,
    unsigned_char = 2,
    signed_char   = 3,
    unsigned_int  = 4,
    signed_int    = 5,
    signed_float  = 6,
} tipovariabile;



typedef struct {
    char  min, max, def;
    char *var;
    char *pmin, *pmax;
} _sch;

typedef struct {
    unsigned char  min, max, def;
    unsigned char *var;
    unsigned char *pmin, *pmax;
} _uch;

typedef struct {
    int  min, max, def;
    int *var;
    int *pmin, *pmax;
} _sint;

typedef struct {
    unsigned int  min, max, def;
    unsigned int *var;
    unsigned int *pmin, *pmax;
} _uint;

typedef struct {
    long  min, max, def;
    long *var;
    long *pmin, *pmax;
} _sl;

typedef struct {
    unsigned long  min, max, def;
    unsigned long *var;
    unsigned long *pmin, *pmax;
} _ul;

typedef struct {
    float  min, max, def;
    float *var;
    float *pmin, *pmax;
} _ft;



typedef union {
    _sch  sch;
    _uch  uch;
    _sint sint;
    _uint uint;
    _sl   sl;
    _ul   ul;
    _ft   ft;
} _data_type_t;

typedef struct _parameter_data_t {
    tipovariabile t;
    _data_type_t  d;

    char **      format;
    char ***     string_value;
    unsigned int lvl;
    unsigned int multiplier;     // TODO: deve diventare un'unita' (per i float)
    void (*runtime_operator)(struct _parameter_data_t *, int);
    int (*runtime_consider)(struct _parameter_data_t *);
} parameter_data_t;

int  string_to_display(parameter_data_t *ps, int len, int i, char *string, int language);
int  next_parameter(parameter_data_t *ps, int len, int *i, int level);
int  prev_parameter(parameter_data_t *ps, int len, int *i, int level);
int  number_of_parameters(parameter_data_t *ps, int len, int level);
int  first_parameter(parameter_data_t *ps, int len, int level);
void parameter_operator(parameter_data_t *ps, int len, int i, int mod);
int  get_parameter_index_from_num(parameter_data_t *p, int len, int num, int level);
int  get_parameter_num_from_index(parameter_data_t *p, int len, int index, int level);
void init_to_default(parameter_data_t *ps, int len);
int  check_for_defaults(parameter_data_t *ps, int len);

#endif