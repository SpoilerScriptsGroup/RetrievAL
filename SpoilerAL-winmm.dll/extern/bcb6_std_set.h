#pragma once

#include "bcb6_std_map.h"

typedef struct _Rb_tree_node
bcb6_std_set_node, *bcb6_std_set_iterator, **pbcb6_std_set_iterator;

typedef struct _Rb_tree
bcb6_std_set, *pbcb6_std_set;

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_set_iterator  set_iterator;
typedef pbcb6_std_set_iterator pset_iterator;
typedef bcb6_std_set           set;
typedef pbcb6_std_set          pset;
#define set_begin              bcb6_std_map_begin
#define set_end                bcb6_std_map_end
#define set_find               bcb6_std_map_find
#define set_iterator_decrement bcb6_std_map_iterator_decrement
#define set_erase_sans_delete  bcb6_std_map_erase_sans_delete
#endif
