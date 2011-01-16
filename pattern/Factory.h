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
///     this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
///     this list of conditions and the following disclaimer in the documentation
///     and/or other materials provided with the distribution.
///    * Neither the name of Royal Society of Secret Design nor the names of its
///     contributors may be used to endorse or promote products derived from
///     this software without specific prior written permission.
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

#include "System"
#include "Manager.h"
#include "Singleton.h"

namespace RSSD {
namespace Core {
namespace Pattern {

/// @note The base Factory<> class will never exist.
///   This pattern only provides for a Manager class
///   that will manage a collection of Factory<>::Impl<>
///   types.
template <typename T>
class Factory
{
public:
  template <typename U>
  class Impl :
    public Pattern::Singleton<Factory<T>::Impl<U> >,
    public Pattern::Factory<T>,
    public Pattern::Manager<T*>
  {
  public:
    /// @todo Research way(s) to make this a compile-time constant integer.
    static /*const*/ uint32_t TYPE;

    Impl();
    virtual ~Impl();
    virtual const uint32_t getType() const { return Impl<U>::TYPE; }
    virtual T* create(params_t &params = params_t());
    virtual void destroy(T *value);
  }; /// class Impl

  class Manager :
    public Pattern::Singleton<Factory<T>::Manager>,
    public Pattern::Manager<Factory<T>*>
  {
  public:
    typedef typename std::map<uint_t, Factory<T>*> FactoryMap;

    Manager();
    virtual ~Manager();
    bool hasFactory(const uint_t type);
    Factory<T>* getFactory(const uint_t type);
    bool registerFactory(Factory<T> *factory);
    Factory<T>* unregisterFactory(const uint_t type);

  protected:
    FactoryMap mFactories;
  }; // class Manager

  Factory();
  virtual ~Factory();
  bool operator ==(const uint32_t &value) const;
  bool operator <(const uint32_t &value) const;
  bool operator ==(const Factory &value) const;
  bool operator <(const Factory &value) const;
  virtual const uint32_t getType() const = 0;
  virtual T* create(params_t &params = params_t()) = 0;
  virtual void destroy(T *value) = 0;

public:
  static uint32_t TYPEID;
}; // class Factory

#include "Factory-inl.h"

} // namespace Pattern
} // namespace Core
} // namespace RSSD

#endif // RSSD_CORE_PATTERN_FACTORY_H
