#include <stdio.h>

#include <rcn/bnrysrch.h>
#include <rcn/quicksrt.h>

#include "score.h"

int main(void)
{
    struct score key = {
        .score = 671.78,
    };
    struct score *found;
    size_t nmemb = score_nr_elem();

    quicksrt(score_data_set, nmemb, sizeof(key), score_compare);
    found = bnrysrch(&key, score_data_set, nmemb, sizeof(key), score_compare);

    printf("found: %u %f\n", found->number, found->score);

    return 0;
}
