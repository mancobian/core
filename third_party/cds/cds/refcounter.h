/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_REFCOUNTER_H
#define __CDS_REFCOUNTER_H

/*
    Refernce counting primitives

    Editions:
*/

#include <cds/atomic/atomic.h>

namespace cds {

    /// Simple reference counter
    /**
        Thread-safe reference counter build on atomic primitives.
        @p T is one of atomic types
    */
    template <typename T>
    class ref_counter
    {
        atomic<T>   m_nRefCount    ;        ///< The reference counter

    public:
        typedef T   ref_counter_type  ; ///< The reference counter type

    public:
        /// Construct reference counter with specified value @p initValue
        ref_counter( T initValue = 0 )
            : m_nRefCount(initValue)
        {
            CDS_STATIC_ASSERT( sizeof(T) == sizeof( atomic_t ) )    ;
        }

        /// Get current value of reference counter. The value is loaded with membar_relaxed constraint
        T    getCounter() const
        {
            return m_nRefCount.template load<membar_relaxed>() ;
        }

        /// Get current value of reference counter. \p ORDER is memory order constraint for loading
        template <typename ORDER>
        T   value() const
        {
            return m_nRefCount.template load<ORDER>()    ;
        }

        /// Current value of reference counter. The value is loaded with membar_relaxed constraint
        operator T() const       
        { 
            return getCounter(); 
        }

        /// Atomic increment
        void    inc()
        {
            m_nRefCount.template inc<membar_acquire>() ;
        }

        /// Atomic decrement. Return @a true if reference counter is 0.
        bool    dec()
        {
            return m_nRefCount.template dec<membar_release>() == 1 ;
        }

        /// Atomic increment
        void operator ++()
        {
            inc()    ;
        }

        /// Atomic decrement
        bool operator--()
        {
            return dec()    ;
        }
    };

    /// Signed reference counter
    typedef ref_counter<atomic_t>           signed_ref_counter      ;

    /// Unsigned reference counter
    typedef ref_counter<unsigned_atomic_t>  unsigned_ref_counter    ;


//@cond none
    /* RCObject
        ������, ����� ����� �������� ������������ ��������� ������ �� ����.
        ������ ������� ������� ��� ���� ������� � ����������� �������� ����� �� ������ �������� ������.
        �������, ����������� �� RCObject, ������ ����������� ������ ����������� (newed).

        ������ ����� �� ����� �� ����������. ���������� �� ������ �������� ������������
    */
    class RCObject
    {
        unsigned_ref_counter    m_RefCount    ;

        RCObject( const RCObject& obj )    ;    // ����������� ���������
    public:
        RCObject()
        {}

        RCObject( bool bLocked )
        {
            if ( bLocked )
                acquire()    ;
        }

        virtual ~RCObject()
        {}

    public:    // Operations
        // ���������� �������� ������ �� ������
        void    acquire()        { m_RefCount.inc(); }

        // ���������� �������� ������ �� ������. ���������� false, ���� ������� ������ ����������� �� 0 � ������ ��������������.
        bool    release()
        {
            bool bRet ;
            if ( !( bRet = m_RefCount.dec()) )
                delete this ;
            return bRet    ;
        }
    };

    /* RCptr<T>
        ��������� �� ������ ���� T, ������������ �� RCObject
        ����� ��� ��������� ��������� ������: ����������� ����������� ������� (����� T::acquire),
        ���������� ��������� (����� T::release).
    */
    template <class T>
    class RCptr {
        T *        m_ptr    ;

        void    release()
        {
            if ( m_ptr ) {
                m_ptr->release()    ;
                m_ptr = NULL        ;
            }
        }

        void    assign( T * p )
        {
            if ( p )
                p->acquire()    ;
            m_ptr = p    ;
        }

    public:
        RCptr()
            : m_ptr( NULL )
        {}

        // ����������� ������� ������ (����� acquire)
        RCptr( T * p )
            { assign ( p )    ; }

        // ����������� ������� ������, ���� bAcquire == true
        RCptr( T * p, bool bAcquire )
        {
            if ( bAcquire )
                assign( p )    ;
            else
                m_ptr = p    ;
        }

        // ��������� ������� ������ (����� release)
        ~RCptr()
            { release() ;    }

    public:
        operator const T * () const            { return m_ptr; }
        operator T *()                        { return m_ptr; }

        const T *    operator ->() const        { return m_ptr; }
        T *            operator ->()            { return m_ptr; }
        T **        operator &()
        {
            assert( m_ptr == NULL ) ;
            return &m_ptr   ;
        }

        T *            operator =( T * p )
        {
            release()    ;
            assign( p )    ;
            return p    ;
        }

        bool        operator !() const      { return isNull(); }

    public:
        bool        isNull() const            { return m_ptr == NULL; }

        // �������� �������� ���������� �� ������.
        // ���������� ���������, ������� ������� RCptr, � ��� ����������� ����������� ��������� NULL.
        // ������������ ��������� ������� ���������� ������� release() �� ��������� �������������.
        T *            freeOwnership()
        {
            T * p = m_ptr    ;
            m_ptr = NULL    ;
            return p        ;
        }

        // ������������� ��� acquired-�������� ��������� p
        void setAquired( T * p )
        {
            release()   ;
            m_ptr = p   ;
        }
    };

//@endcond
} // namespace cds

#endif    // #ifndef __CDS_REFCOUNTER_H
