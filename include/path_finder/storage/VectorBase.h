#pragma once
#include <vector>
#include <cstdint>

namespace pathFinder {
template <typename T, typename Allocator = std::allocator<T>>
class VectorBase {
public:
  virtual T &operator[](std::size_t index);

  virtual T *begin();

  virtual T *end();

  virtual std::size_t size();

  virtual void push_back(T element);

  virtual T *data();

  virtual void clear();

  virtual void shrink_to_fit();
};

template <typename T, typename Allocator>
T &VectorBase<T, Allocator>::operator[](std::size_t index) {
  return nullptr;
}

template <typename T, typename Allocator>
T *VectorBase<T, Allocator>::begin() {
  return nullptr;
}

template <typename T, typename Allocator>
T *VectorBase<T, Allocator>::end() {
  return nullptr;
}

template <typename T, typename Allocator>
std::size_t VectorBase<T, Allocator>::size() {
  return 0;
}

template <typename T, typename Allocator>
void VectorBase<T, Allocator>::push_back(T element) {}

template <typename T, typename Allocator>
T *VectorBase<T, Allocator>::data() {
  return nullptr;
}

template <typename T, typename Allocator>
void VectorBase<T, Allocator>::clear() {}

template <typename T, typename Allocator>
void VectorBase<T, Allocator>::shrink_to_fit() {}
} // namespace pathFinder
