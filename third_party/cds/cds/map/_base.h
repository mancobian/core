/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_MAP__BASE_H
#define __CDS_MAP__BASE_H

/*
    Filename: map/_base.h

    Description:
        Base declarations for maps


    Editions:
        2008.10.06    Maxim.Khiszinsky    Created
*/

#include <cds/details/defs.h>

#include <cds/details/void_selector.h>
#include <cds/details/comparator.h>
#include <cds/atomic/atomic.h>

#include <boost/functional/hash.hpp>

/**\page map_common_interface Map common interface
    Any map implementation in CDS library supports the following interface:

    \par Stable interface

\member{insert}
\code
    bool insert (const KEY &key, const VALUE &val)
\endcode
        Insert new item (\a key, \a val) into the map
        \param key a key
        \param val a value
        \return \c true if insertion succeeded, \c false otherwise (i.e. the \a key already in the map)

\member{ensure}
\code
    template <typename FUNC>
    std::pair< bool, bool > ensure (const KEY &key, const VALUE &val, FUNC func)
\endcode
        Ensures that key \a key exists in map, changes data of existing item to \a val.
        \param key a key
        \param val a value
        \param func the function making change if the key exists in the map
    \details The operation performs insert or change data with lock-free manner.

        If \p key exists in the map then changes the value of the item pointed by \p key to \p val.
        The change is performed by calling user-defined function \p func with signature:
        \code
        void func( VALUE& itemValue, const VALUE& val ) ;
        \endcode
        The first argument \p itemValue of user-defined function \p func is the reference
        to the map's item pointed by key \p key. The second argument \p val is the value passed
        to \p ensure member function. User-defined function \p func must guarantee that during changing
        key's value no any other changes or reads could be made on this map item by concurrent threads.
        The user-defined function is called only if the key exists in the map.

        If \p key is not in the map then add it.
    \return \c std::pair<bool, bool> where \c first is \c true if operation is successfull,
        \c second is \c true if new item has been added or \c false if the item with \a key already in the map.

\member{emplace}
\code
    template <typename T, typename FUNC>
    bool emplace( const KEY& key, const T& val, FUNC func )
\endcode
    Emplaces the vale of key \p key with new value \p val
    \param key a key to find
    \param val new value
    \param func function to change the value of key \p key
    \details The operation changes the value (or a part of the value) of key \p key to new one \p val.
        The user-defined functor \p func has the purpose like \p func argument of \ref ensure
        member function. The signature of \p func is
        \code
        void func( VALUE& itemValue, const T& val ) ;
        \endcode
        The first argument \p itemValue of user-defined function \p func is the reference
        to the map's item pointed by key \p key. The second argument \p val is the value passed
        to \p emplace member function. User-defined function \p func must guarantee that during changing
        key's value no any other changes could be made on this map's item by concurrent thread.
        The map only garantees that the item found by key cannot be deleted while \p func worked

    \return The function returns \p true if the key \p key exists in the map, \p false otherwise

\member{erase}
\code
    bool erase (const KEY &key)
\endcode
        Erase a key \a key from the map
        \param key a key for deleting
        \return \c true if key \a key found and deleted from the map, \c false if the \a key is not found in the map
        \note The \c erase method is applicable for non-persistent implementation only

\member{find}
\code
    bool find (const KEY &key)
\endcode
        Find a key \c key in the map
        \param key a key finding for
        \return \c true if \a key found, \c false otherwise

\member{find}
\code
    template <typename T, typename FUNC>
    bool find (const KEY &key, T &data, FUNC func)
\endcode
        Find key in the map. If key is found returns its data in \a data parameter.
        \param key a key
        \param val a value found
        \param func the user-defined function to copy item's value
    \return \c true if \a key found, \c false otherwise
    \details If \p key found the function calls user-defined function \p func with parameters:
        \code
        void func( T& data, const VALUE& itemValue )
        \endcode
        where \p itemValue is the item found by \p key. The user-defined function
        copies the item's value \p itemValue or its part to \p data. The map guarantees only
        that the item found cannot be deleted while \p func works. The user-defined function \p func
        should take into account that concurrent threads may change the item value.

\member{empty}
\code
    bool empty () const
\endcode
        Check if the map is empty.
        \return \c true if the map is empty, \c false otherwise

\member{clear}
\code
    void clear ()
\endcode
        Clears the map. For many map implementation this function is not thread safe.

\member{size}
\code
    size_t size() const
\endcode
        Returns item count.
        Note that \c size()==0 is not equal to \c empty()==true because of lock-free nature of
        the algorithms implemented in CDS library. The value returned is approximate estimation of map's item count.
        To check whether the map is empty you should use \c empty() method.
*/
namespace cds {

    /// Map implementations
    /**
        Any implementation of the map in \p CDS library is derived from class map_tag. It means that
            \code
                boost::type_traits::is_base_of< cds::map::map_tag, L>::value
            \endcode
        is \p true iff L is an implementation of the map.

        The common interface of the map implementation see \ref map_common_interface.
    */
    namespace map {

        /// Key traits
        template <class KEY>
        struct key_traits {
            typedef KEY                                        key_type        ;    ///< key type
            typedef cds::details::Comparator< KEY >            key_comparator    ;    ///< Key comparator type (binary predicate)

            typedef boost::hash< KEY >                        hash_functor    ;    ///< Hasher (unary functor)
            typedef typename hash_functor::result_type        hash_type        ;    ///< Type of hash value
            typedef cds::details::Comparator< hash_type >    hash_comparator    ;    ///< Comparator for hash values (binary functor)
        }    ;

        /// Value traits
        template <class T>
        struct value_traits {
            typedef T                                value_type    ;    ///< value type
        };

        /// Pair (key, value) traits
        template <class KEY, class VALUE>
        struct pair_traits {
            typedef map::key_traits<KEY>        key_traits        ;    ///< Key traits type
            typedef map::value_traits<VALUE>    value_traits    ;    ///< Value traits type
        };

        /// Empty hash map statistics
        struct empty_statistics {};

        /// Generic map traits
        /**
            Many typedefs in this struct is \p void. The map implementation selects appropriate
            default type if corresponding MapTraits typedef is \p void. cds::details::void_selector is useful for this purpose.
        */
        struct type_traits {
            typedef void                bucket_type    ;    ///< type of bucket. Usually, it is one of ordered list implementation. Default - MichaelList<cds::gc::hzp_gc>
            typedef void                item_counter_type;    ///< item counter. If void then no item counter is used (EmptyItemCouner)
            typedef empty_statistics    statistics    ;    ///< Internal statistics implementation
        };

        /// This empty class is like a marker for the map implementations.
        /**
            In CDS library, for any map implementation L the value of
            \code
                boost::type_traits::is_base_of< cds::map::map_tag, L>::value
            \endcode
            is \p true.
        */
        struct map_tag {};

        /// Base of any map implementation in the library
        /**
            \par Template parameters
                \li \p KEY        type of key stored in list
                \li \p VALUE    type of value stored in list
                \li \p PAIR_TRAITS (key, value) pair traits. The default is cds::map::pair_traits <\p KEY, \p VALUE>
                \li \p TRAITS    map traits. The default is cds::map::type_traits. Different map implementation requires different
                                traits. The cds::map::type_traits class provides generic implementation of map traits.
        */
        template <
            typename KEY,
            typename VALUE,
            typename PAIR_TRAITS = pair_traits< KEY, VALUE >,
            typename TRAITS = type_traits
        >
        struct map_base: public map_tag {
            typedef KEY        key_type    ;    ///< key type
            typedef VALUE    value_type    ;    ///< value type

            typedef typename cds::details::void_selector<
                PAIR_TRAITS,
                map::pair_traits< KEY, VALUE >
            >::type                            pair_traits;    ///< Pair traits

            typedef typename cds::details::void_selector<
                TRAITS,
                map::type_traits
            >::type                            type_traits    ;        ///< Map traits

            /// item counter type. Default is cds::atomics::empty_item_counter if TRAITS::item_counter_type type is void
            typedef typename cds::details::void_selector<
                typename type_traits::item_counter_type,
                atomics::empty_item_counter
            >::type                            item_counter_type ;

            typedef typename pair_traits::key_traits        key_traits        ;    ///< key traits
            typedef typename pair_traits::value_traits        value_traits    ;    ///< value traits type

            typedef typename key_traits::hash_functor        hash_functor    ;    ///< hash function
            typedef typename key_traits::hash_type            hash_type        ;    ///< hash function result type
            typedef typename key_traits::hash_comparator    hash_comparator    ;    ///< hash comparator
        };

    }    // namespace map
}    // namespace cds

#endif  // #ifndef __CDS_MAP__BASE_H
