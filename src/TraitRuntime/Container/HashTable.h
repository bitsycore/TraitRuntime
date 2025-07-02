#ifndef HASHTABLE_H
#define HASHTABLE_H

#define HT_NODE(type) HT_##type##_Node

#define HT_DEF_TYPED_NODE(type) typedef struct HT_NODE(type) {\
	type* current;\
	struct HT_##type##_Node* next;\
} HT_NODE(type);

#define HT_INDEX(_hash, _size) ((_hash) & (_size - 1));

#define HT_INSERT(_type, _array, _hash, _table_size, _arena, _new) { \
	size_t ht__hash_index__ = HT_INDEX(_hash, _table_size); \
	HT_NODE(_type)* ht__node__ = &_array[ht__hash_index__]; \
	while (ht__node__->current != NULL) { \
		if (ht__node__->next != NULL) {\
			ht__node__ = ht__node__->next;\
		} else {\
			ht__node__->next = Arena_alloc(_arena, sizeof(HT_NODE(_type)));\
			ht__node__->next->current = NULL;\
			ht__node__->next->next = NULL;\
			break;\
		}\
	}ht__node__->current = _new; \
} NULL

#define HT_GET(_type, _result, _array, _hash, _size, _compare_func, _key) { \
	size_t ht__hash_index__ = HT_INDEX(_hash, _size); \
	const HT_NODE(_type)* ht__node__ = &_array[ht__hash_index__]; \
	while (ht__node__->current != NULL) { \
	if (_compare_func(ht__node__->current, _key)) { \
	_result = ht__node__->current; \
	break; \
	} \
ht__node__ = ht__node__->next; \
} \
}


#endif //HASHTABLE_H
