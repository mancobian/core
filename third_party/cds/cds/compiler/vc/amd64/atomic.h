/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_COMPILER_VC_AMD64_ATOMIC_H
#define __CDS_COMPILER_VC_AMD64_ATOMIC_H

#include <intrin.h>
#include <emmintrin.h>  // for 128bit atomic load/store

#pragma intrinsic( _InterlockedIncrement )
#pragma intrinsic( _InterlockedDecrement )
#pragma intrinsic( _InterlockedCompareExchange )
#pragma intrinsic( _InterlockedCompareExchangePointer )
#pragma intrinsic( _InterlockedCompareExchange64 )
#pragma intrinsic( _InterlockedExchange )
#pragma intrinsic( _InterlockedExchange64 )
#pragma intrinsic( _InterlockedExchangePointer )
#pragma intrinsic( _InterlockedExchangeAdd )
#pragma intrinsic( _InterlockedAnd )
#pragma intrinsic( _InterlockedOr )
#pragma intrinsic( _InterlockedXor )
#pragma intrinsic( _InterlockedAnd64 )
#pragma intrinsic( _InterlockedOr64 )
#pragma intrinsic( _InterlockedXor64 )

// CAS128 is supported by MS Visual C++ 2008 and higher
#if _MSC_VER >= 1500

#   pragma intrinsic( _InterlockedCompareExchange128 )

    // Platform supports dword CAS primitive
#   define    CDS_DWORD_CAS_SUPPORTED    1
#endif

// For atomic operations the template-based alternatives are defined
#define CDS_ATOMIC_TEMPLATE_DEFINED 1

//@cond none
namespace cds { namespace atomics {
    namespace vc { namespace amd64 {

        template <typename ORDER>
        static inline void fence()
        {}

        template <>
        inline void fence<membar_seq_cst>()
        {
            _ReadWriteBarrier() ;
        }

        static inline void fence(memory_order order)
        {
            switch( order ) {
                case membar_relaxed::order:
                case membar_release::order:
                case membar_consume::order:
                case membar_acquire::order:
                case membar_acq_rel::order:
                    break;
                default:
                    assert( false ) ;   // Are we forgot anything?..
                case membar_seq_cst::order:
                    fence<membar_seq_cst>()   ;
                    break   ;
            }
        }

        //
        // 32bit atomic primitives
        //
        template <typename SUCCESS_ORDER>
        static inline bool cas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            return _InterlockedCompareExchange( reinterpret_cast< long volatile *>(pMem), desired, expected ) ==  expected ;
        }
        static inline bool cas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order /*success_order*/, memory_order /*failure_order*/ )
        {
            return _InterlockedCompareExchange( reinterpret_cast< long volatile *>(pMem), desired, expected ) ==  expected ;
        }

        template <typename SUCCESS_ORDER>
        static inline atomic32_t vcas32(  atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            return _InterlockedCompareExchange( reinterpret_cast< long volatile *>(pMem), desired, expected ) ;
        }
        static inline atomic32_t vcas32(  atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order /*success_order*/, memory_order /*failure_order*/ )
        {
            return _InterlockedCompareExchange( reinterpret_cast< long volatile *>(pMem), desired, expected ) ;
        }

#       define CDS_xchg32_defined
        template <typename ORDER>
        static inline atomic32_t xchg32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedExchange( reinterpret_cast< long volatile *>(pMem), val ) ;
        }
        static inline atomic32_t xchg32( atomic32_t volatile * pMem, atomic32_t val, memory_order /*order*/ )
        {
            return _InterlockedExchange( reinterpret_cast< long volatile *>(pMem), val ) ;
        }

#       define CDS_xadd32_defined
        template <typename ORDER>
        static inline atomic32_t xadd32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedExchangeAdd( reinterpret_cast< long volatile *>(pMem), val ) ;
        }
        static inline atomic32_t xadd32( atomic32_t volatile * pMem, atomic32_t val, memory_order /*order*/ )
        {
            return _InterlockedExchangeAdd( reinterpret_cast< long volatile *>(pMem), val ) ;
        }

        template <typename ORDER>
        static inline atomic32_t load32( atomic32_t volatile const * pMem )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_consume::order
                || ORDER::order ==  membar_acquire::order
                ) ;

            return *pMem        ;
        }
        template <>
        static inline atomic32_t load32<membar_seq_cst>( atomic32_t volatile const * pMem )
        {
            atomic32_t v = *pMem        ;
            fence<membar_seq_cst>()   ;
            return v                    ;
        }
        static inline atomic32_t load32( atomic32_t volatile const * pMem, memory_order order )
        {
            if ( order == membar_seq_cst::order )
                return load32<membar_seq_cst>( pMem ) ;
            return load32<membar_relaxed>( pMem ) ;
        }


        template <typename ORDER>
        static inline void store32( atomic32_t volatile * pMem, atomic32_t val )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_release::order
                ) ;

            *pMem = val ;
        }
        template <>
        static inline void store32<membar_seq_cst>( atomic32_t volatile * pMem, atomic32_t val )
        {
            xchg32<membar_seq_cst>( pMem, val ) ;
        }
        static inline void store32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
        {
            if (order == membar_seq_cst::order )
                store32<membar_seq_cst>( pMem, val )      ;
            else
                store32<membar_relaxed>( pMem, val )      ;
        }

#       define CDS_and32_defined
        template <typename ORDER>
        static inline atomic32_t and32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedAnd( reinterpret_cast< long volatile *>(pMem), val ) ;
        }
        static inline atomic32_t and32( atomic32_t volatile * pMem, atomic32_t val, memory_order /*order*/ )
        {
            return _InterlockedAnd( reinterpret_cast< long volatile *>(pMem), val ) ;
        }

#       define CDS_or32_defined
        template <typename ORDER>
        static inline atomic32_t or32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedOr( reinterpret_cast< long volatile *>(pMem), val ) ;
        }
        static inline atomic32_t or32( atomic32_t volatile * pMem, atomic32_t val, memory_order /*order*/ )
        {
            return _InterlockedOr( reinterpret_cast< long volatile *>(pMem), val ) ;
        }

#       define CDS_xor32_defined
        template <typename ORDER>
        static inline atomic32_t xor32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedXor( reinterpret_cast< long volatile *>(pMem), val ) ;
        }
        static inline atomic32_t xor32( atomic32_t volatile * pMem, atomic32_t val, memory_order /*order*/ )
        {
            return _InterlockedXor( reinterpret_cast< long volatile *>(pMem), val ) ;
        }

        //
        // 64bit atomic primitives
        //
        template <typename SUCCESS_ORDER>
        static inline bool cas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            return _InterlockedCompareExchange64( pMem, desired, expected ) ==  expected ;
        }
        static inline bool cas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired, memory_order /*success_order*/, memory_order /*failure_order*/ )
        {
            return _InterlockedCompareExchange64( pMem, desired, expected ) ==  expected ;
        }

        template <typename SUCCESS_ORDER>
        static inline atomic64_t vcas64(  atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
        {
            return _InterlockedCompareExchange64( pMem, desired, expected ) ;
        }
        static inline atomic64_t vcas64(  atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired, memory_order /*success_order*/, memory_order /*failure_order*/ )
        {
            return _InterlockedCompareExchange64( pMem, desired, expected ) ;
        }

        template <typename ORDER>
        static inline atomic64_t load64( atomic64_t volatile const * pMem )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_consume::order
                || ORDER::order ==  membar_acquire::order
                ) ;

            return *pMem        ;
        }
        template <>
        static inline atomic64_t load64<membar_seq_cst>( atomic64_t volatile const * pMem )
        {
            atomic64_t v = *pMem        ;
            fence<membar_seq_cst>()   ;
            return v    ;
        }

        static inline atomic64_t load64( atomic64_t volatile const * pMem, memory_order order )
        {
            if ( order == membar_seq_cst::order )
                return load64<membar_seq_cst>( pMem ) ;
            return load64<membar_relaxed>( pMem )     ;
        }

#       define CDS_xchg64_defined
        template <typename ORDER>
        static inline atomic64_t xchg64( atomic64_t volatile * pMem, atomic64_t val )
        {
            return _InterlockedExchange64( pMem, val )  ;
        }
        static inline atomic64_t xchg64( atomic64_t volatile * pMem, atomic64_t val, memory_order /*order*/ )
        {
            return _InterlockedExchange64( pMem, val )  ;
        }

        template <typename ORDER>
        static inline void store64( atomic64_t volatile * pMem, atomic64_t val )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_release::order
                ) ;

            *pMem = val    ;
        }
        template <>
        static inline void store64<membar_seq_cst>( atomic64_t volatile * pMem, atomic64_t val )
        {
            xchg64<membar_seq_cst>( pMem, val )   ;
        }
        static inline void store64( atomic64_t volatile * pMem, atomic64_t val, memory_order order )
        {
            if (order == membar_seq_cst::order )
                store64<membar_seq_cst>( pMem, val )  ;
            else
                store64<membar_relaxed>( pMem, val )  ;
        }

#       define CDS_and64_defined
        template <typename ORDER>
        static inline atomic64_t and64( atomic64_t volatile * pMem, atomic64_t val )
        {
            return _InterlockedAnd64( pMem, val ) ;
        }
        static inline atomic64_t and64( atomic64_t volatile * pMem, atomic64_t val, memory_order /*order*/ )
        {
            return _InterlockedAnd64( pMem, val ) ;
        }

#       define CDS_or64_defined
        template <typename ORDER>
        static inline atomic64_t or64( atomic64_t volatile * pMem, atomic64_t val )
        {
            return _InterlockedOr64( pMem, val ) ;
        }
        static inline atomic64_t or64( atomic64_t volatile * pMem, atomic64_t val, memory_order /*order*/ )
        {
            return _InterlockedOr64( pMem, val ) ;
        }

#       define CDS_xor64_defined
        template <typename ORDER>
        static inline atomic64_t xor64( atomic64_t volatile * pMem, atomic64_t val )
        {
            return _InterlockedXor64( pMem, val ) ;
        }
        static inline atomic64_t xor64( atomic64_t volatile * pMem, atomic64_t val, memory_order /*order*/ )
        {
            return _InterlockedXor64( pMem, val ) ;
        }

        //
        // Atomic pointer primitives
        //
        template <typename SUCCESS_ORDER>
        static inline bool casptr( pointer_t volatile * pMem, pointer_t expected, pointer_t desired )
        {
            return _InterlockedCompareExchangePointer( pMem, desired, expected ) ==  expected ;
        }
        static inline bool casptr( pointer_t volatile * pMem, pointer_t expected, pointer_t desired, memory_order /*success_order*/, memory_order /*failure_order*/ )
        {
            return _InterlockedCompareExchangePointer( pMem, desired, expected ) ==  expected ;
        }

        template <typename SUCCESS_ORDER>
        static inline pointer_t vcasptr(  pointer_t volatile * pMem, pointer_t expected, pointer_t desired )
        {
            return _InterlockedCompareExchangePointer( pMem, desired, expected ) ;
        }
        static inline pointer_t vcasptr(  pointer_t volatile * pMem, pointer_t expected, pointer_t desired, memory_order /*success_order*/, memory_order /*failure_order*/ )
        {
            return _InterlockedCompareExchangePointer( pMem, desired, expected ) ;
        }

#       define CDS_xchgptr_defined
        template <typename ORDER>
        static inline pointer_t xchgptr( pointer_t volatile * pMem, pointer_t val )
        {
            return (pointer_t) xchg64<ORDER>( reinterpret_cast<atomic64_t volatile *>( pMem ), (atomic64_t) val ) ;
        }
        static inline pointer_t xchgptr( pointer_t volatile * pMem, pointer_t val, memory_order order )
        {
            return (pointer_t) xchg64( reinterpret_cast<atomic64_t volatile *>( pMem ), (atomic64_t) val, order ) ;
        }

        template <typename ORDER>
        static inline pointer_t loadptr( pointer_t volatile const * pMem )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_consume::order
                || ORDER::order ==  membar_acquire::order
                ) ;

            return *pMem         ;
        }
        template <>
        static inline pointer_t loadptr<membar_seq_cst>( pointer_t volatile const * pMem )
        {
            pointer_t v = *pMem             ;
            fence<membar_seq_cst>()   ;
            return v    ;
        }
        static inline pointer_t loadptr( pointer_t volatile const * pMem, memory_order order )
        {
            if ( order == membar_seq_cst::order )
                return loadptr<membar_seq_cst>( pMem )    ;
            return loadptr<membar_relaxed>( pMem )        ;
        }

        template <typename ORDER>
        static inline void storeptr( pointer_t volatile * pMem, pointer_t val )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_release::order
                ) ;

            *pMem = val    ;
        }
        template <>
        static inline void storeptr<membar_seq_cst>( pointer_t volatile * pMem, pointer_t val )
        {
            xchgptr<membar_seq_cst>( pMem, val )  ;
        }
        static inline void storeptr( pointer_t volatile * pMem, pointer_t val, memory_order order )
        {
            if (order == membar_seq_cst::order )
                storeptr<membar_seq_cst>( pMem, val ) ;
            else
                storeptr<membar_relaxed>( pMem, val ) ;
        }


        //
        // 128bit intrinsics
        //
#if _MSC_VER >= 1500
#       define CDS_128bit_atomic_defined

        struct CDS_TYPE_ALIGNMENT(16) atomic128_t {
            atomic64_t  lo  ;
            atomic64_t  hi  ;
        } ;

        template <typename ORDER>
        static inline atomic128_t load128( atomic128_t volatile const * pMem )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_consume::order
                || ORDER::order ==  membar_acquire::order
                ) ;
            CDS_COMPILER_RW_BARRIER ;
            __m128i v = _mm_load_si128( (__m128i const *) pMem )  ;
            CDS_COMPILER_RW_BARRIER ;
            return *(atomic128_t *) &v  ;
        }
        template <>
        static inline atomic128_t load128<membar_relaxed>( atomic128_t volatile const * pMem )
        {
            __m128i v = _mm_load_si128( (__m128i const *) pMem )  ;
            return *(atomic128_t *) &v  ;
        }
        template <>
        static inline atomic128_t load128<membar_seq_cst>( atomic128_t volatile const * pMem )
        {
            CDS_COMPILER_RW_BARRIER ;
            __m128i v = _mm_load_si128( (__m128i const *) pMem )  ;
            fence<membar_seq_cst>()   ;
            return *(atomic128_t *) &v      ;
        }
        static inline atomic128_t load128( atomic128_t volatile const * pMem, memory_order order )
        {
            if ( order == membar_seq_cst::order )
                return load128<membar_seq_cst>( pMem )    ;
            return load128<membar_relaxed>( pMem )        ;
        }

        template <typename ORDER>
        static inline void store128( atomic128_t volatile * pMem, const atomic128_t& val )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_release::order ) ;
            CDS_COMPILER_RW_BARRIER ;
            _mm_store_si128( (__m128i *) pMem, *(__m128i *) &val ) ;
            CDS_COMPILER_RW_BARRIER ;
        }
        template <>
        static inline void store128<membar_relaxed>( atomic128_t volatile * pMem, const atomic128_t& val )
        {
            _mm_store_si128( (__m128i *) pMem, *(__m128i *) &val ) ;
        }
        template <>
        static inline void store128<membar_seq_cst>( atomic128_t volatile * pMem, const atomic128_t& val )
        {
            CDS_COMPILER_RW_BARRIER ;
            _mm_store_si128( (__m128i *) pMem, *(__m128i *) &val ) ;
            fence<membar_seq_cst>()   ;
        }
        static inline void store128( atomic128_t volatile * pMem, const atomic128_t& val, memory_order order )
        {
            if ( order == membar_seq_cst::order )
                store128<membar_seq_cst>( pMem, val ) ;
            else
                store128<membar_relaxed>( pMem, val ) ;
        }

        template <typename SUCCESS_ORDER>
        static inline bool cas128( atomic128_t volatile * pDest, const atomic128_t& oldVal, const atomic128_t& newVal )
        {
            CDS_STATIC_ASSERT( sizeof(atomic128_t) == 16 ) ;

            atomic128_t oVal = oldVal ;
            return _InterlockedCompareExchange128( reinterpret_cast<__int64 volatile *>( pDest ),
                newVal.hi, newVal.lo,
                reinterpret_cast<__int64 *>( &oVal ) ) != 0 ;
        }
        static inline bool cas128( atomic128_t volatile * pDest, const atomic128_t& oldVal, const atomic128_t& newVal, memory_order /*success_order*/, memory_order /*failure_order*/ )
        {
            return cas128<membar_relaxed>( pDest, oldVal, newVal )    ;
        }

        template <typename SUCCESS_ORDER>
        static inline atomic128_t vcas128( volatile atomic128_t * pDest, const atomic128_t& oldVal, const atomic128_t& newVal )
        {
            CDS_STATIC_ASSERT( sizeof(atomic128_t) == 16 ) ;

            atomic128_t oVal = oldVal ;
            _InterlockedCompareExchange128( reinterpret_cast<__int64 volatile *>( pDest ),
                newVal.hi, newVal.lo,
                reinterpret_cast<__int64 *>( &oVal ) ) ;
            return oVal     ;
        }
        static inline atomic128_t vcas128( volatile atomic128_t * pDest, const atomic128_t& oldVal, const atomic128_t& newVal, memory_order /*success_order*/, memory_order /*failure_order*/ )
        {
            return vcas128<membar_relaxed>( pDest, oldVal, newVal )    ;
        }
#endif


    }} // namespace vc::amd64

    namespace platform {
        using namespace vc::amd64 ;
    }   // namespace platform
}} // cds::atomics
//@endcond

#endif // #ifndef __CDS_COMPILER_VC_X86_ATOMIC_H