//
// Created by frasse_l on 19/09/16.
//

#ifndef PRODUCTERSTREAM_HPP_
# define PRODUCTERSTREAM_HPP_

# include <iostream>
# include <cstring>
# include <mutex>

class ProducterStream
{
public:
    ProducterStream();
    ~ProducterStream();

    std::string nextString();
    bool loadFile(char *path);
    bool loadStdin();

private:
    std::istream *istream;
    std::mutex mutex;
};

#endif /* PRODUCTERSTREAM_HPP_ */