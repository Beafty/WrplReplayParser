#pragma once
#include "cstdint"
#include "vector"
#include "span"

class IWriter {
public:
    IWriter() = default;
    IWriter(const IWriter &) = default;
    IWriter(IWriter &&) = default;
    IWriter &operator=(const IWriter &) = default;
    IWriter &operator=(IWriter &&) = default;
    virtual ~IWriter() = default;
    virtual void write(const void *ptr, size_t size) = 0;

    template <typename T>
    void writeObj( T &obj) {
        write(&obj, sizeof(T));
    }
};

class BaseWriter : public IWriter {
    std::vector<uint8_t> data{};
public:
    ~BaseWriter() override = default;
    void write(const void *ptr, size_t size) override {
        //data.append_range(std::span((uint8_t*)ptr, size));
    }
    void reserve(size_t size) {
        data.reserve(size);
    }
    const std::vector<uint8_t>& getData() {return data;}
};