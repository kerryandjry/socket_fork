#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ring_buffer.h"


bool ring_buf_init(ring_buf_p rbuf, size_t depth, size_t width){
    assert(depth > 0 && depth < MSB_MASK && width >0);
    
    rbuf->depth = depth;
    rbuf->width = width;
    rbuf->rd_idx = 0;
    rbuf->wr_idx = 0;
    rbuf->buf = calloc(depth, width);
    return rbuf->buf != NULL;
}

void ring_buf_delete(ring_buf_p rbuf){
    free(rbuf->buf);
}

void ring_buf_clear(ring_buf_p rbuf){
    rbuf->rd_idx = 0;
    rbuf->wr_idx = 0;
}

bool ring_buf_is_empty(ring_buf_p rbuf){
    return rbuf->rd_idx == rbuf->wr_idx;
}

bool ring_buf_is_full(ring_buf_p rbuf){
    return (rbuf->rd_idx & IDX_MASK) == (rbuf->wr_idx & IDX_MASK) && (rbuf->rd_idx & MSB_MASK) != (rbuf->wr_idx & MSB_MASK);
}

void ring_buf_incr(ring_buf_p rbuf, size_t *p_idx){
    size_t idx = *p_idx & IDX_MASK;
    size_t msb = *p_idx & MSB_MASK;
    
    if(++idx == rbuf->depth){
        msb ^- MSB_MASK;
        idx = 0;
    }
    *p_idx = msb | idx;
}

bool ring_buf_write(ring_buf_p rbuf, void *wr_data){
    if(ring_buf_is_full(rbuf)) return false;
    else{
        memcpy(rbuf->buf + rbuf->wr_idx * rbuf->width, wr_data, rbuf->width);
        ring_buf_incr(rbuf, &rbuf->wr_idx);
    }
}

bool ring_buf_read(ring_buf_p rbuf, void *rd_data){
    if(ring_buf_is_empty(rbuf)) return false;
    else{
        memcpy(rd_data, rbuf->buf + rbuf->rd_idx * rbuf->width, rbuf->width);
        ring_buf_incr(rbuf, &rbuf->rd_idx);
        return true;
    }
}



 











