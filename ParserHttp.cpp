//
// Created by frasse_l on 19/09/16.
//

#include "ParserHttp.hpp"

ParserHttp::ParserHttp(ProducterStream &producterStream) : ConsumerParser(producterStream)
{
}

ParserHttp::~ParserHttp()
{
}

bool ParserHttp::readHttp(std::map<std::string, std::string> &map)
{
    TRY_START

        if (!this->readText((char *) "GET") && !this->readText((char *) "POST"))
            return false;
        if (!this->readChar(' ') || !this->readUri() || !this->readChar(' ') || !this->readText((char *) "http/1.0") || !this->readChar('\n'))
            return false;
        while (!this->peekChar('\n'))
        {
            std::string header;
            std::string data;
            if (!this->beginCapture("HEADER") || !this->readHeaderName() || !this->endCapture("HEADER", header) || !this->readChar(':')
                || !this->beginCapture("DATA") || !this->readUntil('\n') || !this->endCapture("DATA", data))
                return false;
            map.insert(std::pair<std::string, std::string>(header, data.substr(0, data.length() - 1)));
        }
        this->flush();
        this->readUntilEOF();
        this->flush();

    TRY_END
    return false;
}

bool ParserHttp::readUri()
{
    int n = 0;
    TRY_START
        while (this->readChar('/') || this->readChar('.') || this->readChar('?') || this->readChar('%') || this->readChar('&') || this->readChar('=') || this->readChar('+')
                || this->readChar(':') || this->readChar('-') || this->readRange('a', 'z') || this->readRange('A', 'Z') || this->readRange('0', '9'))
            ++n;
    TRY_END
    return n != 0;
}

bool ParserHttp::readHeaderName()
{
    int n = 0;
    TRY_START
        while (this->readChar('-') || this->readRange('a', 'z') || this->readRange('A', 'Z'))
            ++n;
    TRY_END
    return n != 0;
}