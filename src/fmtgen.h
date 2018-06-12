/*
    https://natecraun.net/articles/struct-iteration-through-abuse-of-the-c-preprocessor.html
*/

#ifndef STRUCT_FMT_DEF
    /* One time only definitions */
    #define STRUCT_FMT_DEF
    struct struct_fmt {
        char const *struct_name;
        size_t num_members;
        size_t struct_size;
        size_t packed_size;
        size_t *offsets;
        size_t *sizes;
        char const **type_names;
        char const **names;
        unsigned char *types;
    };
#endif

/* Error Checking */

#ifndef STRUCT_NAME
    #error "Did not define STRUCT_NAME before including fmtgen.h"
#endif

#ifndef STRUCT_FIELDS
    #error "Did not define STRUCT_FIELDS before including fmtgen.h"
#endif

#define STR_NOEXPAND(A) #A
#define STR(A) STR_NOEXPAND(A)

#define CAT_NOEXPAND(A, B) A ## B
#define CAT(A, B) CAT_NOEXPAND(A, B)

/*
struct STRUCT_NAME {
	#define X(L, R) L R;
    STRUCT_FIELDS
    #undef X
};
*/

struct struct_fmt CAT(STRUCT_NAME, _fmt) = {

    .struct_name = STR(STRUCT_NAME),

    .num_members = (
        #define X(L, R, S) 1 +
        STRUCT_FIELDS
        #undef X
    0),

    .struct_size = sizeof(struct STRUCT_NAME),

    .packed_size = (
        #define X(L, R, S) sizeof(L) +
        STRUCT_FIELDS
        #undef X
    0),

    .offsets = (size_t[]){
        #define X(L, R, S) offsetof(struct STRUCT_NAME, R),
        STRUCT_FIELDS
        #undef X
    },

    .sizes = (size_t []){
        #define X(L, R, S) sizeof(L),
        STRUCT_FIELDS
        #undef X
    },

    .type_names = (char const *[]){
        #define X(L, R, S) #L,
        STRUCT_FIELDS
        #undef X
    },

    .names = (char const *[]){
        #define X(L, R, S) #R,
        STRUCT_FIELDS
        #undef X
    },

    .types = (unsigned char []){
        #define X(L, R, S) S,
        STRUCT_FIELDS
        #undef X
    },
};

#undef STRUCT_FIELDS
#undef STRUCT_NAME
#undef STR_NOEXPAND
#undef STR
#undef CAT_NOEXPAND
#undef CAT