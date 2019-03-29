#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "logger.h"

#include <stdbool.h>
#include <stddef.h>

/*
    
    Vector struct
    Do not modify raw data unless you know what you're doing

*/
typedef struct lab_vec_t {
    
    size_t type_size;  // Size of the type that is being stored
    size_t alloc_size; // Not raw byte count, bytecount / type_size
    size_t used_size;  // ^ but used
    void* raw_data;    // Raw pointer to data

} lab_vec_t;

/*
    Initializes vector with values specified
    Program will crash if vector is not initialized before use

    type_size is the size of the type you're planning to use like `sizeof(int)`
    init_size can be left at 0 since the vector will auto resize when not large enough
*/
extern bool lab_vec_init(lab_vec_t* vec, size_t type_size, size_t init_size); 

/*
    Vectors must be freed at the end of lifetime due the use of dynamic memory mangagement
*/
extern void lab_vec_free(lab_vec_t* vec);

/*
    Self explainitory
*/
extern size_t lab_vec_size      (lab_vec_t* vec);
extern size_t lab_vec_alloc_size(lab_vec_t* vec);
extern size_t lab_vec_type_size (lab_vec_t* vec);

/*
    Returns the pointer to index in vector
    Indices are determined by type_size
*/
extern void* lab_vec_at          (lab_vec_t* vec, size_t index);

#define LAB_VEC_TYPE_AT(vec, index, type) (*(type*)lab_vec_at(vec, index))

/*
    Returns pointer to index in vector's allocated space
    Can be outside user's data
*/
extern void* lab_vec_at_raw_alloc(lab_vec_t* vec, size_t index);

#define LAB_VEC_TYPE_AT_RAW_ALLOC(vec, index, type) (*(type*)lab_vec_at_raw_alloc(vec, index))

/*
    Returns false if resize failed
*/
extern bool lab_vec_resize(lab_vec_t* vec, size_t new_size);

/*
    Shrinks vector to used_size
*/
extern bool lab_vec_shrink_to_size(lab_vec_t* vec);

/*
    Appends data to end of vector
    Returns pointer to data appended
    If NULL is passed as raw_data, no data will be copied into vector
*/
extern void* lab_vec_push_back_arr(lab_vec_t* vec, const void* raw_data, size_t count);
extern void* lab_vec_push_back    (lab_vec_t* vec, const void* raw_data);

/*
    Removes data from end of vector
*/
extern void lab_vec_pop_back_arr(lab_vec_t* vec, size_t count);
extern void lab_vec_pop_back    (lab_vec_t* vec);

/*
    Inserts data at specified index
    Returns pointer to new data
    If NULL is passed as raw_data, no data will be copied into vector
*/
extern void* lab_vec_insert    (lab_vec_t* vec,  size_t index, const void* raw_data, size_t count);
extern void* lab_vec_insert_vec(lab_vec_t* dest, size_t index, lab_vec_t* src);


/*
    Removes data from the vector
    Does not shrink vector
*/
extern bool lab_vec_remove_arr(lab_vec_t* vec, size_t start_index, size_t count);
extern bool lab_vec_remove    (lab_vec_t* vec, size_t index);

/*
    Copies a vector into another vector
    Frres vector that is being copied into
*/
extern bool lab_vec_copy(lab_vec_t* dest, lab_vec_t* src);

/*
    Compares two vectors
*/
extern bool lab_vec_equal(lab_vec_t* vec0, lab_vec_t* vec1);

#ifdef __cplusplus
}
#endif