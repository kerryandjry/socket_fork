#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define IDX_MASK (SIZE_MAX>>1)
#define MSB_MASK (~IDX_MASK)

struct ring_buf
{
    size_t depth;
    size_t width;
    size_t rd_idx;
    size_t wr_idx;
    void *buf;
};

typedef struct ring_buf ring_buf_t;
typedef struct ring_buf* ring_buf_p;

bool ring_buf_init(ring_buf_p rbuf, size_t depth, size_t width);
void ring_buf_delete(ring_buf_p rbuf);
void ring_buf_clear(ring_buf_p rbuf);
bool ring_buf_is_empty(ring_buf_p rbuf);
bool ring_buf_is_full(ring_buf_p rbuf);
void ring_buf_incr(ring_buf_p rbuf, size_t *p_idx);
bool ring_buf_write(ring_buf_p rbuf, void *wr_data);
bool ring_buf_read(ring_buf_p rbuf, void *rd_data);

#endif
