#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>

using namespace std;

/// Purloined from https://stackoverflow.com/questions/34218040/how-to-read-a-csv-file-data-into-an-array

/**
 * @brief 
 * 
 * @tparam T 
 */
template <class T>
class csv_istream_iterator : public iterator<input_iterator_tag, T>
{
    istream *_input;
    char _delim;
    string _value;

public:
    csv_istream_iterator(char delim = ',') : _input(0), _delim(delim) {}
    csv_istream_iterator(istream &in, char delim = ',') : _input(&in), _delim(delim) { ++*this; }

    const T operator*() const
    {
        istringstream ss(_value);
        T value;
        ss >> value;
        return value;
    }

    istream &operator++()
    {
        if (!(getline(*_input, _value, _delim)))
        {
            _input = 0;
        }
        return *_input;
    }

    bool operator!=(const csv_istream_iterator &rhs) const
    {
        return _input != rhs._input;
    }
};

template <>
const string csv_istream_iterator<string>::operator*() const
{
    return _value;
}

