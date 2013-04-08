/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#ifndef _REDUCTIONS_H
#define _REDUCTIONS_H

#include "config.h"
#include <stdint.h>
#include <libcouchstore/visibility.h>
#include <libcouchstore/couch_db.h>
#include <libcouchstore/couch_common.h>
#include "bitmap.h"
#include "sorted_list.h"
#include "../node_types.h"
#include "../bitfield.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    uint64_t                kv_count;
    bitmap_t                partitions_bitmap;
    /* number of elements in reduce_values */
    uint8_t                 num_values;
    sized_buf               *reduce_values;
} view_btree_reduction_t;

typedef struct {
    uint64_t                kv_count;
    bitmap_t                partitions_bitmap;
} view_id_btree_reduction_t;

LIBCOUCHSTORE_API
couchstore_error_t decode_view_btree_reductions(const char *bytes, size_t len,
                                                view_btree_reduction_t **reduction);

LIBCOUCHSTORE_API
couchstore_error_t encode_view_btree_reductions(const view_btree_reduction_t *reduction,
                                                char **buffer, size_t *buffer_size);
LIBCOUCHSTORE_API
void free_view_btree_reductions(view_btree_reduction_t *reduction);

LIBCOUCHSTORE_API
couchstore_error_t decode_view_id_btree_reductions(const char *bytes,
                                                    view_id_btree_reduction_t **reduction);

LIBCOUCHSTORE_API
couchstore_error_t encode_view_id_btree_reductions(const view_id_btree_reduction_t *reduction,
                                                    char **buffer, size_t *buffer_size);

LIBCOUCHSTORE_API
void free_view_id_btree_reductions(view_id_btree_reduction_t *reduction);

#ifdef __cplusplus
}
#endif

#endif