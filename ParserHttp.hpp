//
// Created by frasse_l on 19/09/16.
//

#include "ConsumerParser.hpp"

#ifndef PARSERHTTP_HPP_
# define PARSERHTTP_HPP_


class ParserHttp : public ConsumerParser
{
public:
    ParserHttp(ProducterStream &);
    virtual ~ParserHttp();

    bool readHttp(std::map<std::string, std::string> &);

    bool readUri();

    bool readHeaderName();
};


#endif /* !PARSERHTTP_HPP */
