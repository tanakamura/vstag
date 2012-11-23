#include "gc.hpp"
#include "value.hpp"

Object *
new_obj(Klass *klass, int flags)
{
    Object *ret = (Object*)malloc(sizeof(Object));

    ret->klass = klass;
    ret->flags = flags;

    return ret;
}


template <> TagIncludedValue *
alloc_value_vec<TagIncludedValue>(int nelem)
{
    return (TagIncludedValue*)malloc(sizeof(TagIncludedValue) * nelem);
}

template <> TagSeparatedValue *
alloc_value_vec<TagSeparatedValue>(int nelem)
{
    return (TagSeparatedValue*)malloc(sizeof(TagSeparatedValue) * nelem);
}
