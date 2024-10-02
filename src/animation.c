#include "animation.h"

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

void animation_render(animation *animation)
{
    /// NOTE: before using uint64_t and doubles this system used simple floats and ints. But frames
    ///       in the animation did not advance accordingly due to precision errors.
    struct timespec timespec;
    assert((timespec_get(&timespec, TIME_UTC)));
    double current_time = timespec.tv_sec + ((double)timespec.tv_nsec / (double)1000000000);

    double spf = (double)animation->seconds_per_frame;
    size_t i = ((uint64_t)floor(current_time / animation->seconds_per_frame)) % animation->frame_count;
    tilemap_render(animation->tilemap, animation->frames[i].x, animation->frames[i].y);
}