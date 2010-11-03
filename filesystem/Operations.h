///
/// @file Operations.h
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

#ifndef RSSD_FILESYSTEM_HELPERPATH_H
#define RSSD_FILESYSTEM_HELPERPATH_H

///
/// Includes
///

#include <boost/assign.hpp>
#include "../System.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED

namespace boost {
namespace filesystem {
namespace extended {

///
/// Macros
/// @note Boost system error codes can be found in <boost/system/error_code.hpp>.
///

#define FILESYSTEM_ERROR_CODE(CODE) \
    boost::system::error_code( \
        CODE, \
        boost::system::system_category)
        
#define THROW_FILESYSTEM_ERROR(PATH, CODE) \
    throw boost::filesystem::basic_filesystem_error<boost::filesystem::path>( \
        __func__, \
        PATH, \
        FILESYSTEM_ERROR_CODE(CODE));

///
/// @note See: <boost/filesystem/operations.hpp>
/// typedef boost::filesystem::path Path; needs to be in namespace boost::filesystem
///

# ifndef BOOST_FILESYSTEM_NARROW_ONLY
#   define BOOST_FS_FUNC(BOOST_FS_TYPE) \
      template<class Path> typename boost::enable_if<is_basic_path<Path>, \
      BOOST_FS_TYPE>::type
#   define BOOST_INLINE_FS_FUNC(BOOST_FS_TYPE) \
      template<class Path> inline typename boost::enable_if<is_basic_path<Path>, \
      BOOST_FS_TYPE>::type
#   define BOOST_FS_TYPENAME typename
# else
#   define BOOST_FS_FUNC(BOOST_FS_TYPE) inline BOOST_FS_TYPE
#   define BOOST_INLINE_FS_FUNC(BOOST_FS_TYPE) inline BOOST_FS_TYPE
    typedef boost::filesystem::path Path;
#   define BOOST_FS_TYPENAME
# endif

///
/// Structs
///

template<class Path> struct dot { static const char value; };
template<class Path> const char dot<Path>::value = '.';

template<class Path> struct dotdot { static const char* value; };
template<class Path> const char* dotdot<Path>::value = "..";

///
/// Functions
///

BOOST_FS_FUNC(bool)
is_hidden(const Path &target_path)
{
	std::string filename = target_path.filename();
	if (!filename.empty() && filename[0] == '.')
		return true;
	return false;
}

BOOST_FS_FUNC(Path)
normalize(const Path &target_path)
{
    // std::cout << "normalize(" << target_path << ")" << std::endl;

    Path result;
    boost::filesystem::path::iterator
        value_iter = target_path.begin(),
        end_iter = target_path.end();
    for(; value_iter != end_iter; ++value_iter)
    {
        if (*value_iter == extended::dotdot<Path>::value)
        {
            result = result.parent_path();
        }
        else if (*value_iter == &boost::filesystem::extended::dot<Path>::value)
        {
            // Ignore
        }
        else
        {
            // Append
            result /= *value_iter;
        }

        // std::cout << '\t' << "+ " << *value_iter << ": " << result << std::endl;
    } // end for(...)

    // if (!exists(result))
    //    THROW_FILESYSTEM_ERROR(result, boost::system::errc::no_such_file_or_directory);

    return result;
}

#if 0
IN: /usr/local/lib/test.a, RESULT: 4
IN: /usr/local/lib/, RESULT: 3
IN: /usr/local/, RESULT: 2
IN: /usr/, RESULT: 1
IN: /usr/test.a, RESULT: 2
IN: /, RESULT: 1
IN: NULL, RESULT: 0
#endif

///
/// @note Required because boost::filesystem::path::remove_filename(...)
/// does not check if the last component of a given path is actually
/// a regular file or directory.
/// 

BOOST_FS_FUNC(Path) 
get_directory(const Path &target_path)
{
    if (!exists(target_path))
        THROW_FILESYSTEM_ERROR(target_path, boost::system::errc::no_such_file_or_directory);
    if (is_regular_file(target_path))
        return target_path.parent_path();
    if (is_directory(target_path))
        return target_path;
    return target_path;
}

BOOST_FS_FUNC(uint32_t) 
get_depth(const Path &target_path)
{
    uint32_t depth = 0;

    Path target_path_copy(normalize(target_path));
    
    boost::filesystem::path::iterator 
        path_iter = target_path_copy.begin(),
        end_iter = target_path_copy.end();
    for (; path_iter != end_iter; ++path_iter) 
    { 
        if (*path_iter == &boost::filesystem::extended::dot<Path>::value)
            continue;
        else if (*path_iter == extended::dotdot<Path>::value)
            --depth;
        else
            ++depth;
    }
    return depth;
}

BOOST_FS_FUNC(uint32_t) 
get_similar_depth(
    const Path &target_path,
    const Path &other_path)
{
    uint32_t depth = 0;

    Path target_path_copy(normalize(target_path));
    Path other_path_copy(normalize(other_path));

    uint32_t target_depth = get_depth(target_path_copy);
    uint32_t other_depth = get_depth(other_path_copy);

    const uint32_t min_depth = std::min(target_depth, other_depth);

    path::iterator 
        target_iter = target_path.begin(),
        other_iter = other_path.begin();
    for (uint32_t i = 0; i < min_depth; ++i, ++depth, ++target_iter, ++other_iter) 
    {
        // std::cout << *target_iter << " == " << *other_iter << "? " << (*target_iter != *other_iter) << std::endl;
        if (*target_iter != *other_iter)
            return depth;
    }
    return depth;
}

#if 0
S: /usr/local/lib/test.a, T: /usr/lib/test.a, R: ../../lib/test.a
S: /usr/lib/test.a, T: /usr/local/lib/test.a, R: ../local/lib/test.a
S: /usr/local/lib/, T: /usr/local/lib, R: ./
S: /usr/local/lib, T: /usr/local/lib/, R: ./
S: /usr/local/lib, T: /usr/local/lib, R: ./
L: /usr/local/lib/, R: /usr/local/, RESULT: ./lib 
L: /usr/local/, R: /usr/local/lib/, RESULT: ../ 
L: /usr/local/lib/test.a, R: /usr/local/lib, RESULT: ./test.a
L: /usr/local/lib, R: /usr/local/lib/test.a, RESULT: ./
L: /usr/local/test.a, R: /usr/local/lib, RESULT: ../test.a
L: /usr/local/lib, R: /usr/local/test.a, RESULT: ./lib
L: /usr/local/lib/test.a, R: /usr/lib/test.a, RESULT: ../../local/lib/test.a
L: /usr/local/opt/lib/test.a, R: /usr/lib/test.a, RESULT: ../../local/lib/test.a
L: /usr/local/opt/lib/test.a, R: /usr/local/lib/test.a, RESULT: ../../opt/lib/test.a
L: /usr/lib/test.a, R: /usr/local/lib/test.a, RESULT: ../../../lib/test.a
L: /usr/lib/test.a, R: /usr/local/opt/lib/test.a, RESULT: ../../../../lib/test.a
L: /usr/lib/test.a, R: /usr/lib/local/opt/lib/test.a, RESULT: ../../../../test.a
L: /usr/lib/local/test.a, R: /usr/lib/local/opt/lib/test.a, RESULT: ../../../test.a
L: /usr/, R: /usr/local/lib/test.a, RESULT: ../../
L: /usr/local/lib/test.a, R: /usr/, RESULT: ./local/lib/test.a
#endif

BOOST_FS_FUNC(Path)
get_relative_path(
    const Path &from_path,
    const Path &to_path)
{
    Path result;

    Path from_path_copy(normalize(from_path));
    Path to_path_copy(normalize(to_path));

    const uint32_t similar_depth = get_similar_depth(from_path_copy, to_path_copy);
    uint32_t from_depth = get_depth(from_path_copy);
    uint32_t to_depth = get_depth(to_path_copy);

    bool is_target_max_path = (to_depth > from_depth) ? true : false;

    boost::filesystem::path::iterator
        from_iter = from_path.begin(),
        to_iter = to_path.begin(),
        end_iter;

    if (from_path == to_path)
    {
        result /= &boost::filesystem::extended::dot<Path>::value;
        return result;
    }
    else if (is_target_max_path)
    {
        Path append_path;
        const uint32_t num_dotdot = to_depth - similar_depth;
        for (uint32_t i=0; i < to_depth; ++i)
        {
            if (i < num_dotdot)
                result /= extended::dotdot<Path>::value;

            if ((i < from_depth) && (i > similar_depth))
                append_path /= *from_iter;

            std::cout << "i: " << i << '\n';
            std::cout << "\t=> num_dotdot: " << num_dotdot << '\n';
            std::cout << "\t=> from_depth: " << from_depth << '\n';
            std::cout << "\t=> rhs_depth: " << to_depth << '\n';
            std::cout << "\t=> similar_depth: " << similar_depth << '\n';
            std::cout << "\t=> result: " << result << '\n';
            std::cout << "\t=> append_path: " << append_path << '\n';
    
            if (i < from_depth) ++from_iter;
            if (i < to_depth) ++to_iter;
        }
        result /= append_path;
    }
    else
    {
        result /= &boost::filesystem::extended::dot<Path>::value;
        for (uint32_t i=0; i < from_depth; ++i)
        {
            if (i >= similar_depth)
                result /= *from_iter;

            if (i < from_depth) ++from_iter;
            if (i < to_depth) ++to_iter;
        }
    }
    return result;
}

///
/// Typedefs
///

typedef boost::filesystem::path Path;
typedef boost::filesystem::basic_filesystem_error<Path> FilesystemError;

///
/// @note See: <boost/filesystem/operations.hpp>
///

#undef BOOST_FS_FUNC

} // namespace extended
} // namespace filesystem
} // namespace boost

namespace rssd {
namespace filesystem = boost::filesystem;
} // namespace rssd

#endif // RSSD_FILESYSTEM_HELPERPATH_H
