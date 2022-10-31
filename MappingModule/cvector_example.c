/*
 * Copyright (c) 2015 Evan Teran
 *
 * License: The MIT License (MIT)
 */

// credit goes to https://github.com/eteran/c-vector

/* if this is defined, then the vector will double in capacity each
 * time it runs out of space. if it is not defined, then the vector will
 * be conservative, and will have a capcity no larger than necessary.
 * having this defined will minimize how often realloc gets called.
 */
#define CVECTOR_LOGARITHMIC_GROWTH

#include <stdio.h>
#include <stdbool.h>

#include "cvector.h"

int main(void)
{

    /* this is the variable that will store the array, you can have
     * a vector of any type! For example, you may write float *v = NULL,
     * and you'd have a vector of floats :-). NULL will have a size
     * and capacity of 0. Additionally, vector_begin and vector_end will
     * return NULL on a NULL vector. Alternatively, for clarity of writing
     * you can use the cvector_vector_type macro to define a vector of a
     * given type.
     */
    cvector_vector_type(bool) v = NULL;

    /* add some elements to the back */
    cvector_push_back(v, 1);
    cvector_push_back(v, 0);
    cvector_push_back(v, 0);
    cvector_push_back(v, 1);

    /* remove an element by specifying an array subscript */
    cvector_erase(v, 2);

    /* remove an element from the back */
    cvector_pop_back(v);

    /* print out some stats about the vector */
    printf("pointer : %p\n", v);
    printf("capacity: %zu\n", cvector_capacity(v));
    printf("size    : %zu\n", cvector_size(v));

    /* iterator over the vector using "iterator" style */
    if (v)
    {
        bool *it;
        int i = 0;
        for (it = cvector_begin(v); it != cvector_end(v); ++it)
        {
            printf("v[%d] = %d\n", i, *it);
            ++i;
        }
    }

    /* iterator over the vector standard indexing too! */
    if (v)
    {
        size_t i;
        for (i = 0; i < cvector_size(v); ++i)
        {
            printf("v[%zu] = %d\n", i, v[i]);
        }
    }

    /* well, we don't have destructors, so let's clean things up */
    cvector_free(v);

    return 0;
}
