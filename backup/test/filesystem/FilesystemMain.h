#ifndef RSSD_FILESYSTEM_MAIN_H
#define RSSD_FILESYSTEM_MAIN_H

#include "Core"

using namespace RSSD;
using namespace RSSD::filesystem;

namespace RSSD {
namespace Filesystem {

void printFilesystemError(const extended::FilesystemError &error)
{
    std::cout << "\t=> " << error.what() << '\n' << std::endl;
}

void printFileDirectoryAttributes(const string_t &value)
{
    try
    {
        path target_path(value);

        std::cout << "=== File/Directory Attributes: " << value << " ===\n\n";
        std::cout << "\t => normalize(): " << target_path << '\n';
        std::cout << "\t => directory_string(): " << target_path.directory_string() << '\n';
        std::cout << "\t => root_name(): " << target_path.root_name() << '\n';
        std::cout << "\t => root_directory(): " << target_path.root_directory() << '\n';
        std::cout << "\t => root_path(): " << target_path.root_path() << '\n';
        std::cout << "\t => relative_path(): " << target_path.relative_path() << '\n';
        std::cout << "\t => parent_path(): " << target_path.parent_path() << '\n';
        std::cout << "\t => filename(): " << target_path.filename() << '\n';
        std::cout << "\t => string(): " << target_path.string() << '\n';
        std::cout << "\t => is_directory(): " << is_directory(target_path) << '\n';
        std::cout << "\t => is_regular_file(): " << is_regular_file(target_path) << '\n';
        // std::cout << "\t =>remove_filename(): " << target_path.remove_filename() << '\n';
        std::cout << "\t => get_directory(): " << extended::get_directory(target_path) << std::endl;
        std::cout << "\t => get_depth(): " << extended::get_depth(target_path) << std::endl;
        std::cout << std::endl;
    }
    catch (const extended::FilesystemError &error)
    {
        printFilesystemError(error);
    }
}

void printFileDirectoryAttributes(
    const string_t &relative,
    const string_t &base)
{
    try
    {
        path combined_path(complete(relative, base));
        printFileDirectoryAttributes(combined_path.string());
    }
    catch (const extended::FilesystemError &error)
    {
        printFilesystemError(error);
    }
}

void printSymlinkAttributes(const string_t &value)
{
    std::cout << "=== Symlink Attributes: " << value << " ===\n\n";

    try
    {
        path target_path(value);

        std::cout << "\t=> normalize(): " << extended::normalize(target_path) << '\n';
        std::cout << "\t=> is_symlink(): " << is_symlink(target_path) << '\n';
        std::cout << "\t=> is_directory(): " << is_directory(target_path) << '\n';
        std::cout << "\t=> is_regular_file(): " << is_regular_file(target_path) << '\n';
        std::cout << "\t=> is_other(): " << is_other(target_path) << '\n';
        std::cout << std::endl;
    }
    catch (const extended::FilesystemError &error)
    {
        printFilesystemError(error);
    }
}

int main(int argc, char **argv)
{
    path filepath1("/Users/arris/Code/nous/dep/EigenDense");
    path filepath2("/Users/arris/Code/nous/");

    printFileDirectoryAttributes(filepath1.string());
    printFileDirectoryAttributes(filepath2.string());

    std::cout << "get_similar_depth('" << filepath1 << "', '" << filepath2 << "'): "
        << extended::get_similar_depth(filepath1, filepath2)
        << std::endl << std::endl;

    std::cout << "get_similar_depth('" << filepath2 << "', '" << filepath1 << "'): "
        << extended::get_similar_depth(filepath1, filepath2)
        << std::endl << std::endl;

    std::cout << "get_relative_path('" << filepath1 << "', '" << filepath2 << "'): "
        << extended::get_relative_path(filepath2, filepath1)
        << std::endl << std::endl;

    std::cout << "get_relative_path('" << filepath2 << "', '" << filepath1 << "'): "
        << extended::get_relative_path(filepath1, filepath2)
        << std::endl << std::endl;

    return 0;
}

} /// namespace Filesystem
} /// namespace RSSD

#endif /// RSSD_FILESYSTEM_MAIN_H
