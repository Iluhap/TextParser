#include "TextParser.h"

#include <iostream>
#include <thread>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <ranges>
#include <algorithm>
#include <future>


TextParser::TextParser()
{
    const std::size_t cpu_count = std::thread::hardware_concurrency();

    text_pool_ = std::vector<std::string> {cpu_count};
    unique_words_ = std::set<std::string> {};
}

void TextParser::Parse(const std::filesystem::path& file_path)
{
    if (not std::filesystem::exists(file_path))
    {
        return;
    }

    const std::size_t file_size = std::filesystem::file_size(file_path);
    std::ifstream input_file {file_path, std::ios::in | std::ios::binary};

    if (input_file.is_open())
    {
        Load(&input_file, file_size);
    }
    input_file.close();

    ExecuteParse();
}

std::size_t TextParser::GetWordsCount() const
{
    return unique_words_.size();
}

void TextParser::Load(std::basic_istream<char>* buffer, const std::size_t& buffer_size)
{
    if (not buffer)
    {
        return;
    }

    const uint32_t chunk_size = std::ceil(buffer_size / text_pool_.size());

    for (auto& text_block : text_pool_)
    {
        text_block.resize(chunk_size);
        buffer->read(text_block.data(), chunk_size);

        char symbol;
        while (not IsDelimiter(text_block.back()) and buffer->get(symbol))
        {
            text_block.push_back(symbol);
        }
    }
}

void TextParser::ExecuteParse()
{
    unique_words_.clear();

    std::vector<std::future<std::set<std::string>>> futures;

    for (const auto& text : text_pool_)
    {
        auto future = std::async(std::launch::async, &TextParser::ParseWorker, text);
        futures.push_back(std::move(future));
    }

    for (auto& future : futures)
    {
        auto words = future.get();
        unique_words_.insert(words.begin(), words.end());
    }
}

std::set<std::string> TextParser::ParseWorker(const std::string& text)
{
    std::set<std::string> unique_words;

    std::string buffer;

    for (const auto& symbol : text)
    {
        if (IsDelimiter(symbol))
        {
            if (not buffer.empty())
            {
                unique_words.insert(buffer);
                buffer.clear();
            }
        }
        else
        {
            buffer += symbol;
        }
    }

    if (not buffer.empty())
    {
        unique_words.insert(buffer);
    }

    return unique_words;
}

bool TextParser::IsDelimiter(const char& symbol)
{
    static const std::vector<char> delimiters = {' ', '\n', '\t', '\0'};

    return std::ranges::any_of(delimiters.begin(), delimiters.end(),
                               [&symbol](const auto& delimiter)
                               {
                                   return symbol == delimiter;
                               });
}
