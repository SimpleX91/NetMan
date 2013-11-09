#include "NetworkManager/ByteArray.h"
#include "cstring"

#include <string>

namespace netman
{

ByteArray::ByteArray()
{
}

ByteArray::ByteArray(const char* data, size_t data_size)
{
  this->data_ = fromCharArray(data, data_size);
}

ByteArray::ByteArray(const DataType& data)
  : data_(data)
{
}

ByteArray::ByteArray(ConstIterator& first
                     , ConstIterator& last)
  : data_(first, last)
{
}

ByteArray::ByteArray(const ByteArray& data)
{
  this->data_ = data.data_;
}

ByteArray::ByteArray(const std::string& data)
{
  this->data_ = fromStdString(data);
}

ByteArray& ByteArray::operator =(const ByteArray& data)
{
  this->data_ = data.data_;
  return *this;
}

//ByteArray& ByteArray::operator =(const char* str)
//{
//  size_t size = strlen(str);
//  data_ = fromCharArray(str, size);

//  return *this;
//}

ByteArray& ByteArray::operator =(const std::string& str)
{
  data_ = DataType(str.begin(), str.end());
  return *this;
}

ByteArray& ByteArray::operator =(const DataType& data)
{
  this->data_ = data;
  return *this;
}

ByteArray ByteArray::operator +(const ByteArray& data) const
{
  ByteArray buf(this->data_);
  buf.append(data);
  return buf;
}

//ByteArray ByteArray::operator +(const char* str) const
//{
//  DataType buf = this->data_;
//  buf = appendDataType(buf, fromStdString(str));
//  return ByteArray(buf);
//}

ByteArray ByteArray::operator +(const std::string& data) const
{
  DataType buf = this->data_;
  buf = appendDataType(buf, fromStdString(data));
  return ByteArray(buf);
}

ByteArray ByteArray::operator +(const DataType& data) const
{
  DataType buf = this->data_;
  buf = appendDataType(buf, data);
  return ByteArray(buf);
}

ByteArray& ByteArray::operator <<(const ByteArray& data)
{
  this->append(data);
  return *this;
}

//ByteArray& ByteArray::operator <<(const char* str)
//{
//  appendDataType(this->data_, fromStdString(str));
//  return *this;
//}

ByteArray& ByteArray::operator <<(const std::string data)
{
  appendDataType(this->data_, fromStdString(data));
  return *this;
}

char& ByteArray::operator [](unsigned int index)
{
  return this->data_[index];
}

const char& ByteArray::operator [](unsigned int index) const
{
  return this->data_[index];
}

void ByteArray::pushBack(const char byte)
{
  this->data_.push_back(byte);
}

void ByteArray::append(const ByteArray& data)
{
  size_t new_size = data.data_.size() + this->data_.size();

  // Резервируем необходимый объем памяти под объединенный массив
  if (this->data_.capacity() < new_size)
    this->data_.reserve(new_size);

  this->data_.insert(data_.end(), data.data_.begin(), data.data_.end());
}

void ByteArray::erase(const ByteArray::Iterator& first
                      , const ByteArray::Iterator& last)
{
  data_.erase(first, last);
}

void ByteArray::erase(size_t pos, size_t n)
{
  data_.erase(data_.begin() + pos, data_.begin() + pos + n);
}

void ByteArray::clear()
{
  data_.clear();
}

ByteArray ByteArray::subtract(const ByteArray::Iterator& first
                              , const ByteArray::Iterator& last) const
{
  DataType buf = DataType(first, last);
  return ByteArray(buf);
}

ByteArray ByteArray::subtract(size_t pos, size_t n) const
{
  ConstIterator first = data_.begin() + pos;
  ConstIterator last = data_.begin() + pos + n;

  return ByteArray(first, last);
}

size_t ByteArray::size() const
{
  return data_.size();
}

const char* ByteArray::data() const
{
  return data_.data();
}

std::string ByteArray::toString() const
{
  return toStdString(data_);
}

ByteArray::DataType ByteArray::fromCharArray(const char* array,
                                             size_t size) const
{
  DataType buf;
  buf.reserve(size);
  std::copy_n(array, size, std::back_inserter(buf));
  return buf;
}

ByteArray::DataType ByteArray::fromStdString(const std::string& str) const
{
  DataType buf;
  buf = DataType(str.begin(), str.end());
  buf.shrink_to_fit();
  str.size();
  return buf;
}

std::string ByteArray::toStdString(const ByteArray::DataType& data) const
{
  return std::string(data.begin(), data.end());
}

ByteArray::DataType& ByteArray::appendDataType(ByteArray::DataType& in
                                               , const ByteArray::DataType& data
                                              ) const
{
  in.reserve(in.size() + data.size());
  std::copy(data.begin(), data.end(), std::back_inserter(in));
  return in;
}

}
