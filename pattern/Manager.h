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

#include "../System.h"

namespace rssd {
namespace pattern {

///
/// @todo Rethink memory ownership of Manager-derived objects.
/// @todo Add a template specialization for shared_ptr items.
/// @todo Add MappedManager implementations for key-value pairs of managed items.
///
template <typename ITEM>
class Manager
{
	public:
		typedef ITEM Item;
		typedef typename std::list<Item> Item_l;
		typedef typename Item_l::iterator Handle;

	public:
		Manager();
		virtual ~Manager();

	public:
		virtual inline Item_l& getItems() { return this->_items; }
		virtual inline const Item_l& getItems() const { return this->_items; }

	public:
		virtual Handle get(const ITEM &item);
		virtual bool has(const ITEM &item);
		virtual uint32_t size() const;
		virtual bool add(ITEM item);
		virtual bool remove(ITEM item);
		virtual void clear();

	protected:
		Item_l _items;
}; // class Manager


/// @todo Claim ownership of memory
/// and store pointers as shared pointers...
template <typename ITEM>
class Manager<ITEM*>
{
	public:
		typedef ITEM Item;
		typedef typename std::list<Item*> Item_l;
		typedef typename Item_l::iterator Handle;

	public:
		Manager();
		virtual ~Manager();

	public:
		virtual Handle get(const ITEM *item);
		virtual bool has(const ITEM *item);
		virtual uint32_t size() const;
		virtual bool add(ITEM *item);
		virtual bool remove(ITEM *item);
		virtual void clear();

	protected:
		Item_l _items;
}; // class Manager

#include "Manager-inl.h"

} // namespace pattern
} // namespace rssd

#endif // RSSD_CORE_PATTERN_MANAGER_H
