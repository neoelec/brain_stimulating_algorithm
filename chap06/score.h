#ifndef __PRIVATE_SCORE_H__
#define __PRIVATE_SCORE_H__

#include <stddef.h>

struct score {
    unsigned int number;
    float score;
};

extern struct score score_data_set[];

extern size_t score_nr_elem(void);
extern int score_compare(const void *_elem1, const void *_elem2);

#endif /* __PRIVATE_SCORE_H__ */
