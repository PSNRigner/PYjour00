//
// Created by frasse_l on 19/09/16.
//

#ifndef CONSUMERPARSER_HPP_
# define CONSUMERPARSER_HPP_

# include <map>
# include <vector>
# include "ProducterStream.hpp"

# define TRY_START   try{
# define TRY_END     } catch (std::exception ignored) { return false; }

class ConsumerParser
{
public:
    ConsumerParser(ProducterStream &);
    ~ConsumerParser();
    inline bool flush()
    {
        size_t n = this->buffers.size();
        this->bufferIndex[0] = 0;
        this->bufferIndex[1] = 0;
        this->buffers.clear();
        this->captures.clear();
        return n != 0;
    }

    inline bool peekChar(char c)
    {
        TRY_START
            this->checkBuffer();
        TRY_END
        return (*(this->buffers[this->bufferIndex[0]]))[this->bufferIndex[1]] == c;
    }

    inline bool readChar(char c)
    {
        if (!this->peekChar(c))
            return false;
        this->bufferIndex[1]++;
        return true;
    }

    inline bool readRange(char begin, char end)
    {
        TRY_START
            this->checkBuffer();
            char tmp = (*(this->buffers[this->bufferIndex[0]]))[this->bufferIndex[1]];
            if (tmp < begin || tmp > end)
                return false;
            this->bufferIndex[1]++;
        TRY_END
        return true;
    }

    bool readText(char *text);
    bool readEOF();
    bool readUntil(char c);
    bool readUntilEOF();
    bool readInteger();
    bool readIdentifier();

    inline bool beginCapture(std::string tag)
    {
        size_t *tab = new size_t[2];
        tab[0] = this->bufferIndex[0];
        tab[1] = this->bufferIndex[1];
        return this->captures.insert(std::pair<std::string, size_t *>(tag, tab)).second;
    }

    inline bool endCapture(std::string tag, std::string &out)
    {
        std::map<std::string, size_t *>::iterator it = this->captures.begin();
        std::map<std::string, size_t *>::iterator it_end = this->captures.end();
        while (it != it_end)
        {
            if (it->first == tag)
            {
                out.clear();
                for (size_t i = it->second[0]; i <= this->bufferIndex[0]; i++)
                    if (this->bufferIndex[0] == i)
                        for (size_t j = this->bufferIndex[0] == it->second[0] ? it->second[1] : 0; j < this->bufferIndex[1]; j++)
                            out += this->buffers[i]->at(j);
                    else
                        for (size_t j = i == it->second[0] ? it->second[1] : 0; j < this->buffers[i]->length(); j++)
                            out += this->buffers[i]->at(j);

                return true;
            }
            it++;
        }
        return false;
    }

private:
    void checkBuffer();

    size_t bufferIndex[2];
    std::vector<std::string *> buffers;
    std::map<std::string, size_t *> captures;
    ProducterStream &producterStream;
};


#endif //CONSUMERPARSER_HPP_
