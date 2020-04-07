#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#ifndef LANGUAGES
#define LANGUAGES 1
#endif

#ifndef PARAMETER_USER_DATA
typedef void *parameter_user_data_t;
#else
typedef PARAMETER_USER_DATA parameter_user_data_t;
#endif

#define par_op(par, op)                     parameter_operator(par, 1, 0, op)
#define par_get_desc(par, str, lang)        parameter_get_description(par, 1, 0, str, lang)
#define par_get_udata(par, udata)           parameter_get_userdata(par, 1, 0, udata)
#define par_get_num_values(par)             parameter_get_num_values(par, 1, 0)
#define par_get_string(par, str, val, lang) parameter_get_string_value(par, 1, 0, str, val, lang)
#define par_set_uint(par, val)              parameter_set_uint_value(par, 1, 0, val)
#define par_get_uint(par)                   (*((par)->d.uint.var))
#define par_set_default(par)                init_to_default(par, 1)
#define par_limits(par)                     parameter_is_inside_limits(par, 1, 0)
#define par_correct(par)                    parameter_correct(par, 1, 0);

typedef enum {
    unsigned_long = 0,
    signed_long   = 1,
    unsigned_char = 2,
    signed_char   = 3,
    unsigned_int  = 4,
    signed_int    = 5,
    signed_float  = 6,
} parameter_variable_type_t;

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
    parameter_variable_type_t t;     // Variable type
    _data_type_t              d;     // Data union, depends on the variable type

    // Strings to display. They should be char arrays
    struct {
        char ** description;      // Description; array on languages
        char ** format;           // Format; array on languages
        char ***string_value;     // Values; array of values on array of languages.
        // If string_value is not NULL the parameter's value is used to index this string array for the
        // value to display
        int (*special_format)(struct _parameter_data_t *, char *, int);
    } display;

    int lvl;

    struct {
        void (*runtime_operator)(struct _parameter_data_t *, int);
        int (*runtime_consider)(void *);
        parameter_user_data_t userdata;
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

int           parameter_correct(parameter_data_t *ps, int len, int i);
int           parameter_is_inside_limits(parameter_data_t *ps, int len, int i);
int           parameter_set_from_data(parameter_data_t *src, parameter_data_t *ps, int len, int index);
int           parameter_get_description(parameter_data_t *ps, int len, int i, char *string, int language);
unsigned long parameter_get_num_values(parameter_data_t *ps, int len, int i);
int           parameter_get_userdata(parameter_data_t *ps, int len, int i, parameter_user_data_t *data);
int parameter_get_string_value(parameter_data_t *ps, int len, int i, char *string, unsigned int value, int language);
parameter_variable_type_t parameter_type(parameter_data_t *ps, int len, int i);

int parameter_set_uint_value(parameter_data_t *ps, int len, int i, unsigned int value);
int parameter_clone_data(parameter_data_t *dest, parameter_data_t *ps, int len, int index, void *subst);

#endif