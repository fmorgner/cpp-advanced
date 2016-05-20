#ifndef __FMO__BOUNDED_QUEUE
#define __FMO__BOUNDED_QUEUE

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>

template<typename ValueType, typename MutexType = std::mutex, typename ConditionType = std::condition_variable>
struct BoundedQueue
  {
  using value_type      = ValueType;
  using reference       = value_type &;
  using const_reference = value_type const &;
  using pointer         = value_type *;
  using const_pointer   = value_type const *;
  using size_type       = std::size_t;

  using __mutex = MutexType;
  using __condition = ConditionType;
  using __guard = std::lock_guard<__mutex>;
  using __ulock = std::unique_lock<__mutex>;

  BoundedQueue(size_type const size)
    : m_maximumSize{size ? size : throw std::invalid_argument{"Tried to allocate BoundedQueue of size 0"}},
      m_data{new char[size * sizeof(ValueType)]}
    {

    }

  BoundedQueue(BoundedQueue const & other)
    : BoundedQueue{other.m_maximumSize}
    {
    __guard guard{other.m_mutex};

    for(std::size_t index{}; index < other.m_size; ++index)
      {
      do_push(*(other.ptr() + other.to_buffer_index(index)));
      }
    }

  BoundedQueue(BoundedQueue && other)
    {
    swap(other);
    }

  ~BoundedQueue()
    {
    while(!do_empty())
      {
      (ptr() + m_first)->~value_type();
      m_first = (m_first + 1) % m_maximumSize;
      --m_size;
      }

    delete[](m_data);
    }

  auto empty() const
    {
    __guard guard{m_mutex};
    return do_empty();
    }

  auto full() const
    {
    __guard guard{m_mutex};
    return do_full();
    }

  auto size() const
    {
    __guard guard{m_mutex};
    return m_size;
    }

  auto push(value_type const & elem)
    {
    __ulock ulock{m_mutex};
    m_hasSpace.wait(ulock, [&]{ return !do_full(); });

    do_push(elem);

    m_hasElements.notify_one();
    }

  auto push(value_type && elem)
    {
    __ulock ulock{m_mutex};
    m_hasSpace.wait(ulock, [&]{ return !do_full(); });

    do_push(std::move(elem));

    m_hasElements.notify_one();
    }

  value_type pop()
    {
    __ulock ulock{m_mutex};
    m_hasElements.wait(ulock, [&]{ return !do_empty(); });

    auto temporary = do_pop();

    m_hasSpace.notify_one();

    return temporary;
    }

  auto try_push(value_type const & elem)
    {
    __ulock ulock{m_mutex};
    if(do_full())
      {
      return false;
      }

    do_push(elem);

    m_hasElements.notify_one();
    return true;
    }

  auto try_push(value_type && elem)
    {
    __guard guard{m_mutex};
    if(do_full())
      {
      return false;
      }

    do_push(std::move(elem));

    m_hasElements.notify_one();
    return true;
    }

  template<typename RepresentationType, typename Period>
  auto try_push_for(value_type const & elem, std::chrono::duration<RepresentationType, Period> const & timeout)
    {
    __ulock ulock{m_mutex};
    if(!m_hasSpace.wait_for(ulock, timeout, [&]{ return !do_full(); }))
      {
      return false;
      }

    do_push(elem);

    m_hasElements.notify_one();

    return true;
    }

  template<typename RepresentationType, typename Period>
  auto try_pop_for(value_type & target, std::chrono::duration<RepresentationType, Period> const & timeout)
    {
    __ulock ulock{m_mutex};
    if(!m_hasElements.wait_for(ulock, timeout, [&]{ return !do_empty(); }))
      {
      return false;
      }

    target = do_pop();

    m_hasSpace.notify_one();

    return true;
    }

  auto try_pop(value_type & target)
    {
    __ulock ulock{m_mutex};
    if(do_empty())
      {
      return false;
      }

    target = do_pop();

    m_hasSpace.notify_one();

    return true;
    }

  auto swap(BoundedQueue & other)
    {
    __ulock theirs{other.m_mutex, std::defer_lock};
    __ulock ours{m_mutex, std::defer_lock};
    std::lock(ours, theirs);

    std::swap(m_maximumSize, other.m_maximumSize);
    std::swap(m_first, other.m_first);
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
    }

  decltype(auto) operator=(BoundedQueue const & other)
    {
    if(this != &other)
      {
      BoundedQueue temporary{other};
      swap(temporary);
      }

    return *this;
    }

  decltype(auto) operator=(BoundedQueue && other)
    {
    if(this != &other)
      {
      swap(other);
      }

    return *this;
    }

  private:
    auto do_empty() const noexcept
      {
      return !m_size;
      }

    auto do_full() const noexcept
      {
      return m_size == m_maximumSize;
      }

    auto do_push(value_type const & element) noexcept(noexcept(new (nullptr) value_type{element}))
      {
      new (ptr() + push_index()) value_type{element};
      }

    auto do_push(value_type && element) noexcept(noexcept(new (nullptr) value_type{std::move(element)}))
      {
      new (ptr() + push_index()) value_type{std::move(element)};
      }

    value_type do_pop() noexcept(noexcept(std::declval<value_type &>() = std::declval<value_type>()) && noexcept(value_type{std::declval<value_type>()}))
      {
      auto temporary = std::move(*(ptr() + m_first));
      (ptr() + m_first)->~value_type();

      m_first = (m_first + 1) % m_maximumSize;
      --m_size;

      return temporary;
      }

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

    auto ptr() noexcept
      {
      return reinterpret_cast<pointer>(m_data);
      }

    auto ptr() const noexcept
      {
      return reinterpret_cast<const_pointer>(m_data);
      }

    size_type m_maximumSize{};
    size_type m_first{};
    size_type m_size{};
    char * m_data{};

    __mutex mutable m_mutex{};
    __condition m_hasSpace{};
    __condition m_hasElements{};
  };

#endif

