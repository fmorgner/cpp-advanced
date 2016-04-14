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

  template<typename BufferType>
  struct buffer_iterator : boost::random_access_iterator_helper<buffer_iterator<BufferType>, typename BufferType::value_type>
    {
    using __base = boost::random_access_iterator_helper<buffer_iterator<BufferType>, typename BufferType::value_type>;

    using typename __base::value_type;
    using typename __base::pointer;
    using typename __base::reference;
    using typename __base::iterator_category;
    using typename __base::difference_type;

    buffer_iterator(BufferType * const associated, typename BufferType::size_type const index)
      : m_buffer{associated ? associated : throw std::invalid_argument{"Cannot create iterator from nullptr"}},
        m_index{index}
      {

      }

    bool operator==(buffer_iterator const & other) const
      {
      return (m_buffer == other.m_buffer) && (m_index == other.m_index);
      }

    bool operator<(buffer_iterator const & other) const
      {
      return (m_buffer == other.m_buffer) && (m_index < other.m_index);
      }

    buffer_iterator & operator++()
      {
      throw_on_overflow();
      ++m_index;

      return *this;
      }

    buffer_iterator & operator--()
      {
      throw_on_underrun();
      --m_index;

      return *this;
      }

    buffer_iterator & operator+=(difference_type const offset)
      {
      throw_on_overflow(offset);
      m_index += offset;

      return *this;
      }

    buffer_iterator & operator-=(difference_type const offset)
      {
      throw_on_underrun(offset);
      m_index -= offset;

      return *this;
      }

    difference_type operator-(buffer_iterator const & other) const
      {
      throw_on_different_buffer(other);
      throw_on_underrun(other.m_index - m_index);

      return m_index > other.m_index ? m_index - other.m_index : other.m_index - m_index;
      }

    auto operator*() const
      {
      throw_on_invalid_position();

      return (*m_buffer)[m_index];
      }

    private:
      BufferType * m_buffer;
      std::size_t m_index;

      void throw_on_underrun(typename BufferType::size_type const offset = 1) const
        {
        if(m_index - offset <= 0)
          {
          throw std::logic_error{"Iterator out of range - underrun"};
          }
        }

      void throw_on_overflow(typename BufferType::size_type const offset = 1) const
        {
        if(m_index + offset >= m_buffer->size())
          {
          throw std::logic_error{"Iterator out of range - overflow"};
          }
        }

      void throw_on_invalid_position() const
        {
        if(m_index >= m_buffer->size() || m_index < 0)
          {
          throw std::logic_error{"Invalid iterator access - out of bounds"};
          }
        }

      void throw_on_different_buffer(buffer_iterator const & other) const
        {
        if(m_buffer != other.m_buffer)
          {
          throw std::logic_error{"Iterators in binary expression are created from different buffers!"};
          }
        }

    };

















  using value_type      = ValueType;
  using reference       = ValueType &;
  using const_reference = ValueType const &;
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
    : m_maximumSize{other.m_maximumSize},
      m_first{other.m_first},
      m_size{other.m_size},
      m_data{std::move(other.m_data)}
    {
    other.m_data = nullptr;
    }

  ~BoundedBuffer()
    {
    clear();
    delete[](m_data);
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
    return throw_if_empty(), get(m_first);
    }

  reference front()
    {
    return throw_if_empty(), get(m_first);
    }

  const_reference back() const
    {
    return throw_if_empty(), get(back_index());
    }

  reference back()
    {
    return throw_if_empty(), get(back_index());
    }

  void push(value_type const & elem)
    {
    throw_if_full();
    ::new (ptr() + push_index()) value_type{elem};
    }

  void push(value_type && elem)
    {
    throw_if_full();
    ::new (ptr() + push_index()) value_type{std::move(elem)};
    }

  void pop()
    {
    throw_if_empty();

    ptr()[m_first].~value_type();

    m_first = (m_first + 1) % m_maximumSize;
    --m_size;
    }

  void swap(BoundedBuffer & other)
    {
    std::swap(m_maximumSize, other.m_maximumSize);
    std::swap(m_first, other.m_first);
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
    }

  BoundedBuffer & operator=(BoundedBuffer const & other)
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

  BoundedBuffer & operator=(BoundedBuffer && other)
    {
    clear();
    delete[](m_data);

    m_maximumSize = other.m_maximumSize;
    m_first = other.m_first;
    m_size = other.m_size;
    m_data = other.m_data;
    other.m_data = nullptr;

    return *this;
    }

  iterator begin()
    {
    return iterator{this, 0};
    }

  iterator end()
    {
    return iterator{this, m_size};
    }

  const_iterator begin() const
    {
    return const_iterator{this, 0};
    }

  const_iterator end() const
    {
    return const_iterator{this, m_size};
    }

  const_iterator cbegin() const
    {
    return begin();
    }

  const_iterator cend() const
    {
    return end();
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

    void throw_if_empty() const
      {
      if(empty()) throw std::logic_error{"BoundedBuffer is empty"};
      }

    void throw_if_full() const
      {
      if(full()) throw std::logic_error{"BoundedBuffer is full"};
      }

    value_type & get(size_type const idx) const
      {
      return ptr()[idx];
      }

    value_type * ptr() const
      {
      return reinterpret_cast<value_type *>(m_data);
      }

    void copy(BoundedBuffer const & other)
      {
      for(size_type idx = 0; idx < other.m_size; ++idx)
        {
        push(other.get((other.m_first + idx) % m_maximumSize));
        }
      }

    void move(BoundedBuffer && other)
      {
      for(size_type idx = 0; idx < other.m_size; ++idx)
        {
        push(other.ptr()[((other.m_first + idx) % m_maximumSize)]);
        }

      other.clear();
      }

    void clear()
      {
      for(size_type idx = 0; idx < m_size; ++idx)
        {
        destroy((m_first + idx) % m_maximumSize);
        }

      m_size = 0;
      m_first = 0;
      }

    void destroy(size_type const idx)
      {
      ptr()[idx].~value_type();
      }

    reference operator[](size_type const index)
      {
      return ptr()[(m_first + index) % m_maximumSize];
      }

    const_reference operator[](size_type const index) const
      {
      return ptr()[(m_first + index) % m_maximumSize];
      }

    size_type m_maximumSize{};

    size_type m_first{};
    size_type m_size{};

    char * m_data{};

  };


#endif

