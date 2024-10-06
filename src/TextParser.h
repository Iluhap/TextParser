#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <set>
#include <mutex>


class TextParser
{
public:
    TextParser();

public:
    void Parse(const std::filesystem::path& file_path);
    std::size_t GetWordsCount() const;

private:
    void Load(std::basic_istream<char>* buffer, const std::size_t& buffer_size);
    void ExecuteParse();
    void ParseWorker(const std::string& text);
    void Add(const std::string& word);

private:
    static bool IsDelimiter(const char& symbol);

private:
    std::vector<std::string> text_pool_;
    std::set<std::string> unique_words_;

    std::mutex unique_words_write_mutex_;
};
