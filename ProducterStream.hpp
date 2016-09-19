#include <iostream>
#include <cstring>

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
};