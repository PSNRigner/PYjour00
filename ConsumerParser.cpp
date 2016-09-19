//
// Created by frasse_l on 19/09/16.
//

#include <cstring>
#include "ConsumerParser.hpp"

ConsumerParser::ConsumerParser(ProducterStream &producterStream) : producterStream(producterStream)
{
    this->bufferIndex[0] = 0;
    this->bufferIndex[1] = 0;
}

ConsumerParser::~ConsumerParser()
{
    this->flush();
}

void ConsumerParser::checkBuffer()
{
    if (this->bufferIndex[0] < this->buffers.size())
    {
        if (this->bufferIndex[1] >= this->buffers[this->bufferIndex[0]]->length())
        {
            this->bufferIndex[1] -= this->buffers[this->bufferIndex[0]]->length();
            this->bufferIndex[0]++;
            this->checkBuffer();
        }
        return ;
    }

    this->buffers.push_back(new std::string(this->producterStream.nextString()));
    this->checkBuffer();
}

bool ConsumerParser::readText(char *text)
{
    TRY_START
        size_t size = strlen(text);
        size_t savedIndexes[2];
        savedIndexes[0] = this->bufferIndex[0];
        savedIndexes[1] = this->bufferIndex[1];
        for (int i = 0; i < size; ++i)
            if (!this->readChar(text[i]))
            {
                this->bufferIndex[0] = savedIndexes[0];
                this->bufferIndex[1] = savedIndexes[1];
                return false;
            }
    TRY_END
    return true;
}

bool ConsumerParser::readEOF()
{
    try
    {
        this->checkBuffer();
        return false;
    }
    catch (std::exception ignored)
    {
        return true;
    }
}

bool ConsumerParser::readUntil(char c)
{
    TRY_START
        while (!this->readChar(c))
            this->bufferIndex[1]++;
        return true;
    TRY_END;
}

bool ConsumerParser::readUntilEOF()
{
    while (!this->readEOF())
        this->bufferIndex[1]++;
    return true;
}

bool ConsumerParser::readInteger()
{
    int n = 0;
    TRY_START
        while (this->readRange('0', '9'))
            n++;
    TRY_END
    return n != 0;
}

bool ConsumerParser::readIdentifier()
{
    if (!this->readRange('a', 'z') && !this->readRange('A', 'Z') && !this->readChar('_'))
        return false;
    while (this->readRange('a', 'z') || this->readRange('A', 'Z') || this->readRange('0', '9') || this->readChar('_'));
    return true;
}