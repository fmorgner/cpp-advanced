#ifndef __FMO__BOUNDED_BUFFER
#define __FMO__BOUNDED_BUFFER

#include <array>
#include <stdexcept>
#include <type_traits>
#include <utility>

template<typename ValueType, std::size_t Size>
struct BoundedBuffer
  {
  using container_type = typename std::array<ValueType, Size>;

  using value_type = typename container_type::value_type;
  using reference = typename container_type::reference;
  using const_reference = typename container_type::const_reference;
  using size_type = typename container_type::size_type;

  BoundedBuffer() noexcept(std::is_nothrow_constructible<value_type>::value)
    {

    }

  BoundedBuffer(BoundedBuffer const & other) noexcept(std::is_nothrow_copy_constructible<value_type>::value)
    : m_first{other.m_first},
      m_size{other.m_size},
      m_data{other.m_data}
    {

    }

  BoundedBuffer(BoundedBuffer && other) noexcept(std::is_nothrow_move_constructible<value_type>::value)
    : m_first{std::move(other.m_first)},
      m_size{std::move(other.m_size)},
      m_data{std::move(other.m_data)}
    {

    }

  BoundedBuffer & operator=(BoundedBuffer const & other) noexcept(std::is_nothrow_copy_assignable<value_type>::value)
    {
    m_first = other.m_first;
    m_size = other.m_size;
    m_data = other.m_data;

    return *this;
    }

  BoundedBuffer &  operator=(BoundedBuffer && other) noexcept(std::is_nothrow_move_assignable<value_type>::value)
    {
    m_first = std::move(other.m_first);
    m_size = std::move(other.m_size);
    m_data = std::move(other.m_data);

    return *this;
    }

  bool empty() const noexcept
    {
    return !m_size;
    }

  bool full() const noexcept
    {
    return m_size == Size;
    }

  size_type size() const noexcept
    {
    return m_size;
    }

  const_reference front() const
    {
    return empty() ? throw std::logic_error{"empty BoundedBuffer"} : m_data.at(m_first);
    }

  reference front()
    {
    return empty() ? throw std::logic_error{"empty BoundedBuffer"} : m_data.at(m_first);
    }

  const_reference back() const
    {
    return empty() ? throw std::logic_error{"empty BoundedBuffer"} : m_data.at(back_index());
    }

  reference back()
    {
    return empty() ? throw std::logic_error{"empty BoundedBuffer"} : m_data.at(back_index());
    }

  void push(value_type const & elem)
    {
    full() ? throw std::logic_error{"full BoundedBuffer"} : m_data.at(push_index()) = elem;
    }

  void push(value_type && elem)
    {
    full() ? throw std::logic_error{"full BoundedBuffer"} : m_data.at(push_index()) = std::move(elem);
    }

  void pop()
    {
    if(empty())
      {
      throw std::logic_error{"empty BoundedBuffer"};
      }

    m_first = (m_first + 1) % Size;
    --m_size;
    }

  void swap(BoundedBuffer & other) noexcept(noexcept(std::declval<container_type &>().swap(std::declval<container_type &>())))
    {
    if(!(empty() && other.empty()))
      {
      std::swap(m_first, other.m_first);
      std::swap(m_size, other.m_size);

      m_data.swap(other.m_data);
      }
    }

  private:
    size_type back_index() const noexcept
      {
      return (m_first + m_size - 1) % Size;
      }

    size_type push_index() noexcept
      {
      return (m_first + m_size++) % Size;
      }

    size_type m_first{};
    size_type m_size{};

    container_type m_data{};
  };

template<typename ValueType>
struct BoundedBuffer<ValueType, 0>
  {
  using container_type = typename std::array<ValueType, 0>;

  using value_type = typename container_type::value_type;
  using reference = typename container_type::reference;
  using const_reference = typename container_type::const_reference;
  using size_type = typename container_type::size_type;

  bool empty() const noexcept { return true; }

  bool full() const noexcept { return true; }

  size_type size() const noexcept { return 0; }

  [[noreturn]] const_reference front() const { throw std::logic_error{"Zero sized BoundedBuffer"}; }

  [[noreturn]] reference front() { throw std::logic_error{"Zero sized BoundedBuffer"}; }

  [[noreturn]] const_reference back() const { throw std::logic_error{"Zero sized BoundedBuffer"}; }

  [[noreturn]] reference back() { throw std::logic_error{"Zero sized BoundedBuffer"}; }

  [[noreturn]] void push(value_type const &) { throw std::logic_error{"Zero sized BoundedBuffer"}; }

  [[noreturn]] void push(value_type &&) { throw std::logic_error{"Zero sized BoundedBuffer"}; }

  [[noreturn]] void pop() { throw std::logic_error{"Zero sized BoundedBuffer"}; }

  void swap(BoundedBuffer &) noexcept { }
  };

#endif

