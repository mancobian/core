///
/// @file Factory.h
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

#ifndef RSSD_CORE_PATTERN_FACTORY_H
#define RSSD_CORE_PATTERN_FACTORY_H

#include "../System.h"
#include "Manager.h"
#include "Singleton.h"

namespace rssd {
namespace pattern {

template <typename PRODUCT>
class Factory :
	virtual public Manager<PRODUCT*>,
	public boost::noncopyable
{
public:
	typedef PRODUCT Product;

public:
	Factory();
	virtual ~Factory();

public:
  virtual uint32_t getType() const = 0; // { return this->_type; }

public:
	bool operator ==(const uint32_t &value) const;
	bool operator <(const uint32_t &value) const;
	bool operator ==(const Factory &value) const;
	bool operator <(const Factory &value) const;

public:
	virtual PRODUCT* create();
	virtual void destroy(PRODUCT *product);

protected:
	virtual PRODUCT* createImpl() = 0;

protected:
	uint32_t _type;
}; // class Factory

template <typename PRODUCT>
class FactoryManager : virtual public Manager<Factory<PRODUCT>*>
{
public:
	typedef typename std::map<uint32_t, Factory<PRODUCT>*> Factory_m;
	typedef std::map<PRODUCT*, uint32_t> Product_m;

public:
	FactoryManager();
	virtual ~FactoryManager();

public:
	virtual bool hasFactory(const uint32_t type);
	virtual bool hasProduct(const PRODUCT *product);
	virtual bool registerFactory(Factory<PRODUCT> *factory);
	virtual Factory<PRODUCT>* unregisterFactory(const uint32_t type);
	virtual PRODUCT* create(const uint32_t type);
	virtual bool destroy(PRODUCT *product);
	virtual uint32_t generateFactoryId();

protected:
	uint32_t _id;
	Factory_m _factories;
	Product_m _products;
}; // class FactoryManager

#include "Factory-inl.h"

} // namespace pattern
} // namespace rssd

#endif // RSSD_CORE_PATTERN_FACTORY_H
