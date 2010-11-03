///
/// @file Singleton.h
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

///
/// Original version Copyright (C) Scott Bilas, 2000.
/// All rights reserved worldwide.
///
/// This software is provided "as is" without express or implied
/// warranties. You may freely copy and compile this source into
/// applications you distribute provided that the copyright text
/// below is included in the resulting source code, for example:
/// "Portions Copyright (C) Scott Bilas, 2000"
///

#ifndef RSSD_CORE_PATTERNS_SINGLETON_H
#define RSSD_CORE_PATTERNS_SINGLETON_H

#include "../System.h"

namespace rssd {
namespace pattern {

template<typename T>
class Singleton
{
	public:
		Singleton();
		virtual ~Singleton();

	public:
		static T& getReference();
		static T* getPointer();

	protected:
		static T* _instance;

	private:
		Singleton(const Singleton<T> &);
		Singleton& operator=(const Singleton<T> &);
}; // namespace Singleton

///
/// Includes
///

#include "Singleton-inl.h"

///
/// Macros
///

#define DECLARE_SINGLETON(TYPENAME) \
template<> TYPENAME* rssd::pattern::Singleton<TYPENAME>::_instance = NULL;

} // namespace pattern
} // namespace rssd

#endif /* RSSD_CORE_PATTERNS_SINGLETON_H */
