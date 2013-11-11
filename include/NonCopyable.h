#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

/**
 * @class NonCopyable
 *
 * @brief Запрет копирования объектов
 * @details Базовый класс для запрета копирования объектов потомков.
 *
 */

class NonCopyable
{
protected:
  NonCopyable() {}
  ~NonCopyable() {}

private:
  /// Конструктор копирования, объявленный в private части класса
  NonCopyable(const NonCopyable&) = delete;
  /// Оператор присваивания, объявленный в private части класса
  const NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif // NONCOPYABLE_H
