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
    tipovariabile t;     // Variable type
    _data_type_t  d;     // Data union, depends on the variable type

    // Strings to display. They should be char arrays
    struct {
        char ** description;      // Description; array on languages
        char ** format;           // Format; array on languages
        char ***string_value;     // Values; array of values on array of languages.
        // If string_value is not NULL the parameter's value is used to index this string array for the
        // value to display
        int (*special_format)(struct _parameter_data_t *, char *);
    } display;

    int lvl;

    struct {
        void (*runtime_operator)(struct _parameter_data_t *, int);
        int (*runtime_consider)(void *);
        void *userdata;
    } runtime;

    float multiplier;
} parameter_data_t;

/*
 *  Copies the value to display of the parameter (following the format and string_value fields)
 * int string. The result depends on language
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  i : index of the parameter
 *  language: language to use
 *  return: lenght of the copied string
 */
int string_to_display(parameter_data_t *ps, int len, int i, char *string, int language);

unsigned long get_num_values(parameter_data_t *ps, int len, int i);

int get_string_value(parameter_data_t *ps, int len, int i, char *string, unsigned int value, int language);

/*
 *  Moves the parameter index forward, stepping through any parameter that is not included in the
 * specified access level.
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  i : pointer to the index of the parameter. The function modifies it.
 *  level: access level bitmap
 *  return: new index (also modified by the pointer)
 */
int next_parameter(parameter_data_t *ps, int len, int *i, int level);

/*
 *  Moves the parameter index backwards, stepping through any parameter that is not included in the
 * specified access level.
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  i : pointer to the index of the parameter. The function modifies it.
 *  level: access level bitmap
 *  return: new index (also modified by the pointer)
 */
int prev_parameter(parameter_data_t *ps, int len, int *i, int level);

/*
 *  Returns the counted number of parameters included in the specified access level.
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  level: access level bitmap
 *  return: number of parameters
 */
int number_of_parameters(parameter_data_t *ps, int len, int level);

/*
 *  Finds the index of the first parameter included in the specified access level.
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  level: access level bitmap
 *  return: index of the first parameter
 */
int first_parameter(parameter_data_t *ps, int len, int level);

/*
 *  Modifies the parameter according to its restrictions. mod should be a positive
 * or negative integer depending on the direction of the change. mod refers to the
 * multiplier field, that indicates the unit of the parameter.
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  i : index of the parameter. The function modifies it.
 *  mod : modifier to apply to the parameter.
 */
void parameter_operator(parameter_data_t *ps, int len, int i, int mod);

/*
 *  Returns the index of the nth parameter (according to the access level)
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  num: number of the parameter
 *  level: access level bitmap
 *  return: index of the nth parameter
 */
int get_parameter_index_from_num(parameter_data_t *p, int len, int num, int level);

/*
 *  Returns the number of the indexed parameter in the list of parameters included in
 * the specified access level.
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  index: index of the parameter
 *  level: access level bitmap
 *  return: number of the indexed parameter
 */
int get_parameter_num_from_index(parameter_data_t *p, int len, int index, int level);

/*
 *  Initializes all parameters to their default value.
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 */
void init_to_default(parameter_data_t *ps, int len);
/*
 *  Initializes any parameter that is out of the specified range to its
 * default value. Returns 1 if any such parameter is found.
 *  ps: array of parameter_data_t structs.
 *  len: lenght of ps
 *  return: 1 or 0
 */
int check_for_defaults(parameter_data_t *ps, int len);

int get_description(parameter_data_t *ps, int len, int i, char *string, int language);

#endif