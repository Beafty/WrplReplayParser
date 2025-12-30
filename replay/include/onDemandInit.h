

#ifndef MYEXTENSION_ONDEMANDINIT_H
#define MYEXTENSION_ONDEMANDINIT_H

template <typename T>
struct OnDemandInit {
public:
  OnDemandInit() = default;
  T *Init()
  {
    return getPtr();
  }

  inline explicit operator T *() const { return getPtr(); }
  T &operator*() const { return *getPtr(); }
  T *operator->() const { return getPtr(); }
  T *get() const { return getPtr(); }



  ~OnDemandInit()
  {
    delete obj;
  }
private:
  T * getPtr() {
    if (!obj)
      obj = new T();
    return obj;
  }

  T * getPtr() const {
    return obj;
  }
  T * obj = nullptr;
};

#endif //MYEXTENSION_ONDEMANDINIT_H
