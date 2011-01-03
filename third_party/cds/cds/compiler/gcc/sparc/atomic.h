/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_COMPILER_GCC_SPARC_ATOMIC_H
#define __CDS_COMPILER_GCC_SPARC_ATOMIC_H

// For atomic operations the template-based alternatives are defined
#define CDS_ATOMIC_TEMPLATE_DEFINED 1

//@cond none
namespace cds { namespace atomics {
    namespace gcc { namespace Sparc {

        /*
            [Doug Lea "JSR-133 Cookbook for Compiler Writers]:

            Acquire semantics: load; LoadLoad+LoadStore
            Release semantics: LoadStore+StoreStore; store
        */

#   ifdef CDS_SPARC_RMO_MEMORY_MODEL
        // RMO memory model (Linux only?..) Untested
#       define CDS_SPARC_MB_ACQ    "membar #LoadLoad|#LoadStore    \n\t"
#       define CDS_SPARC_MB_REL    "membar #LoadStore|#StoreStore  \n\t"
#       define CDS_SPARC_MB_FULL   "membar #LoadLoad|#LoadStore|#StoreLoad|#StoreStore   \n\t"
#       define CDS_SPARC_MB_SL_LL  "membar #LoadLoad|#StoreLoad     \n\t"
#       define CDS_SPARC_MB_SL_SS  "membar #StoreLoad|#StoreStore   \n\t"
#       define CDS_SPARC_MB_LL     "membar #LoadLoad    \n\t"
#       define CDS_SPARC_MB_ACQ_REL "membar #LoadLoad|#LoadStore|#StoreStore \n\t"
#   else
        // TSO memory model (default; Solaris is used this model)
#       define CDS_SPARC_MB_ACQ
#       define CDS_SPARC_MB_REL
#       define CDS_SPARC_MB_FULL   "membar #StoreLoad   \n\t"
#       define CDS_SPARC_MB_SL_LL  CDS_SPARC_MB_FULL
#       define CDS_SPARC_MB_SL_SS  CDS_SPARC_MB_FULL
#       define CDS_SPARC_MB_LL
#       define CDS_SPARC_MB_ACQ_REL
#   endif

        //////////////////////////////////////////////////////////////////////////
        // fence
        template <typename ORDER>
        static inline void fence()  ;
        template <>
        inline void fence<membar_relaxed>()
        {}
        template <>
        inline void fence<membar_release>()
        {
#       ifdef CDS_SPARC_RMO_MEMORY_MODEL
            asm volatile (
                CDS_SPARC_MB_REL
                : : : "memory"
                ) ;
#       endif
        }
        template <>
        inline void fence<membar_consume>()
        {
#       ifdef CDS_SPARC_RMO_MEMORY_MODEL
            asm volatile (
                CDS_SPARC_MB_LL
                : : : "memory"
                ) ;
#       endif
        }
        template <>
        inline void fence<membar_acquire>()
        {
#       ifdef CDS_SPARC_RMO_MEMORY_MODEL
            asm volatile (
                CDS_SPARC_MB_ACQ
                : : : "memory"
                ) ;
#       endif
        }
        template <>
        inline void fence<membar_acq_rel>()
        {
#       ifdef CDS_SPARC_RMO_MEMORY_MODEL
            asm volatile (
                CDS_SPARC_MB_ACQ_REL
                : : : "memory"
                ) ;
#       endif
        }
        template <>
        inline void fence<membar_seq_cst>()
        {
            asm volatile (
                CDS_SPARC_MB_FULL
                : : : "memory"
                ) ;
        }
        static inline void fence(memory_order order)
        {
            switch( order ) {
                case membar_relaxed::order:
                    fence<membar_relaxed>()   ;
                    break   ;
                case membar_release::order:
                    fence<membar_release>()   ;
                    break   ;
                case membar_consume::order:
                    fence<membar_consume>()   ;
                    break   ;
                case membar_acquire::order:
                    fence<membar_acquire>()   ;
                    break   ;
                case membar_acq_rel::order:
                    fence<membar_acq_rel>()   ;
                    break   ;
                default:
                    assert( false ) ;   // Are we forgot anything?..
                case membar_seq_cst::order:
                    fence<membar_seq_cst>()   ;
                    break   ;
            }
        }

        //
        // 32bit primitives
        //

        //////////////////////////////////////////////////////////////////////////
        // load32

        template <typename ORDER>
        static inline atomic32_t load32( atomic32_t volatile const * pMem ) ;

        template <>
        inline atomic32_t load32<membar_relaxed>( atomic32_t volatile const * pMem )
        {
            atomic32_t val    ;
            asm volatile (
                "lduw [%[pMem]], %[val]  \n\t"
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                :
            ) ;
            return val  ;
        }
        /*
        template <>
        inline atomic32_t load32<membar_release>( atomic32_t volatile const * pMem )
        {
            // Load with release semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            atomic32_t val    ;
            asm volatile (
                CDS_SPARC_MB_SL_LL
                "lduw [%[pMem]], %[val]      \n\t"
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                : "memory"
                ) ;
            return val  ;
        }
        */
        template <>
        inline atomic32_t load32<membar_consume>( atomic32_t volatile const * pMem )
        {
            atomic32_t val    ;
            asm volatile (
                CDS_SPARC_MB_LL
                "lduw [%[pMem]], %[val]      \n\t"
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                : "memory"
                ) ;
            return val  ;
        }
        template <>
        inline atomic32_t load32<membar_acquire>( atomic32_t volatile const * pMem )
        {
            atomic32_t val    ;
            asm volatile (
                "lduw [%[pMem]], %[val]      \n\t"
                CDS_SPARC_MB_ACQ
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                : "memory"
                ) ;
            return val  ;
        }
        template <>
        inline atomic32_t load32<membar_seq_cst>( atomic32_t volatile const * pMem )
        {
            atomic32_t val    ;
            asm volatile (
                CDS_SPARC_MB_SL_LL
                "lduw [%[pMem]], %[val]      \n\t"
                CDS_SPARC_MB_ACQ
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                : "memory"
                ) ;
            return val  ;
        }
        /*
        template <>
        inline atomic32_t load32<membar_acq_rel>( atomic32_t volatile const * pMem )
        {
            // Load with acquire-release semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            return load32<membar_seq_cst>( pMem ) ;
        }
        */
        static inline atomic32_t load32( atomic32_t volatile const * pMem, memory_order order )
        {
            switch (order) {
                case membar_relaxed::order:
                    return load32<membar_relaxed>( pMem ) ;
                //case membar_release::order:
                //    return load32<membar_release>( pMem ) ;
                case membar_consume::order:
                    return load32<membar_consume>( pMem ) ;
                case membar_acquire::order:
                    return load32<membar_acquire>( pMem ) ;
                //case membar_acq_rel::order:
                //    return load32<membar_acq_rel>( pMem ) ;
                default:
                    assert( false ) ;   // Are we forgot anything?..
                case membar_seq_cst::order:
                    return load32<membar_seq_cst>( pMem ) ;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // store32
        template <typename ORDER>
        static inline void store32( atomic32_t volatile const * pMem, atomic32_t val ) ;

        template <>
        inline void store32<membar_relaxed>( atomic32_t volatile const * pMem, atomic32_t val )
        {
            asm volatile (
                "stw %[val], [%[pMem]]  \n\t"
                :
                : [pMem] "r" (pMem), [val] "r" (val)
                :
            ) ;
        }
        /*
        template <>
        inline void store32<membar_acquire>( atomic32_t volatile const * pMem, atomic32_t val )
        {
            // Store with acquire semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            asm volatile (
                "stw %[val], [%[pMem]]  \n\t"
                CDS_SPARC_MB_SL_SS
                :
                : [pMem] "r" (pMem), [val] "r" (val)
                : "memory"
                ) ;
        }

        template <>
        inline void store32<membar_consume>( atomic32_t volatile const * pMem, atomic32_t val )
        {
            // Store with consume semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            store32<membar_acquire>( pMem, val )  ;
        }
        */
        template <>
        inline void store32<membar_release>( atomic32_t volatile const * pMem, atomic32_t val )
        {
            asm volatile (
                CDS_SPARC_MB_REL
                "stw %[val], [%[pMem]]  \n\t"
                :
                : [pMem] "r" (pMem), [val] "r" (val)
                : "memory"
                ) ;
        }

        template <>
        inline void store32<membar_seq_cst>( atomic32_t volatile const * pMem, atomic32_t val )
        {
            asm volatile (
                CDS_SPARC_MB_REL
                "stw %[val], [%[pMem]]  \n\t"
                CDS_SPARC_MB_SL_SS
                :
                : [pMem] "r" (pMem), [val] "r" (val)
                : "memory"
                ) ;
        }
        /*
        template <>
        inline void store32<membar_acq_rel>( atomic32_t volatile const * pMem, atomic32_t val )
        {
            // Store with acquire/release semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            store32<membar_seq_cst>( pMem, val )  ;
        }
        */
        static inline void store32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
        {
            switch (order) {
                case membar_relaxed::order:
                    store32<membar_relaxed>( pMem, val )  ;
                    break;
                //case membar_consume::order:
                //    store32<membar_consume>( pMem, val )  ;
                //    break;
                //case membar_acquire::order:
                //    store32<membar_acquire>( pMem, val )  ;
                //    break;
                case membar_release::order:
                    store32<membar_release>( pMem, val )  ;
                    break;
                //case membar_acq_rel::order:
                //    store32<membar_acq_rel>( pMem, val )  ;
                //    break;
                default:
                    assert( false ) ;   // Are we forgot anything?..
                case membar_seq_cst::order:
                    store32<membar_seq_cst>( pMem, val )  ;
                    break;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // vcas32

        template <typename SUCCESS_ORDER>
        static inline atomic32_t vcas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )  ;

        template <>
        inline atomic32_t vcas32<membar_relaxed>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            atomic32_t current ;
            asm volatile (
                "cas [%[pMem]], %[expected], %[current]    \n\t"
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        template <>
        inline atomic32_t vcas32<membar_acquire>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            atomic32_t current ;
            asm volatile (
                "cas [%[pMem]], %[expected], %[current]    \n\t"
                CDS_SPARC_MB_ACQ
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        template <>
        inline atomic32_t vcas32<membar_consume>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            return vcas32<membar_acquire>( pMem, expected, desired )  ;
        }

        template <>
        inline atomic32_t vcas32<membar_release>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            atomic32_t current ;
            asm volatile (
                CDS_SPARC_MB_REL
                "cas [%[pMem]], %[expected], %[current]    \n\t"
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        template <>
        inline atomic32_t vcas32<membar_acq_rel>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            atomic32_t current ;
            asm volatile (
                CDS_SPARC_MB_REL
                "cas [%[pMem]], %[expected], %[current]    \n\t"
                CDS_SPARC_MB_ACQ
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        template <>
        inline atomic32_t vcas32<membar_seq_cst>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            atomic32_t current ;
            asm volatile (
                CDS_SPARC_MB_FULL
                "cas [%[pMem]], %[expected], %[current]    \n\t"
                CDS_SPARC_MB_FULL
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        static inline atomic32_t vcas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order success_order, memory_order failure_order )
        {

            switch (success_order) {
                case membar_relaxed::order:
                    return vcas32<membar_relaxed>( pMem, expected, desired )   ;
                case membar_consume::order:
                    return vcas32<membar_consume>( pMem, expected, desired )   ;
                case membar_acquire::order:
                    return vcas32<membar_acquire>( pMem, expected, desired )   ;
                case membar_release::order:
                    return vcas32<membar_release>( pMem, expected, desired )   ;
                case membar_acq_rel::order:
                    return vcas32<membar_acq_rel>( pMem, expected, desired )   ;
                default:
                    assert( false ) ;   // Are we forgot anything?..
                case membar_seq_cst::order:
                    return vcas32<membar_seq_cst>( pMem, expected, desired )   ;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // cas32

        template <typename SUCCESS_ORDER>
        static inline bool cas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            return vcas32<SUCCESS_ORDER>( pMem, expected, desired ) == expected ;
        }

        static inline bool cas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order success_order, memory_order failure_order )
        {
            return vcas32( pMem, expected, desired, success_order, failure_order ) == expected ;
        }


        //
        // 64bit primitives
        //

        //////////////////////////////////////////////////////////////////////////
        // load64

        template <typename ORDER>
        static inline atomic64_t load64( atomic64_t volatile const * pMem ) ;

        template <>
        inline atomic64_t load64<membar_relaxed>( atomic64_t volatile const * pMem )
        {
            atomic64_t val    ;
            asm volatile (
                "ldx [%[pMem]], %[val]  \n\t"
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                :
            ) ;
            return val  ;
        }
        /*
        template <>
        inline atomic64_t load64<membar_release>( atomic64_t volatile const * pMem )
        {
            // Load with release semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            atomic64_t val    ;
            asm volatile (
                CDS_SPARC_MB_SL_LL
                "ldx [%[pMem]], %[val]      \n\t"
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                : "memory"
                ) ;
            return val  ;
        }
        */
        template <>
        inline atomic64_t load64<membar_consume>( atomic64_t volatile const * pMem )
        {
            atomic64_t val    ;
            asm volatile (
                CDS_SPARC_MB_LL
                "ldx [%[pMem]], %[val]      \n\t"
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                : "memory"
                ) ;
            return val  ;
        }
        template <>
        inline atomic64_t load64<membar_acquire>( atomic64_t volatile const * pMem )
        {
            atomic64_t val    ;
            asm volatile (
                "ldx [%[pMem]], %[val]      \n\t"
                CDS_SPARC_MB_ACQ
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                : "memory"
                ) ;
            return val  ;
        }
        template <>
        inline atomic64_t load64<membar_seq_cst>( atomic64_t volatile const * pMem )
        {
            atomic64_t val    ;
            asm volatile (
                CDS_SPARC_MB_SL_LL
                "ldx [%[pMem]], %[val]      \n\t"
                CDS_SPARC_MB_ACQ
                : [val] "=r" (val)
                : [pMem] "r" (pMem)
                : "memory"
                ) ;
            return val  ;
        }
        /*
        template <>
        inline atomic64_t load64<membar_acq_rel>( atomic64_t volatile const * pMem )
        {
            // Load with acquire/release semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            return load64<membar_seq_cst>( pMem ) ;
        }
        */
        static inline atomic64_t load64( atomic64_t volatile const * pMem, memory_order order )
        {
            switch (order) {
                case membar_relaxed::order:
                    return load64<membar_relaxed>( pMem ) ;
                //case membar_release::order:
                //    return load64<membar_release>( pMem ) ;
                case membar_consume::order:
                    return load64<membar_consume>( pMem ) ;
                case membar_acquire::order:
                    return load64<membar_acquire>( pMem ) ;
                //case membar_acq_rel::order:
                //    return load64<membar_acq_rel>( pMem ) ;
                default:
                    assert( false ) ;   // Are we forgot anything?..
                case membar_seq_cst::order:
                    return load64<membar_seq_cst>( pMem ) ;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // store64

        template <typename ORDER>
        static inline void store64( atomic64_t volatile const * pMem, atomic64_t val ) ;

        template <>
        inline void store64<membar_relaxed>( atomic64_t volatile const * pMem, atomic64_t val )
        {
            asm volatile (
                "stx %[val], [%[pMem]]  \n\t"
                :
                : [pMem] "r" (pMem), [val] "r" (val)
                :
            ) ;
        }
        /*
        template <>
        inline void store64<membar_acquire>( atomic64_t volatile const * pMem, atomic64_t val )
        {
            // Store with acquire semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            asm volatile (
                "stx %[val], [%[pMem]]  \n\t"
                CDS_SPARC_MB_SL_SS
                :
                : [pMem] "r" (pMem), [val] "r" (val)
                : "memory"
                ) ;
        }

        template <>
        inline void store64<membar_consume>( atomic64_t volatile const * pMem, atomic64_t val )
        {
            // Store with consume semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            store64<membar_acquire>( pMem, val )  ;
        }
        */
        template <>
        inline void store64<membar_release>( atomic64_t volatile const * pMem, atomic64_t val )
        {
            asm volatile (
                CDS_SPARC_MB_REL
                "stx %[val], [%[pMem]]  \n\t"
                :
                : [pMem] "r" (pMem), [val] "r" (val)
                : "memory"
                ) ;
        }

        template <>
        inline void store64<membar_seq_cst>( atomic64_t volatile const * pMem, atomic64_t val )
        {
            asm volatile (
                CDS_SPARC_MB_REL
                "stx %[val], [%[pMem]]  \n\t"
                CDS_SPARC_MB_SL_SS
                :
                : [pMem] "r" (pMem), [val] "r" (val)
                : "memory"
                ) ;
        }
        /*
        template <>
        inline void store64<membar_acq_rel>( atomic64_t volatile const * pMem, atomic64_t val )
        {
            // Store with acquire/release semantics?..
            //CDS_STATIC_ASSERT( false ) ;

            store64<membar_seq_cst>( pMem, val )  ;
        }
        */
        static inline void store64( atomic64_t volatile * pMem, atomic64_t val, memory_order order )
        {
            switch (order) {
                case membar_relaxed::order:
                    store64<membar_relaxed>( pMem, val )  ;
                    break;
                //case membar_consume::order:
                //    store64<membar_consume>( pMem, val )  ;
                //    break;
                //case membar_acquire::order:
                //    store64<membar_acquire>( pMem, val )  ;
                //    break;
                case membar_release::order:
                    store64<membar_release>( pMem, val )  ;
                    break;
                //case membar_acq_rel::order:
                //    store64<membar_acq_rel>( pMem, val )  ;
                //    break;
                default:
                    assert( false ) ;   // Are we forgot anything?..
                case membar_seq_cst::order:
                    store64<membar_seq_cst>( pMem, val )  ;
                    break;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // vcas64

        template <typename SUCCESS_ORDER>
        static inline atomic64_t vcas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )  ;

        template <>
        inline atomic64_t vcas64<membar_relaxed>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            atomic64_t current ;
            asm volatile (
                "casx [%[pMem]], %[expected], %[current]    \n\t"
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        template <>
        inline atomic64_t vcas64<membar_acquire>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            atomic64_t current ;
            asm volatile (
                "casx [%[pMem]], %[expected], %[current]    \n\t"
                CDS_SPARC_MB_ACQ
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        template <>
        inline atomic64_t vcas64<membar_consume>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            return vcas64<membar_acquire>( pMem, expected, desired )  ;
        }

        template <>
        inline atomic64_t vcas64<membar_release>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            atomic64_t current ;
            asm volatile (
                CDS_SPARC_MB_REL
                "casx [%[pMem]], %[expected], %[current]    \n\t"
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        template <>
        inline atomic64_t vcas64<membar_acq_rel>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            atomic64_t current ;
            asm volatile (
                CDS_SPARC_MB_REL
                "casx [%[pMem]], %[expected], %[current]    \n\t"
                CDS_SPARC_MB_ACQ
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        template <>
        inline atomic64_t vcas64<membar_seq_cst>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            atomic64_t current ;
            asm volatile (
                CDS_SPARC_MB_FULL
                "casx [%[pMem]], %[expected], %[current]    \n\t"
                CDS_SPARC_MB_FULL
                : [current] "=r" (current)
                : "0" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
                : "memory"
                );
            return current ;
        }

        static inline atomic64_t vcas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired, memory_order success_order, memory_order failure_order )
        {

            switch (success_order) {
                case membar_relaxed::order:
                    return vcas64<membar_relaxed>( pMem, expected, desired )   ;
                case membar_consume::order:
                    return vcas64<membar_consume>( pMem, expected, desired )   ;
                case membar_acquire::order:
                    return vcas64<membar_acquire>( pMem, expected, desired )   ;
                case membar_release::order:
                    return vcas64<membar_release>( pMem, expected, desired )   ;
                case membar_acq_rel::order:
                    return vcas64<membar_acq_rel>( pMem, expected, desired )   ;
                default:
                    assert( false ) ;   // Are we forgot anything?..
                case membar_seq_cst::order:
                    return vcas64<membar_seq_cst>( pMem, expected, desired )   ;
            }
        }


        //////////////////////////////////////////////////////////////////////////
        // cas64

        template <typename SUCCESS_ORDER>
        static inline atomic64_t cas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            return vcas64<SUCCESS_ORDER>( pMem, expected, desired ) == expected ;
        }
        static inline bool cas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired, memory_order success_order, memory_order failure_order )
        {
            return vcas64( pMem, expected, desired, success_order, failure_order ) == expected ;
        }


        //
        // Atomic pointer primitives
        //
        template <typename SUCCESS_ORDER>
        static inline bool casptr( pointer_t volatile * pMem, pointer_t expected, pointer_t desired )
        {
            return cas64<SUCCESS_ORDER>( (atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired );
        }
        static inline bool casptr( pointer_t volatile * pMem, pointer_t expected, pointer_t desired, memory_order success_order, memory_order failure_order )
        {
            return cas64( (atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired, success_order, failure_order );
        }

        template <typename SUCCESS_ORDER>
        static inline pointer_t vcasptr(  pointer_t volatile * pMem, pointer_t expected, pointer_t desired )
        {
            return (pointer_t) vcas64<SUCCESS_ORDER>( (atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired );
        }
        static inline pointer_t vcasptr(  pointer_t volatile * pMem, pointer_t expected, pointer_t desired, memory_order success_order, memory_order failure_order )
        {
            return (pointer_t) vcas64( (atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired, success_order, failure_order );
        }

        template <typename ORDER>
        static inline pointer_t loadptr( pointer_t volatile const * pMem )
        {
            return (pointer_t) load64<ORDER>( (atomic64_t volatile const *) pMem ) ;
        }
        static inline pointer_t loadptr( pointer_t volatile const * pMem, memory_order order )
        {
            return (pointer_t) load64( (atomic64_t volatile const *) pMem, order ) ;
        }

        template <typename ORDER>
        static inline void storeptr( pointer_t volatile * pMem, pointer_t val )
        {
            store64<ORDER>( (atomic64_t volatile *) pMem, *((atomic64_t *) &val) ) ;
        }
        static inline void storeptr( pointer_t volatile * pMem, pointer_t val, memory_order order )
        {
            store64( (atomic64_t volatile *) pMem, *((atomic64_t *) &val), order ) ;
        }


        //
        // Spin-lock primitives
        //
#        define CDS_spinlock_defined
        typedef unsigned char    spinlock_t    ;
        static inline bool spinlock_try_acquire( spinlock_t volatile * pDest )
        {
            spinlock_t nCur ;
            asm volatile (
                "ldstub    [%[pDest]], %[nCur]    \n\t"
                CDS_SPARC_MB_ACQ
                : [nCur] "=r"(nCur)
                : [pDest] "r"(pDest)
                : "memory", "cc"
                );
            if ( nCur == 0 ) {
                asm volatile ( CDS_SPARC_MB_SL_SS ::: "memory" );
                return true ;
            }
            return false ;
        }
        static inline void spinlock_release( spinlock_t volatile * pDest )
        {
            asm volatile (
                CDS_SPARC_MB_REL
                "stub    %%g0, [%[pDest]]   \n\t"
                :
                : [pDest] "r"(pDest)
                : "memory"
                ) ;
        }

#       undef CDS_SPARC_MB_ACQ
#       undef CDS_SPARC_MB_REL
#       undef CDS_SPARC_MB_FULL
#       undef CDS_SPARC_MB_SL_LL
#       undef CDS_SPARC_MB_SL_SS
#       undef CDS_SPARC_MB_LL
#       undef CDS_SPARC_MB_ACQ_REL

    }} // namespace gcc::Sparc

    namespace platform {
        using namespace gcc::Sparc ;
    }   // namespace platform
}} // cds::atomics
//@endcond

#endif // #ifndef __CDS_COMPILER_GCC_SPARC_ATOMIC_H
