///
/// @file Error.h
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

#ifndef RSSD_FILESYSTEM_ERROR_H
#define RSSD_FILESYSTEM_ERROR_H

namespace rssd {
namespace system {

///
/// Includes
///

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include "Type.h"

///
/// @brief The following extends the boost error_code system to support custom errors
/// for the rssd::filesystem namespace.
/// @note See: System error support in C++0x - part 4: Creating your own error codes
/// @url http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html
///

///
/// @note Step 1: define the error values
/// 

struct filesystem_error
{
    enum value
    {
        success = 0,
        minuend_does_not_contain_subtrahend,
        count
    }; // enum value
}; // struct filesystem_error

///
/// @class filesystem_category_impl
/// @note Step 2: define an error_category class
///

class filesystem_category_impl : public boost::system::error_category
{
public:
    /// @note Step 3: give the category a human-readable name
    inline virtual const char* name() const 
    { 
        return "rssd::filesystem"; 
    }

    /// @note Step 4: convert error codes to strings
    inline virtual std::string message(int ev) const
    {
        switch (ev)
        {
            case filesystem_error::success:
                return "Success";
            case filesystem_error::minuend_does_not_contain_subtrahend:
                return "The minuend path does not contain the subtrahend path";
            case filesystem_error::count:
            default: 
                return "Unknown filesystem error";
        }
    }
}; // class filesystem_category_impl

///
/// @note Step 5: uniquely identify the category
/// 

const boost::system::error_category& filesystem_category()
{
    static filesystem_category_impl INSTANCE;
    return INSTANCE;
}

///
/// @note Step 6: construct an error_code from the enum
/// 

boost::system::error_code make_error_code(filesystem_error::value e)
{
    return boost::system::error_code(
        static_cast<int>(e),
        filesystem_category());
}

boost::system::error_condition make_error_condition(filesystem_error::value e)
{
    return boost::system::error_condition(
        static_cast<int>(e),
        filesystem_category());
}

} // namespace system
} // namespace rssd

///
/// @note Step 7: register for implicit conversion to error_code
/// 

namespace boost {
namespace system {
template <>
struct is_error_code_enum<rssd::system::filesystem_error::value> : public boost::true_type 
{}; // struct is_error_code_enum
}
}

#endif // RSSD_FILESYSTEM_ERROR_H
