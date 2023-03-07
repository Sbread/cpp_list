#pragma once
#include <cassert>
#include <iterator>

template <typename T>
class list {
private:

  struct value_node;
  struct node;

  template <typename V>
  struct my_iterator;

public:
  // bidirectional iterator
  using iterator = my_iterator<T>;
  // bidirectional iterator
  using const_iterator = my_iterator<T const>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // O(1)
  list() noexcept;

  // O(n), strong
  list(list const&);

  // O(n), strong
  list& operator=(list const&);

  // O(n)
  ~list();

  // O(1)
  bool empty() const noexcept;

  // O(1)
  T& front() noexcept;
  // O(1)
  T const& front() const noexcept;

  // O(1), strong
  void push_front(T const&);
  // O(1)
  void pop_front() noexcept;

  // O(1)
  T& back() noexcept;
  // O(1)
  T const& back() const noexcept;

  // O(1), strong
  void push_back(T const&);
  // O(1)
  void pop_back() noexcept;

  // O(1)
  iterator begin() noexcept;
  // O(1)
  const_iterator begin() const noexcept;

  // O(1)
  iterator end() noexcept;
  // O(1)
  const_iterator end() const noexcept;

  // O(1)
  reverse_iterator rbegin() noexcept;
  // O(1)
  const_reverse_iterator rbegin() const noexcept;

  // O(1)
  reverse_iterator rend() noexcept;
  // O(1)
  const_reverse_iterator rend() const noexcept;

  // O(n)
  void clear() noexcept;

  // O(1), strong
  iterator insert(const_iterator pos, T const& val);
  // O(1)
  iterator erase(const_iterator pos) noexcept;
  // O(n)
  iterator erase(const_iterator first, const_iterator last) noexcept;
  // O(1)
  void splice(const_iterator pos, list& other, const_iterator first,
              const_iterator last) noexcept;

  friend void swap(list& a, list& b) noexcept {
      a.swap(b);
  };

private:
  struct node {
  public:
    T& get_value() {
      return static_cast<value_node*>(this)->get_value();
    }

    node* prev{this};
    node* next{this};
  };

  struct value_node : node {
  public:
    value_node(node* prev_, node* next_, T const& val) : value(val) {
      node::prev = prev_;
      node::next = next_;
    }

    T& get_value() {
      return value;
    }
    T value;
  };

  template <typename V>
  struct my_iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = V;
    using pointer = V*;
    using reference = V&;

    my_iterator(iterator const& other) : memory_ptr(other.memory_ptr) {};

    reference operator*() const {
      return memory_ptr->get_value();
    }

    pointer operator->() const {
      return &(memory_ptr->get_value());
    }

    my_iterator& operator++() {
      memory_ptr = memory_ptr->next;
      return *this;
    }

    my_iterator& operator--() {
      memory_ptr = memory_ptr->prev;
      return *this;
    }

    my_iterator operator++(int) {
      my_iterator a = *this;
      ++(*this);
      return a;
    }

    my_iterator operator--(int) {
      my_iterator a = *this;
      --(*this);
      return a;
    }

    bool operator==(const_iterator const& other) const {
      return memory_ptr == other.memory_ptr;
    }

    bool operator!=(const_iterator const& other) const {
      return memory_ptr != other.memory_ptr;
    }

  private:
    explicit my_iterator(node* memory_ptr_) : memory_ptr(memory_ptr_) {};

    node* memory_ptr{nullptr};

    friend class list;
  };

  void swap(list& other);

  node base;
};

template <typename T>
list<T>::list() noexcept = default;

template <typename T>
list<T>::list(const list& other) : list() {
  for (auto &i : other) {
    push_back(i);
  }
}

template <typename T>
list<T>& list<T>::operator=(const list& other) {
  if (this != &other) {
    list(other).swap(*this);
  }
  return *this;
}

template <typename T>
list<T>::~list() {
  erase(begin(), end());
}

template <typename T>
bool list<T>::empty() const noexcept {
  return base.next == &base;
}

template <typename T>
typename list<T>::iterator list<T>::begin() noexcept {
  return iterator(base.next);
}

template <typename T>
typename list<T>::const_iterator list<T>::begin() const noexcept {
  return const_iterator(base.next);
}

template <typename T>
typename list<T>::iterator list<T>::end() noexcept {
  return iterator(&base);
}

template <typename T>
typename list<T>::const_iterator list<T>::end() const noexcept {
  return const_iterator(const_cast<node*>(&base));
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rend() noexcept {
  return reverse_iterator(begin());
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::rend() const noexcept {
  return const_reverse_iterator(begin());
}

template <typename T>
T& list<T>::front() noexcept {
  return *(begin());
}

template <typename T>
T const& list<T>::front() const noexcept {
  return *(begin());
}

template <typename T>
T& list<T>::back() noexcept {
  return (*(std::prev(end())));
}
template <typename T>
T const& list<T>::back() const noexcept {
  return (*(std::prev(end())));
}

template <typename T>
void list<T>::clear() noexcept {
  erase(begin(), end());
}

template <typename T>
void list<T>::push_front(const T& val) {
  insert(begin(), val);
}

template <typename T>
void list<T>::pop_front() noexcept {
  erase(begin());
}

template <typename T>
void list<T>::push_back(const T& val) {
  insert(end(), val);
}

template <typename T>
void list<T>::pop_back() noexcept {
  erase(std::prev(end()));
}

template <typename T>
typename list<T>::iterator list<T>::insert(const_iterator pos, const T& val) {
  node* curr = static_cast<node*>(new value_node(pos.memory_ptr->prev, pos.memory_ptr, val));
  pos.memory_ptr->prev->next = curr;
  pos.memory_ptr->prev = curr;
  return iterator(curr);
}

template <typename T>
typename list<T>::iterator list<T>::erase(list::const_iterator pos) noexcept {
  return erase(pos, std::next(pos));
}

template <typename T>
typename list<T>::iterator list<T>::erase(const_iterator first,
                                          const_iterator last) noexcept {
  if (first != last) {
    first.memory_ptr->prev->next = last.memory_ptr;
    last.memory_ptr->prev = first.memory_ptr->prev;
    node* curr1 = first.memory_ptr;
    while(curr1 != last.memory_ptr) {
      auto tmp = curr1->next;
      delete static_cast<value_node*>(curr1);
      curr1 = tmp;
    }
  }
  return iterator(last.memory_ptr);
}

template <typename T>
void list<T>::splice(const_iterator pos, list& other,
                     const_iterator first,
                     const_iterator last) noexcept {
  if (first == last) {
    return;
  }
  node* tmp_pos = pos.memory_ptr;
  node* tmp_first = first.memory_ptr;
  node* tmp_last = last.memory_ptr->prev;
  tmp_last->next->prev = tmp_first->prev;
  tmp_first->prev->next = tmp_last->next;
  tmp_last->next = tmp_pos;
  tmp_first->prev = tmp_pos->prev;
  tmp_pos->prev->next = tmp_first;
  tmp_pos->prev = tmp_last;
}

template <typename T>
void list<T>::swap(list& other) {
  if (empty()) {
    base.prev = &other.base;
    base.next = &other.base;
  } else {
    base.prev->next = &other.base;
    base.next->prev = &other.base;
  }
  if (other.empty()) {
    other.base.prev = &base;
    other.base.next = &base;
  } else {
    other.base.prev->next = &base;
    other.base.next->prev = &base;
  }
  std::swap(base, other.base);
}
