#pragma once
#include "Module.h"
#include "ecs/typesAndLimits.h"
#include "ecs/component.h"


template<typename Vec, typename CName = Vec>
void bind_readonly_vector(py::module_ &m, const char *name) {
  py::class_<Vec>(m, name)
      .def("__len__", [](const Vec &v) { return v.size(); })
      .def("__getitem__", [](const Vec &v, size_t i) {
        if (i >= v.size()) throw py::index_error();
        return v[i];
      })
      .def("__iter__", [](const Vec &v) {
        return py::make_iterator(v.begin(), v.end());
      }, py::keep_alive<0, 1>()) // Keep vector alive while iterator exists
      .def("__contains__", [](const Vec &v, const typename Vec::value_type &val) {
        return std::find(v.begin(), v.end(), val) != v.end();
      })
      .def("__repr__", [name](const Vec &v) {
        std::ostringstream oss;
        oss << name << "(";
        oss << "[";
        for (size_t i = 0; i < v.size(); ++i) {
          if (i) oss << ", ";
          oss << py::repr(py::cast(v[i]));
        }
        oss << "])";
        return oss.str();
      });
}

namespace ecs {
  class Component;
}
typedef py::object (*cast_cb)(const void*);
extern std::unordered_map<ecs::component_type_t, cast_cb> cast_to_py_types;
py::object castData(ecs::component_type_t hash, const void *data);
py::object castComponent(const ecs::Component *comp);

template <typename T>
py::object casterTemplate(const void*data) {
  return py::cast((const T*)data);
}
template <>
inline py::object casterTemplate<ecs::string>(const void* data) {
  ecs::string ptr = *(ecs::string*)data;
  if(ptr.size() > 0) {
    ptr.resize(ptr.size()-1);
  }
  return py::cast(ptr);
}

#define REGISTER_TYPE(T) cast_to_py_types.emplace(ecs::ComponentTypeInfo<T>::type, casterTemplate<T>)



class PyECSTypes : protected Module {
private:
  void include_array(py::module_ &m);
public:
  PyECSTypes() : Module() {}
  void include(py::module_ &m);
};



extern PyECSTypes py_ecs_types;