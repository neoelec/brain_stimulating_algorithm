#include <stdio.h>
#include <stdlib.h>

#include "score.h"

int main(void)
{
    struct score key = {
        .score = 671.78,
    };
    struct score *found;
    size_t nmemb = score_nr_elem();

    qsort(score_data_set, nmemb, sizeof(struct score), score_compare);
    found = bsearch(&key, score_data_set, nmemb, sizeof(struct score),
                    score_compare);

    printf("found: %u %f\n", found->number, found->score);

    return 0;
}
