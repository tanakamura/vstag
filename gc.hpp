#ifndef VSTAG_GC_HPP
#define VSTAG_GC_HPP

struct Klass;
struct Object;

Object *new_obj(Klass *klass, int flags);
template <typename T> T *alloc_value_vec(int nelem);

#endif
