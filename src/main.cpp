#include <iostream>
#include <filesystem>

#include <boost/program_options.hpp>

#include "TextParser.h"


namespace po = boost::program_options;

int main(int argc, char** argv)
{
    std::filesystem::path file_path {};

    po::options_description desc("Allowed options");
    try
    {
        desc.add_options()
            ("help", "produce help message")

            ("file",
             po::value<std::filesystem::path>(&file_path)->required(),
             "path to file to be parsed");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 1;
        }

        po::notify(vm);
    }
    catch (std::exception& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        std::cout << desc << std::endl;
        return 1;
    }

    auto parser = std::make_unique<TextParser>();

    if (parser)
    {
        parser->Parse(file_path);
        std::cout << "File " << file_path.filename()
                  << " contains: " << parser->GetWordsCount() << " words" << std::endl;
    }

    return 0;
}