#pragma once
#include <vector>
namespace cas {

class RealValueProvider {
  static inline std::vector<RealValueProvider *> s_registry;

public:
  RealValueProvider() {}
  virtual void tick() = 0;

  static void erase(RealValueProvider *rvp) { std::erase(s_registry, rvp); }
  static void insert(RealValueProvider *rvp) { s_registry.push_back(rvp); }

  static void tickRegistry() {
    for (auto *s : s_registry) {
      s->tick();
    }
    s_registry.clear();
  }
};

template <typename T> class RealValue : public RealValueProvider {
  T m_cur;
  T m_next;
  bool m_dirty = false;

  void tick() {
    m_cur = m_next;
    m_dirty = false;
  }

public:
  RealValue(T val) : m_cur(val), m_next(val), m_dirty(false) {}
  ~RealValue() { RealValueProvider::erase(this); }
  const T &operator*() { return m_cur; }
  RealValue &operator=(const T &other) {
    m_next = other;
    m_dirty = true;
    RealValueProvider::insert(this);
    return *this;
  }
};
} // namespace cas
