#ifndef NMTYPES_H
#define NMTYPES_H

#include <vector>
#include <algorithm>
#include <queue>

namespace netman
{

class ByteArray;
typedef std::queue<ByteArray> ByteArrayQueue;
typedef std::vector<ByteArray> ByteArrayUnion;

/**
 * @brief Массив байт. Позволяет работать с массивом как со строкой
 *        и вектором.
 */
class ByteArray
{
  typedef std::vector<char> DataType;
  typedef std::vector<char>::iterator Iterator;
  typedef std::vector<char>::const_iterator ConstIterator;

  std::vector<char> data_;

public:
  ByteArray();
  explicit ByteArray(const char* data, size_t data_size);
  ByteArray(const DataType& data);
  ByteArray(ConstIterator& first
            , ConstIterator& last);

  ByteArray(const ByteArray& data);
  ByteArray(const std::string& data);

  ByteArray& operator =(const ByteArray& data);
  //  ByteArray& operator =(const char * str);
  ByteArray& operator =(const std::string& str);
  ByteArray& operator =(const DataType& data);

  ByteArray operator +(const ByteArray& data) const;
  //ByteArray operator +(const char * str) const;
  ByteArray operator +(const std::string& data) const;
  ByteArray operator +(const DataType& data) const;

  ByteArray& operator <<(const ByteArray& data);
  //ByteArray& operator <<(const char * str);
  ByteArray& operator <<(const std::string data);

  char& operator [](unsigned int index);
  const char& operator [](unsigned int index) const;

  void pushBack(const char byte);

  void append(const ByteArray& data);

  void erase(const Iterator& first
             , const Iterator& last);
  void erase(size_t pos, size_t n);

  void clear();

  ByteArray subtract(const Iterator& first
                     , const Iterator& last) const;
  ByteArray subtract(size_t pos, size_t n) const;

  size_t size() const;

  const char* data() const;
  std::string toString() const;
protected:
  DataType fromCharArray(const char* array, size_t size) const;
  DataType fromStdString(const std::string& str) const;
  std::string toStdString(const DataType& data) const;

  DataType& appendDataType(DataType& in, const DataType& data) const;
};


}

#endif // NMTYPES_H
