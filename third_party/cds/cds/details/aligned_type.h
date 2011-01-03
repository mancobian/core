/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_DETAILS_ALIGNED_TYPE_H
#define __CDS_DETAILS_ALIGNED_TYPE_H

#include <cds/details/defs.h>

namespace cds { namespace details {

    /// Aligned type
    /**
        This meta-algorithm solves compiler problem when you need to declare a type \p T with alignment
        equal to another type alignment. For example, the following declaration produces an error in Microsoft Visual Studio 2008 compiler:
        \code
            typedef double  my_double   ;
            typedef __declspec(align( __alignof(my_double) )) int   aligned_int ;
        \endcode
        In MS VS, the __declspec(align(N)) construction requires that N must be a integer constant (1, 2, 4 and so on)
        but not an integer constant expression.

        The result of this meta-algo is a type \p aligned_type<T,ALIGNMENT>::type that is \p T aligned by \p ALIGNMENT.
        For example, with \p aligned_type the prevoius example will not generate an error:
        \code
            typedef double  my_double   ;
            typedef typename cds::details::aligned_type<int, __alignof(my_double)>::type   aligned_int ;
        \endcode
        and result of this declaration is equivalent to
        \code
            typedef __declspec(align(8)) int   aligned_int ;
        \endcode

        The \p ALIGNMENT template parameter must be a constant expression and its result must be power of two.
        The maximum of its value is 128.

        See also \ref align_as
    */
    template <typename T, size_t ALIGNMENT>
    struct aligned_type
#ifdef CDS_DOXYGEN_INVOKED
        {}
#endif
        ;

    //@cond none
#   define CDS_ALIGNED_TYPE_impl(nAlign) template <typename T> struct aligned_type<T,nAlign> { typedef CDS_TYPE_ALIGNMENT(nAlign) T type; }
    CDS_ALIGNED_TYPE_impl(1)        ;
    CDS_ALIGNED_TYPE_impl(2)        ;
    CDS_ALIGNED_TYPE_impl(4)        ;
    CDS_ALIGNED_TYPE_impl(8)        ;
    CDS_ALIGNED_TYPE_impl(16)       ;
    CDS_ALIGNED_TYPE_impl(32)       ;
    CDS_ALIGNED_TYPE_impl(64)       ;
    CDS_ALIGNED_TYPE_impl(128)      ;
#   undef CDS_ALIGNED_TYPE_impl
    //@endcond

    /** Alignment by example

        This meta-algo is similar to \ref aligned_type <T, alignof(ALIGN_AS)>.

        For example, the following code
        \code
        typedef typename cds::details::align_as<int, double>::type   aligned_int ;
        \endcode
        declares type \p aligned_int which is \p int aligned like \p double.

        See also: \ref aligned_type
    */
    template <typename T, typename ALIGN_AS>
    struct align_as {
        /// Result of meta-algo: type \p T aligned like type \p ALIGN_AS
        typedef aligned_type<T, alignof(ALIGN_AS)>::type type    ;
    };

}}  // namespace cds::details

#endif // #ifndef __CDS_DETAILS_ALIGNED_TYPE_H
