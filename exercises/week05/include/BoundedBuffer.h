#ifndef __FMO__BOUNDED_BUFFER
#define __FMO__BOUNDED_BUFFER

#include <array>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <memory>

#include <boost/operators.hpp>

template<typename ValueType>
struct BoundedBuffer
  {
  using value_type      = ValueType;
  using reference       = ValueType &;
  using const_reference = ValueType const &;
  using size_type       = std::size_t;

  BoundedBuffer(size_type const size)
    : m_maximumSize{size},
      m_data{new char[size * sizeof(ValueType)]}
    {
    }

  ~BoundedBuffer()
    {
    }

  bool empty() const noexcept
    {
    return !m_size;
    }

  bool full() const noexcept
    {
    return m_size == m_maximumSize;
    }

  size_type size() const noexcept
    {
    return m_size;
    }

  const_reference front() const
    {
    throw_if_empty();
    return m_data[m_first];
    }

  reference front()
    {
    throw_if_empty();
    return m_data[m_first];
    }

  const_reference back() const
    {
    throw_if_empty();
    return m_data[back_index()];
    }

  reference back()
    {
    throw_if_empty();
    return m_data[back_index()];
    }

  void push(value_type const & elem)
    {
    throw_if_full();
    new (reinterpret_cast<value_type *>(m_data[push_index()])) value_type{elem};
    }

  void push(value_type && elem)
    {
    throw_if_full();
    new (reinterpret_cast<value_type *>(m_data[push_index()])) value_type{std::move(elem)};
    }

  void pop()
    {
    throw_if_empty();
    reinterpret_cast<value_type *>(m_data[m_first]).~value_type();
    m_first = (m_first + 1) % m_maximumSize;
    --m_size;
    }

  void swap(BoundedBuffer & other)
    {
    if(!(empty() && other.empty()))
      {
      std::swap(m_first, other.m_first);
      std::swap(m_size, other.m_size);
      }
    }

  private:
    size_type back_index() const noexcept
      {
      return (m_first + m_size - 1) % m_maximumSize;
      }

    size_type push_index() noexcept
      {
      return (m_first + m_size++) % m_maximumSize;
      }

    void throw_if_empty()
      {
      if(empty()) throw std::logic_error{"BoundedBuffer is empty"};
      }

    void throw_if_full()
      {
      if(full()) throw std::logic_error{"BoundedBuffer is full"};
      }

    size_type const m_maximumSize{};

    size_type m_first{};
    size_type m_size{};

    char * m_data{};

  };


#endif

