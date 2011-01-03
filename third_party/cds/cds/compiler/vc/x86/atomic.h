/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_COMPILER_VC_X86_ATOMIC_H
#define __CDS_COMPILER_VC_X86_ATOMIC_H

#include <intrin.h>
#include <emmintrin.h>  // for 64bit atomic load/store
#pragma intrinsic( _InterlockedIncrement )
#pragma intrinsic( _InterlockedDecrement )
#pragma intrinsic( _InterlockedCompareExchange )
//#pragma intrinsic( _InterlockedCompareExchangePointer )   // On the x86 architecture, _InterlockedCompareExchangePointer is a macro that calls _InterlockedCompareExchange
#pragma intrinsic( _InterlockedCompareExchange64 )
#pragma intrinsic( _InterlockedExchange )
//#pragma intrinsic( _InterlockedExchangePointer )  // On the x86 architecture, _InterlockedExchangePointer is a macro that calls _InterlockedExchange
#pragma intrinsic( _InterlockedExchangeAdd )
#pragma intrinsic( _InterlockedXor )
#pragma intrinsic( _InterlockedOr )
#pragma intrinsic( _InterlockedAnd )

// Platform supports dword CAS primitive
#define    CDS_DWORD_CAS_SUPPORTED    1

// For atomic operations the template-based alternatives are defined
#define CDS_ATOMIC_TEMPLATE_DEFINED 1

#if CDS_COMPILER == CDS_COMPILER_MSVC
#   pragma warning( push )
#   if CDS_PROCESSOR_ARCH == CDS_PROCESSOR_X86
//      warning C4311: 'type cast' : pointer truncation from 'cds::atomic_integrals::pointer_t' to 'cds::atomic_integrals::atomic32_t'
//      warning C4312: 'type cast' : conversion from 'cds::atomic_integrals::atomic32_t' to 'cds::atomic_integrals::pointer_t' of greater size
#       pragma warning( disable: 4311 4312 )
#   endif
#endif

//@cond none
namespace cds { namespace atomics {
    namespace vc { namespace x86 {

        template <typename ORDER>
        static inline void fence()
        {}

        template <>
        inline void fence<membar_seq_cst>()
        {
            __asm { mfence }        ;
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
            return _InterlockedCompareExchange( pMem, desired, expected ) ==  expected ;
        }
        static inline bool cas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order success_order, memory_order failure_order )
        {
            return _InterlockedCompareExchange( pMem, desired, expected ) ==  expected ;
        }


        static inline atomic32_t vcas32(  atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order success_order, memory_order failure_order )
        {
            return _InterlockedCompareExchange( pMem, desired, expected ) ;
        }
        template <typename SUCCESS_ORDER>
        static inline atomic32_t vcas32(  atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
        {
            return _InterlockedCompareExchange( pMem, desired, expected ) ;
        }

#       define CDS_xchg32_defined
        static inline atomic32_t xchg32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
        {
            return _InterlockedExchange( pMem, val ) ;
        }
        template <typename ORDER>
        static inline atomic32_t xchg32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedExchange( pMem, val ) ;
        }

#       define CDS_xadd32_defined
        static inline atomic32_t xadd32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
        {
            return _InterlockedExchangeAdd( pMem, val ) ;
        }
        template <typename ORDER>
        static inline atomic32_t xadd32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedExchangeAdd( pMem, val ) ;
        }

        template <typename ORDER>
        static inline atomic32_t load32( atomic32_t volatile const * pMem )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_consume::order
                || ORDER::order ==  membar_acquire::order
                ) ;

            atomic32_t v = *pMem        ;
            return v    ;
        }
        template <>
        inline atomic32_t load32<membar_seq_cst>( atomic32_t volatile const * pMem )
        {
            atomic32_t v = *pMem            ;
            fence<membar_seq_cst>()   ;
            return v    ;
        }
        static inline atomic32_t load32( atomic32_t volatile const * pMem, memory_order order )
        {
            atomic32_t v = *pMem        ;
            if ( order == membar_seq_cst::order )
                fence<membar_seq_cst>()   ;
            return v    ;
        }


        template <typename ORDER>
        static inline void store32( atomic32_t volatile * pMem, atomic32_t val )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_release::order
                ) ;

            *pMem = val    ;
        }
        template <>
        static inline void store32<membar_seq_cst>( atomic32_t volatile * pMem, atomic32_t val )
        {
            xchg32<membar_seq_cst>( pMem, val )  ;
        }

        static inline void store32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
        {
            if (order != membar_seq_cst::order )
                *pMem = val    ;
            else
                store32<membar_seq_cst>( pMem, val )  ;
        }

#       define CDS_and32_defined
        static inline atomic32_t and32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
        {
            return _InterlockedAnd( pMem, val ) ;
        }
        template <typename ORDER>
        static inline atomic32_t and32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedAnd( pMem, val ) ;
        }


#       define CDS_or32_defined
        static inline atomic32_t or32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
        {
            return _InterlockedOr( pMem, val ) ;
        }
        template <typename ORDER>
        static inline atomic32_t or32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedOr( pMem, val ) ;
        }

#       define CDS_xor32_defined
        static inline atomic32_t xor32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
        {
            return _InterlockedXor( pMem, val ) ;
        }
        template <typename ORDER>
        static inline atomic32_t xor32( atomic32_t volatile * pMem, atomic32_t val )
        {
            return _InterlockedXor( pMem, val ) ;
        }

        //
        // Atomic pointer primitives
        //
        static inline bool casptr( pointer_t volatile * pMem, pointer_t expected, pointer_t desired, memory_order success_order, memory_order failure_order )
        {
            // _InterlockedCompareExchangePointer is not defined in intrin.h
            return cas32( (atomic32_t volatile *) pMem, (atomic32_t) expected, (atomic32_t) desired, success_order, failure_order );
        }
        template <typename SUCCESS_ORDER>
        static inline bool casptr( pointer_t volatile * pMem, pointer_t expected, pointer_t desired )
        {
            // _InterlockedCompareExchangePointer is not defined in intrin.h
            return cas32<SUCCESS_ORDER>( (atomic32_t volatile *) pMem, (atomic32_t) expected, (atomic32_t) desired );
        }

        static inline pointer_t vcasptr(  pointer_t volatile * pMem, pointer_t expected, pointer_t desired, memory_order success_order, memory_order failure_order )
        {
            // _InterlockedCompareExchangePointer is not defined in intrin.h
            return (pointer_t) vcas32( (atomic32_t volatile *) pMem, (atomic32_t) expected, (atomic32_t) desired, success_order, failure_order );
        }
        template <typename SUCCESS_ORDER>
        static inline pointer_t vcasptr(  pointer_t volatile * pMem, pointer_t expected, pointer_t desired )
        {
            // _InterlockedCompareExchangePointer is not defined in intrin.h
            return (pointer_t) vcas32<SUCCESS_ORDER>( (atomic32_t volatile *) pMem, (atomic32_t) expected, (atomic32_t) desired );
        }

#       define CDS_xchgptr_defined
        static inline pointer_t xchgptr( pointer_t volatile * pMem, pointer_t val, memory_order order )
        {
            return (pointer_t) xchg32( (atomic32_t volatile *) pMem, *((atomic32_t *) &val), order )   ;
        }
        template <typename ORDER>
        static inline pointer_t xchgptr( pointer_t volatile * pMem, pointer_t val )
        {
            return (pointer_t) xchg32<ORDER>( (atomic32_t volatile *) pMem, *((atomic32_t *) &val) )   ;
        }

        template <typename ORDER>
        static inline pointer_t loadptr( pointer_t volatile const * pMem )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_consume::order
                || ORDER::order ==  membar_acquire::order
                ) ;

            return *pMem    ;
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
            if ( order != membar_seq_cst::order )
                return *pMem ;
            else
                return loadptr<membar_seq_cst>( pMem )    ;
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
            if (order != membar_seq_cst::order )
                *pMem = val    ;
            else
                storeptr<membar_seq_cst>( pMem, val ) ;
        }

        //
        // 64bit atomic primitives
        //
        static inline bool cas64( atomic64_t volatile * pMem, atomic64_unaligned expected, atomic64_unaligned desired, memory_order success_order, memory_order failure_order )
        {
            return _InterlockedCompareExchange64( pMem, desired, expected ) ==  expected ;
        }
        template <typename SUCCESS_ORDER>
        static inline bool cas64( atomic64_t volatile * pMem, atomic64_unaligned expected, atomic64_unaligned desired )
        {
            return _InterlockedCompareExchange64( pMem, desired, expected ) ==  expected ;
        }

        static inline atomic64_t vcas64(  atomic64_t volatile * pMem, atomic64_unaligned expected, atomic64_unaligned desired, memory_order success_order, memory_order failure_order )
        {
            return _InterlockedCompareExchange64( pMem, desired, expected ) ;
        }
        template <typename SUCCESS_ORDER>
        static inline atomic64_t vcas64(  atomic64_t volatile * pMem, atomic64_unaligned expected, atomic64_unaligned desired )
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

            // Atomically loads 64bit value by SSE intrinsics
            __m128i volatile v = _mm_loadl_epi64( (__m128i const *) pMem )    ;
            return v.m128i_i64[0]    ;
        }
        template <>
        static inline atomic64_t load64<membar_seq_cst>( atomic64_t volatile const * pMem )
        {
            // Atomically loads 64bit value by SSE intrinsics
            __m128i volatile v = _mm_loadl_epi64( (__m128i const *) pMem )    ;
            fence<membar_seq_cst>()     ;
            return v.m128i_i64[0]       ;
        }
        static inline atomic64_t load64( atomic64_t volatile const * pMem, memory_order order )
        {
            if ( order == membar_seq_cst::order )
                return load64<membar_seq_cst>( pMem ) ;
            return load64<membar_relaxed>( pMem )     ;
        }

#       define CDS_xchg64_defined
        template <typename ORDER>
        static inline atomic64_t xchg64( atomic64_t volatile * pMem, atomic64_unaligned val )
        {
            atomic64_t expected ;
            do {
                expected = load64<membar_relaxed>( pMem )   ;
            } while ( !cas64<ORDER>( pMem, expected, val ))     ;
            return expected ;
        }
        static inline atomic64_t xchg64( atomic64_t volatile * pMem, atomic64_unaligned val, memory_order order )
        {
            atomic64_t expected ;
            do {
                expected = load64<membar_relaxed>( pMem )   ;
            } while ( !cas64( pMem, expected, val, order, membar_relaxed::order ))     ;
            return expected ;
        }

        template <typename ORDER>
        static inline void store64( atomic64_t volatile * pMem, atomic64_unaligned val )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_release::order
                ) ;

            // Atomically stores 64bit value by SSE intrinsics
            __m128i v   ;
            v.m128i_i64[0] = val    ;
            _mm_storel_epi64( (__m128i *) pMem, v )    ;
        }
        template <>
        static inline void store64<membar_seq_cst>( atomic64_t volatile * pMem, atomic64_unaligned val )
        {
            xchg64<membar_seq_cst>( pMem, val );
        }
        static inline void store64( atomic64_t volatile * pMem, atomic64_unaligned val, memory_order order )
        {
            if (order == membar_seq_cst::order )
                store64<membar_seq_cst>( pMem, val )  ;
            else
                store64<membar_relaxed>( pMem, val )  ;
        }

        //
        // Spin-lock primitives
        //
#        define CDS_spinlock_defined
        typedef unsigned char    spinlock_t    ;
        static inline bool spinlock_try_acquire( spinlock_t volatile * pDest )
        {
            __asm {
                mov        ecx, pDest    ;
                mov        ax, 0x0100    ;
                xor        dx, dx        ;   // dx := 0
                lock    cmpxchg    byte ptr [ecx], ah    ;    // set AL if fail!!!
                cmovnz    ax, dx        ;    // if fail
                setz    al            ;    // if OK
            }
        }
        static inline void spinlock_release( spinlock_t volatile * pDest )
        {
            CDS_COMPILER_RW_BARRIER ;
            *pDest = 0    ;
            CDS_COMPILER_RW_BARRIER ;
        }

        //////////////////////////////////////////////////////////////////////////
        // atomic bool class

#       define CDS_atomic_bool_defined
        template <typename ORDER>
        static inline bool load_bool( bool volatile const * pBool )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_consume::order
                || ORDER::order ==  membar_acquire::order
                ) ;

            return *pBool  ;
        }
        template <>
        static inline bool load_bool<membar_seq_cst>( bool volatile const * pBool )
        {
            bool b =  *pBool  ;
            fence<membar_seq_cst>()   ;
            return b;
        }

        template <typename ORDER>
        static inline bool xchg_bool(bool volatile * pBool, bool val )
        {
            __asm {
                mov al, val     ;
                mov ecx, pBool  ;
                lock xchg byte ptr [ecx], al    ;
            }
        }

        template <typename ORDER>
        static inline void store_bool( bool volatile * pBool, bool bVal )
        {
            CDS_STATIC_ASSERT( ORDER::order ==  membar_relaxed::order
                || ORDER::order ==  membar_release::order
                ) ;

            *pBool = bVal ;
        }
        template <>
        static inline void store_bool<membar_seq_cst>( bool volatile * pBool, bool bVal )
        {
            xchg_bool<membar_seq_cst>( pBool, bVal )    ;
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
                return load_bool<ORDER>( &m_atomic )    ;
            }

            void store( bool v, memory_order order ) volatile
            {
                if (order != membar_seq_cst::order )
                    m_atomic = v ;
                else
                    exchange( v, membar_seq_cst::order );
            }

            template <typename ORDER>
            void store( bool v ) volatile
            {
                store_bool<ORDER>( &m_atomic, v )   ;
            }

            bool exchange(bool val, memory_order order ) volatile
            {
                return xchg_bool<membar_seq_cst>( &m_atomic, val ) ;
            }

            template <typename ORDER>
            bool exchange(bool val) volatile
            {
                return xchg_bool<ORDER>( &m_atomic, val ) ;
            }

            template <typename SUCCESS_ORDER>
            bool cas( bool bExpected, bool bDesired ) volatile
            {
                bool volatile * p = reinterpret_cast<bool volatile *>( &m_atomic ) ;
                bool bRet = false       ;
                __asm {
                    mov ecx, p          ;
                    mov al, bExpected   ;
                    mov ah, bDesired    ;
                    lock cmpxchg byte ptr [ecx], ah  ;
                    setz bRet           ;
                }
                return bRet ;
            }

            bool cas( bool bExpected, bool bDesired, memory_order success_order, memory_order failure_order ) volatile
            {
                return cas<membar_relaxed>( bExpected, bDesired ) ;
            }

            template <typename SUCCESS_ORDER>
            bool vcas( bool bExpected, bool bDesired ) volatile
            {
                bool volatile * p = reinterpret_cast<bool volatile *>( &m_atomic ) ;
                bool bRet ;
                __asm {
                    mov ecx, p          ;
                    mov al, bExpected   ;
                    mov ah, bDesired    ;
                    lock cmpxchg byte ptr [ecx], ah  ;
                    mov bRet, al        ;
                }
                return bRet ;
            }

            bool vcas( bool bExpected, bool bDesired, memory_order success_order, memory_order failure_order ) volatile
            {
                return vcas<membar_relaxed>( bExpected, bDesired )    ;
            }

        private:
            bool volatile m_atomic  ;
        } ;

    }} // namespace vc::x86

    namespace platform {
        using namespace vc::x86 ;
    }   // namespace platform
}} // cds::atomics
//@endcond

#if CDS_COMPILER == CDS_COMPILER_MSVC
#   pragma warning( pop )
#endif


#endif // #ifndef __CDS_COMPILER_VC_X86_ATOMIC_H