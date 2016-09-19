#include <fstream>
#include <stdexcept>
#include "ProducterStream.hpp"

ProducterStream::ProducterStream()
{
    this->istream = NULL;
}

ProducterStream::~ProducterStream()
{
    if (this->istream != NULL && this->istream != &std::cin)
        ((std::ifstream *)this->istream)->close();
    if (this->istream != NULL)
        delete(this->istream);
}

bool ProducterStream::loadFile(char *path)
{
    if (this->istream != NULL && this->istream != &std::cin)
        ((std::ifstream *)this->istream)->close();
    std::ifstream *ifStream = new std::ifstream;
    ifStream->open(path);
    if (*ifStream)
    {
        this->istream = ifStream;
        return true;
    }
    delete(ifStream);
    return false;
}

bool ProducterStream::loadStdin()
{
    if (this->istream != NULL && this->istream != &std::cin)
        ((std::ifstream *)this->istream)->close();
    if (std::cin)
    {
        this->istream = &std::cin;
        return true;
    }
    return false;
}

std::string ProducterStream::nextString()
{
    if (this->istream == NULL || !(*this->istream))
        throw std::logic_error("Stream not opened");
    if (!*this->istream)
        throw std::logic_error("Empty stream");
    std::string line;
    std::getline(*this->istream, line);
    if (this->istream)
        line += "\n";
    return line;
}