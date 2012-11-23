#ifndef VALUE_HPP
#define VALUE_HPP

#include <assert.h>
#include <limits.h>
//#include <gmpxx.h>
#include <stdlib.h>
#include "gc.hpp"

#define MASK_FIXNUM (1)
#define MASK_TRUE (2)
#define MASK_FALSE (3)

typedef long fixint_t;

enum type {
    TYPE_FIXNUM,
    TYPE_FLOAT,
    TYPE_BIGNUM,
    TYPE_STRING,
    TYPE_OBJECT
};

struct Klass {
    
};

extern Klass CBigNum, CFloat;

struct Object {
    static const int TYPE_MASK = 0xf;

    int flags;
    Klass *klass;

    union {
        void *p;
        double fv;
    }u;

    enum type obj_type() const {
        return (enum type)(this->flags & Object::TYPE_MASK);
    }
};

struct TagIncludedValue {
    static const fixint_t FIXNUM_TAG_MAX = LONG_MAX>>2;
    static const fixint_t FIXNUM_TAG_MIN = LONG_MIN>>2;

    union {
        fixint_t iv;
        fixint_t value;
        Object *obj;
    } u;

    static inline bool is_fixnum(fixint_t i)
    {
        return ((((i)&3)) == MASK_FIXNUM);
    }

    enum type type() const {
        if (is_fixnum(u.value)) {
            return TYPE_FIXNUM;
        } else {
            Object *o = this->u.obj;
            return o->obj_type();
        }
    }

    static inline fixint_t int2fix(fixint_t i) {
        return (i<<2) | MASK_FIXNUM;
    }
    static inline fixint_t fix2int(fixint_t i) {
        return i>>2;
    }

    static inline TagIncludedValue int2big(fixint_t i) {
        TagIncludedValue ret;
        ret.u.obj = new_obj(&CBigNum, TYPE_BIGNUM);
        return ret;
    }

    fixint_t to_int() const {
        assert(is_fixnum(u.value));
        return u.iv >> 2;
    }

    double to_float() const {
        assert(! is_fixnum(u.value));
        Object *o = u.obj;
        assert(o->klass == &CFloat);

        return o->u.fv;
    };

    static TagIncludedValue from_int(fixint_t value) {
        if ((value <= FIXNUM_TAG_MAX) &&
            (value >= FIXNUM_TAG_MIN))
        {
            TagIncludedValue ret;
            ret.u.iv = int2fix(value);
            return ret;
        } else {
            return int2big(value);
        }
    }

    static TagIncludedValue from_float(double f) {
        TagIncludedValue ret;
        ret.u.obj = new_obj(&CFloat, TYPE_FLOAT);
        ret.u.obj->u.fv = f;
        return ret;
    }

    TagIncludedValue operator +(TagIncludedValue rhs) const {
        switch (this->type()) {
        case TYPE_FIXNUM:
            switch (rhs.type()) {
            case TYPE_FIXNUM: {
                fixint_t l = fix2int(this->u.iv);
                fixint_t r = fix2int(rhs.u.iv);
                r = l + r;
                return from_int(r);
            }
                break;

            case TYPE_FLOAT: {
                double lv = fix2int(this->u.iv);
                Object *ro = rhs.u.obj;
                return from_float(lv + ro->u.fv);
            }
                break;

            default:
                abort();
            }
            break;

        case TYPE_FLOAT: {
            Object *lo = this->u.obj;
            switch (rhs.type()) {
            case TYPE_FIXNUM: {
                /* fl + fi */
                fixint_t r = fix2int(rhs.u.iv);
                double a = lo->u.fv + r;
                return from_float(a);
            }
                break;
                
            case TYPE_FLOAT: {
                Object *ro = rhs.u.obj;
                /* fl + fl */
                return from_float(lo->u.fv + ro->u.fv);
            }
                break;

            default:
                abort();    // ??
                break;
            }
        }
            break;

        default:
            abort();    // ??
        }
        abort();    // ??
    }
};

struct TagSeparatedValue {
    static const fixint_t FIXNUM_MAX = LONG_MAX;
    static const fixint_t FIXNUM_MIN = LONG_MIN;

    enum type typetag;
    union {
        fixint_t iv;
        double fv;
        void *ov;
    }u;

    fixint_t to_int() const {
        assert(typetag == TYPE_FIXNUM);
        return u.iv;
    }

    double to_float() const {
        assert(typetag == TYPE_FLOAT);
        return u.fv;
    }

    enum type type() const {
        return typetag;
    }

    static inline TagSeparatedValue int2big(fixint_t i) {
        TagSeparatedValue ret;
        ret.typetag = TYPE_OBJECT;
        ret.u.ov = new_obj(&CBigNum, TYPE_BIGNUM);
        return ret;
    }

    static TagSeparatedValue from_int(fixint_t value) {
        if ((value <= FIXNUM_MAX) &&
            (value >= FIXNUM_MIN))
        {
            TagSeparatedValue ret;
            ret.u.iv = value;
            ret.typetag = TYPE_FIXNUM;
            return ret;
        } else {
            return int2big(value);
        }
    }

    static TagSeparatedValue from_float(double value) {
        TagSeparatedValue ret;
        ret.typetag = TYPE_FLOAT;
        ret.u.fv = value;
        return ret;
    }

    TagSeparatedValue operator +(TagSeparatedValue const &rhs) const {
        switch (this->type()) {
        case TYPE_FIXNUM:
            switch (rhs.type()) {
            case TYPE_FIXNUM: {
                fixint_t r = this->u.iv + rhs.u.iv;;
                return from_int(r);
            }
                break;

            case TYPE_FLOAT: {
                double lv = this->u.iv;
                return from_float(lv + rhs.u.fv);
            }
                break;

            default:
                abort();
            }
            break;

        case TYPE_FLOAT: {
            double lv = this->u.fv;
            switch (rhs.type()) {
            case TYPE_FIXNUM: {
                /* fl + fi */
                fixint_t r = rhs.u.iv;
                double a = lv + r;
                return from_float(a);
            }
                break;
                
            case TYPE_FLOAT: {
                return from_float(lv + rhs.u.fv);
            }
                break;
            default:
                abort();
                break;
            }
        }
            break;

        default:
            abort();    // ??
        }
        abort();    // ??
    }

};

#endif
