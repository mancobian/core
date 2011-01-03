/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_DETAILS_TYPE_PADDING_H
#define __CDS_DETAILS_TYPE_PADDING_H

namespace cds { namespace details {

    //@cond none
    template <typename T, int MODULO>
    struct type_padding_helper: public T
    {
        char    _[MODULO]   ;   // padding
    };
    template <typename T>
    struct type_padding_helper<T, 0>: public T
    {};
    //@endcond

    /// Automatic alignment type \p T to \p ALIGN_FACTOR
    /**
        The class adds appropriate bytes to type T that the following condition is true:
        \code
        sizeof( type_padding<T,ALIGN_FACTOR>::type ) % ALIGN_FACTOR == 0
        \endcode
        It is guaranteeded that count of padding bytes no more than ALIGN_FACTOR - 1.

        \b Applicability: type \p T must not have constructors another that default ctor.
        For example, \p T may be any POD type.
    */
    template <typename T, int ALIGN_FACTOR>
    class type_padding {
    public:
        /// Result type
        typedef type_padding_helper<T, sizeof(T) % ALIGN_FACTOR>    type    ;
    };

}}   // namespace cds::details
#endif // #ifndef __CDS_DETAILS_TYPE_PADDING_H
