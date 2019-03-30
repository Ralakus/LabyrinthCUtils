#ifdef __cplusplus
extern "C" {
#endif

#include "vector.h"

#include <stdlib.h>
#include <string.h>

bool lab_vec_init(lab_vec_t* vec, size_t type_size, size_t init_size) {
    vec->type_size  = type_size;
    vec->used_size  = 0;
    vec->alloc_size = init_size;
    if(init_size == 0) {

        vec->raw_data = NULL;

        return true;

    } else {

        vec->raw_data   = malloc(type_size * vec->alloc_size);
        if(vec->raw_data == NULL) {
            lab_errorln("Failed to allocate vector array with size of %d and type size of %d", vec->alloc_size, vec->type_size);
            return false;
        } else {
            return true;
        }

    }
}

void lab_vec_free(lab_vec_t* vec) {
    vec->used_size  = 0;
    vec->alloc_size = 0;
    free(vec->raw_data);
}

size_t lab_vec_size(lab_vec_t* vec) {
    return vec->used_size;
}

size_t lab_vec_alloc_size(lab_vec_t* vec) {
    return vec->alloc_size;
}

size_t lab_vec_type_size(lab_vec_t* vec) {
    return vec->type_size;
}

void* lab_vec_at(lab_vec_t* vec, size_t index) {
    if(index >= vec->used_size) {
        lab_errorln("Tried to access data outside of vector of size %d but access index of %d", vec->used_size, index);
        return NULL;
    } else {
        return vec->raw_data + (vec->type_size * index);
    }
}

void* lab_vec_at_raw_alloc(lab_vec_t* vec, size_t index) {
    if(index >= vec->alloc_size) {
        lab_errorln("Tried to access data outside of vector of alloc size %d but access index of %d", vec->alloc_size, index);
        return NULL;
    } else {
        return vec->raw_data + (vec->type_size * index);
    }
}

bool lab_vec_resize(lab_vec_t* vec, size_t new_size) {
    if(new_size == 0) {
        lab_vec_free(vec);
        return true;
    }
    vec->alloc_size = new_size;
    vec->raw_data = realloc(vec->raw_data, vec->alloc_size * vec->type_size);
    if(vec->raw_data == NULL) {
        lab_errorln("Failed to reallocate vector from size %d to %d with type size of %d", vec->alloc_size, vec->type_size * new_size, vec->type_size);
        return false;
    } else {
        if(vec->used_size > vec->alloc_size) {
            vec->used_size = vec->alloc_size;
        }
        return true;
    }
}

bool lab_vec_shrink_to_size(lab_vec_t* vec) {
    return lab_vec_resize(vec, vec->used_size);
}

void* lab_vec_push_back_arr(lab_vec_t* vec, const void* raw_data, size_t count) {
    vec->used_size += count;
    if(vec->used_size > vec->alloc_size) {
        if(!lab_vec_resize(vec, vec->alloc_size + (vec->used_size - vec->alloc_size))) {
            lab_errorln("Failed to push back vector!");
            return NULL;
        }
    }
    if(raw_data!=NULL){
        if(memcpy(lab_vec_at_raw_alloc(vec, vec->used_size - count), raw_data, vec->type_size * count)==NULL) {
            lab_errorln("Failed to copy data into vector!");
            return NULL;
        }
    }
    return lab_vec_at(vec, vec->used_size - count);
}

void* lab_vec_push_back(lab_vec_t* vec, const void* raw_data) {
    return lab_vec_push_back_arr(vec, raw_data, 1);
}

void lab_vec_pop_back_arr(lab_vec_t* vec, size_t count) {
    vec->used_size -= count;
}
void lab_vec_pop_back    (lab_vec_t* vec) {
    return lab_vec_pop_back_arr(vec, 1);
}

void* lab_vec_insert(lab_vec_t* vec, size_t index, const void* raw_data, size_t count) {
    vec->used_size += count;
    if(vec->used_size > vec->alloc_size) {
        if(!lab_vec_resize(vec, vec->used_size)) {
            return NULL;
        }
    }
    memmove(lab_vec_at_raw_alloc(vec, index + count), lab_vec_at_raw_alloc(vec, index), vec->used_size - index);
    if(raw_data!=NULL) {
        memcpy(vec->raw_data + index, raw_data, vec->type_size * count);
    }
    return lab_vec_at(vec, index);
}

void* lab_vec_insert_vec(lab_vec_t* dest, size_t index, lab_vec_t* src) {
    if(dest->type_size != src->type_size) {
        lab_errorln("\'lab_vec_insert_vec\' dest vector and src vector have different type sizes!");
        return NULL;
    } else {
        return lab_vec_insert(dest, index, src->raw_data, src->used_size);
    }
}


bool lab_vec_remove_arr(lab_vec_t* vec, size_t start_index, size_t count) {

    vec->used_size -= count;

    memmove(lab_vec_at(vec, start_index), lab_vec_at(vec, start_index + count), vec->used_size - start_index);

    return true;
}

bool lab_vec_remove    (lab_vec_t* vec, size_t index) {
    return lab_vec_remove_arr(vec, index, 1);
}


bool lab_vec_copy(lab_vec_t* dest, lab_vec_t* src) {

    lab_vec_free(dest);

    return lab_vec_init         (dest, src->type_size, src->alloc_size) &&
           lab_vec_push_back_arr(dest, src->raw_data, src->used_size) != NULL;

}

bool lab_vec_equal(lab_vec_t* vec0, lab_vec_t* vec1) {
    if(vec0->type_size != vec1->type_size) return false;
    if(vec0->used_size != vec1->used_size) return false;
    if(memcmp(vec0->raw_data, vec1->raw_data, vec0->type_size * vec0->used_size)==0) {
        return true;
    } else {
        return false;
    }
}

#ifdef __cplusplus
}
#endif