#ifndef __FMO__BOUNDED_BUFFER
#define __FMO__BOUNDED_BUFFER

#include <boost/operators.hpp>

#include <stdexcept>
#include <type_traits>
#include <utility>

template<typename ValueType>
struct BoundedBuffer
  {

  template<typename BufferType>
  struct buffer_iterator : boost::random_access_iterator_helper<buffer_iterator<BufferType>, typename BufferType::value_type>
    {
    using __base = boost::random_access_iterator_helper<buffer_iterator<BufferType>, typename BufferType::value_type>;

    using typename __base::value_type;
    using typename __base::pointer;
    using reference = std::conditional_t<std::is_const<BufferType>::value, typename BufferType::const_reference,
                                                                           typename BufferType::reference>;
    using typename __base::iterator_category;
    using typename __base::difference_type;

    buffer_iterator(BufferType & buffer, typename BufferType::size_type const index)
      : m_buffer{buffer},
        m_index{index}
      {

      }

    buffer_iterator(buffer_iterator<std::remove_const_t<BufferType>> const & other)
      : m_buffer{other.m_buffer},
        m_index{other.m_index}
      {

      }

    decltype(auto) operator=(buffer_iterator<std::remove_const_t<BufferType>> const & other)
      {
      throw_on_different_buffer(other);

      m_index = other.m_index;
      }

    auto operator==(buffer_iterator const & other) const
      {
      throw_on_different_buffer(other);

      return m_index == other.m_index;
      }

    auto operator<(buffer_iterator const & other) const
      {
      throw_on_different_buffer(other);
      return m_index < other.m_index;
      }

    decltype(auto) operator++()
      {
      throw_on_overflow();
      ++m_index;

      return *this;
      }

    decltype(auto) operator--()
      {
      throw_on_underrun();
      --m_index;

      return *this;
      }

    decltype(auto) operator+=(difference_type const offset)
      {
      throw_on_overflow(offset);
      m_index += offset;

      return *this;
      }

    decltype(auto) operator-=(difference_type const offset)
      {
      throw_on_underrun(offset);
      m_index -= offset;

      return *this;
      }

    decltype(auto) operator-(buffer_iterator const & other) const
      {
      throw_on_different_buffer(other);

      return m_index > other.m_index ? m_index - other.m_index : other.m_index - m_index;
      }

    decltype(auto) operator*() const
      {
      throw_on_invalid_position();

      return m_buffer.get(m_buffer.to_buffer_index(m_index));
      }

    private:
      BufferType & m_buffer;
      std::size_t m_index;

      auto throw_on_underrun(typename BufferType::size_type const offset = 1) const
        {
        if(m_index == 0 || offset > m_index)
          {
          throw std::out_of_range{"Iterator out of range - underrun"};
          }
        }

      auto throw_on_overflow(typename BufferType::size_type const offset = 1) const
        {
        if(m_index + offset > m_buffer.size())
          {
          throw std::out_of_range{"Iterator out of range - overflow"};
          }
        }

      auto throw_on_invalid_position() const
        {
        if(m_index >= m_buffer.size())
          {
          throw std::out_of_range{"Invalid iterator access - out of bounds"};
          }
        }

      auto throw_on_different_buffer(buffer_iterator const & other) const
        {
        if(&m_buffer != &other.m_buffer)
          {
          throw std::logic_error{"Iterators in binary expression are created from different buffers!"};
          }
        }

      friend struct buffer_iterator<BufferType const>;

    };

  using value_type      = ValueType;
  using reference       = value_type &;
  using const_reference = value_type const &;
  using pointer         = value_type *;
  using const_pointer   = value_type const *;
  using size_type       = std::size_t;
  using iterator        = buffer_iterator<BoundedBuffer>;
  using const_iterator  = buffer_iterator<BoundedBuffer const>;

  BoundedBuffer(size_type const size)
    : m_maximumSize{size ? size : throw std::invalid_argument{"Tried to allocate BoundedBuffer of size 0"}},
      m_data{new char[size * sizeof(ValueType)]}
    {

    }

  BoundedBuffer(BoundedBuffer const & other)
    : BoundedBuffer{other.m_maximumSize}
    {
    copy(other);
    }

  BoundedBuffer(BoundedBuffer && other)
    {
    swap(other);
    }

  ~BoundedBuffer()
    {
    clear();

    delete[](m_data);
    }

  auto empty() const noexcept
    {
    return !m_size;
    }

  auto full() const noexcept
    {
    return m_size == m_maximumSize;
    }

  auto size() const noexcept
    {
    return m_size;
    }

  decltype(auto) front() const
    {
    throw_if_empty();
    return get(m_first);
    }

  decltype(auto) front()
    {
    throw_if_empty();
    return get(m_first);
    }

  decltype(auto) back() const
    {
    throw_if_empty();
    return get(back_index());
    }

  decltype(auto) back()
    {
    throw_if_empty();
    return get(back_index());
    }

  auto push(value_type const & elem)
    {
    throw_if_full();

    ::new (ptr() + push_index()) value_type{elem};
    }

  auto push(value_type && elem)
    {
    throw_if_full();

    ::new (ptr() + push_index()) value_type{std::move(elem)};
    }

  auto pop()
    {
    throw_if_empty();

    ptr()[m_first].~value_type();

    m_first = (m_first + 1) % m_maximumSize;
    --m_size;
    }

  auto swap(BoundedBuffer & other) noexcept
    {
    std::swap(m_maximumSize, other.m_maximumSize);
    std::swap(m_first, other.m_first);
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
    }

  decltype(auto) operator=(BoundedBuffer const & other)
    {
    clear();

    if(m_maximumSize != other.m_maximumSize)
      {
      delete[](m_data);
      m_maximumSize = other.m_maximumSize;
      m_data = new char[m_maximumSize * sizeof(value_type)];
      }

    copy(other);
    return *this;
    }

  decltype(auto) operator=(BoundedBuffer && other)
    {
    swap(other);
    return *this;
    }

  auto begin()
    {
    return iterator{*this, 0};
    }

  auto end()
    {
    return iterator{*this, m_size};
    }

  auto begin() const
    {
    return const_iterator{*this, 0};
    }

  auto end() const
    {
    return const_iterator{*this, m_size};
    }

  auto cbegin() const
    {
    return begin();
    }

  auto cend() const
    {
    return end();
    }

  private:
    auto back_index() const noexcept
      {
      return to_buffer_index(m_size - 1);
      }

    auto push_index() noexcept
      {
      return to_buffer_index(m_size++);
      }

    auto to_buffer_index(size_type const index) const noexcept
      {
      return (m_first + index) % m_maximumSize;
      }

    auto throw_if_empty() const
      {
      if(empty()) throw std::logic_error{"BoundedBuffer is empty"};
      }

    auto throw_if_full() const
      {
      if(full()) throw std::logic_error{"BoundedBuffer is full"};
      }

    auto ptr()
      {
      return reinterpret_cast<pointer>(m_data);
      }

    auto ptr() const
      {
      return reinterpret_cast<const_pointer>(m_data);
      }

    decltype(auto) get(size_type const idx)
      {
      return ptr()[idx];
      }

    decltype(auto) get(size_type const idx) const
      {
      return ptr()[idx];
      }

    auto copy(BoundedBuffer const & other)
      {
      for(auto const & element : other)
        {
        push(element);
        }
      }

    auto clear()
      {
      for(size_type idx = 0; idx < m_size; ++idx)
        {
        pop();
        }

      m_size = 0;
      m_first = 0;
      }

    size_type m_maximumSize{};
    size_type m_first{};
    size_type m_size{};
    char * m_data{};
  };

#endif

