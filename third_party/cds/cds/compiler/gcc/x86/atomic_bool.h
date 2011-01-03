/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_COMPILER_GCC_X86_ATOMIC_BOOL_H
#define __CDS_COMPILER_GCC_X86_ATOMIC_BOOL_H

//@cond none
//
// This file is used on x86 and amd64 architecture for GCC compiler
// It is included into appropriate namespace
// Do not use the file directly!!!
//

#       define CDS_atomic_bool_defined

        //////////////////////////////////////////////////////////////////////////
        // Atomic bool

        template <typename ORDER>
        static inline bool load_bool(volatile const bool * p)   ;
        template <>
        inline bool load_bool<membar_relaxed>(volatile const bool * p)
        {
            return *p   ;
        }
        template <>
        inline bool load_bool<membar_consume>(volatile const bool * p)
        {
            return load_bool<membar_relaxed>( p )   ;
        }
        template <>
        inline bool load_bool<membar_acquire>(volatile const bool * p)
        {
            CDS_COMPILER_RW_BARRIER ;
            bool v =  *p   ;
            CDS_COMPILER_RW_BARRIER ;
            return v    ;
        }
        template <>
        inline bool load_bool<membar_seq_cst>(volatile const bool * p)
        {
            CDS_COMPILER_RW_BARRIER ;
            bool b = *p ;
            fence<membar_seq_cst>()   ;
            return b    ;
        }

        template <typename ORDER>
        static inline bool xchg_bool(volatile bool * p, bool val )
        {
            bool ret;
            asm volatile (
                " xchgb %2, %0"
                : "=m" (*p), "=a" (ret)
                : "a" (val), "m" (*p)
                : "memory"
                );
            return (ret);
        }

        template <typename ORDER>
        static inline void store_bool(volatile bool * p, bool bVal )    ;
        template <>
        inline void store_bool<membar_relaxed>(volatile bool * p, bool bVal )
        {
            *p = bVal ;
        }
        template <>
        inline void store_bool<membar_release>(volatile bool * p, bool bVal )
        {
            CDS_COMPILER_RW_BARRIER ;
            *p = bVal ;
            CDS_COMPILER_RW_BARRIER ;
        }
        template <>
        inline void store_bool<membar_seq_cst>(volatile bool * p, bool bVal )
        {
            xchg_bool<membar_seq_cst>( p, bVal )    ;
        }

        class atomic_bool
        {
        public:
            atomic_bool()
                : m_atomic( false )
            {}

            explicit atomic_bool( bool bVal )
                : m_atomic( bVal )
            {}

            bool    load( memory_order order ) const volatile
            {
                bool v = *reinterpret_cast< volatile const bool * >( &m_atomic );
                if ( order == membar_seq_cst::order )
                    fence<membar_seq_cst>()   ;
                return v;
            }

            template <typename ORDER>
            bool    load() const volatile
            {
                return load_bool<ORDER>(&m_atomic) ;
            }

            void store( bool v, memory_order order ) volatile
            {
                if (order != membar_seq_cst::order )
                    m_atomic = v ;
                else
                    exchange<membar_seq_cst>(v);
            }

            template <typename ORDER>
            void store( bool v ) volatile
            {
                store_bool<ORDER>( &m_atomic, v ) ;
            }

            bool exchange(bool val, memory_order order ) volatile
            {
                return xchg_bool<membar_seq_cst>( &m_atomic, val )    ;
            }

            template <typename ORDER>
            bool exchange( bool val ) volatile
            {
                return xchg_bool<ORDER>( &m_atomic, val )    ;
            }

            template <typename SUCCESS_ORDER>
            bool cas( bool bExpected, bool bDesired ) volatile
            {
                bool volatile * pMem = reinterpret_cast<bool volatile *>( &m_atomic ) ;
#       if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1) || CDS_COMPILER_VERSION > 40099
                return __sync_bool_compare_and_swap( pMem, bExpected, bDesired ) ;
#       else
                bool bRet    ;
                asm volatile (
                    "lock; cmpxchgb %2, %0; setz %1"
                    : "+m" (*pMem), "=q" (bRet)
                    : "r" (bDesired), "m" (*pMem), "a" (bExpected)
                    : "cc", "memory"
                    );
                return bRet;
#       endif
            }

            bool cas( bool bExpected, bool bDesired, memory_order /*success_order*/, memory_order /*failure_order*/ ) volatile
            {
                return cas<membar_relaxed>( bExpected, bDesired ) ;
            }

            template <typename SUCCESS_ORDER>
            bool vcas( bool bExpected, bool bDesired ) volatile
            {
                bool volatile * pMem = reinterpret_cast<bool volatile *>( &m_atomic ) ;
#       if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1) || CDS_COMPILER_VERSION > 40099
                return __sync_val_compare_and_swap( pMem, bExpected, bDesired ) ;
#       else
                bool bRet    ;
                asm volatile (
                    "lock; cmpxchgb %2, %0;"
                    : "+m" (*pMem), "=a" (bRet)
                    : "r" (bDesired), "m" (*pMem), "a" (bExpected)
                    : "cc", "memory"
                    );
                return bRet;
#       endif
            }

            bool vcas( bool bExpected, bool bDesired, memory_order /*success_order*/, memory_order /*failure_order*/ ) volatile
            {
                return vcas<membar_relaxed>( bExpected, bDesired )    ;
            }

        private:
            bool volatile m_atomic  ;
        } ;

//@endcond

#endif // #ifndef __CDS_COMPILER_GCC_X86_ATOMIC_BOOL_H
