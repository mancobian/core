///
/// @file Manager.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by Royal Society of Secret Design
/// All rights reserved
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
///    * Redistributions of source code must retain the above copyright notice,
/// 		this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
/// 		this list of conditions and the following disclaimer in the documentation
/// 		and/or other materials provided with the distribution.
///    * Neither the name of Royal Society of Secret Design nor the names of its
/// 		contributors may be used to endorse or promote products derived from
/// 		this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
/// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///

#ifndef RSSD_CORE_PATTERN_MANAGER_H
#define RSSD_CORE_PATTERN_MANAGER_H

#include "System"

namespace RSSD {
namespace Core {
namespace Pattern {

///
/// @plan Use raw pointers if you intend for a MANAGER
///   derived object to manage memory de-allocation.
///   Otherwise, if you would like a MANAGER to maintain
///   a collection of pointers but NOT to manage the
///   de-allocation of the referenced memory, use a weak
///   pointer object to wrap your pointers, such as
///   boost::weak_ptr or std::tr1::weak_ptr.
///
///   Weak pointer wrappers will cause submitted items
///   to be viewed a value types, which will be managed
///   by the value type manager:
///     - template <typename ITEM> Manager
///   instead of the reference type manager:
///     - template <typename ITEM*> Manager
///
///   An example of this can be seen in Pattern::Publisher,
///   which is a manager of Subscriber objects but should
///   not be responsible for their memory management, e.g. de-allocation.
///

///
/// @todo Rethink memory ownership of Manager-derived objects.
/// @todo Add a template specialization for shared_ptr items.
/// @todo Add MappedManager implementations for key-value pairs of managed items.
/// @todo Consider how C++ Traits may be used to reduce code duplication between
///   Manager class template specializations.
///
template <typename ITEM>
class Manager : public boost::noncopyable
{
	public:
		typedef ITEM Item;
		typedef typename std::list<Item> ItemList;
		typedef typename ItemList::iterator Handle;
		typedef std::tr1::shared_ptr<Manager<ITEM> > Pointer;

	public:
		Manager();
		virtual ~Manager();

	public:
		virtual inline ItemList& getItems() { return this->_items; }
		virtual inline const ItemList& getItems() const { return this->_items; }

	public:
		virtual Handle get(const ITEM &item);
		virtual bool has(const ITEM &item);
		virtual uint32_t size() const;
		virtual bool add(ITEM item);
		virtual bool remove(ITEM item);
		virtual void clear();

	protected:
		ItemList _items;
}; // class Manager

template <typename ITEM>
class Manager<std::tr1::weak_ptr<ITEM> > : public boost::noncopyable
{
public:
  typedef std::tr1::weak_ptr<ITEM> Item;
  typedef typename std::list<Item> ItemList;
  typedef typename ItemList::iterator Handle;
  typedef std::tr1::shared_ptr<Manager<Item> > Pointer;

public:
  Manager();
  virtual ~Manager();

public:
  virtual inline ItemList& getItems() { return this->_items; }
  virtual inline const ItemList& getItems() const { return this->_items; }

public:
  virtual Handle get(const Item &item);
  virtual bool has(const Item &item);
  virtual uint32_t size() const;
  virtual bool add(Item item);
  virtual bool remove(Item item);
  virtual void clear();

protected:
  ItemList _items;
};

template <typename ITEM>
class Manager<ITEM*> : public boost::noncopyable
{
public:
  typedef ITEM Item;
  typedef typename std::list<Item*> ItemList;
  typedef typename ItemList::iterator Handle;
  typedef std::tr1::shared_ptr<Manager<ITEM*> > Pointer;

public:
  Manager();
  virtual ~Manager();

public:
  virtual Handle get(const Item *item);
  virtual bool has(const Item *item);
  virtual uint32_t size() const;
  virtual bool add(Item *item);
  virtual bool remove(Item *item);
  virtual void clear();

protected:
  ItemList _items;
}; // class Manager

#include "Manager-inl.h"

} // namespace Pattern
} // namespace Core
} // namespace RSSD

#endif // RSSD_CORE_PATTERN_MANAGER_H
