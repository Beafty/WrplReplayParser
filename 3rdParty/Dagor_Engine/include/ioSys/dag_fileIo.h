//
// Dagor Engine 6.5
// Copyright (C) Gaijin Games KFT.  All rights reserved.
//
#pragma once

#include <ioSys/dag_baseIo.h>
#include "FileSystem.h"

/// @addtogroup utility_classes
/// @{

/// @addtogroup serialization
/// @{


/// @file
/// Serialization callbacks.


/// File load callback.
class LFileGeneralLoadCB : public IBaseLoad {
public:
    File *fileHandle;

    explicit LFileGeneralLoadCB(File *handle = nullptr);

    void read(void *ptr, int size) override;

    int tryRead(void *ptr, int size) override;

    int tell() override;

    void seekto(int) override;

    bool seekrel(int) override;

    const char *getTargetName() override { return fileHandle->getFullFilePath().string().c_str(); }

    const VROMFs *getTargetVromFs() const override;

    std::span<char> getTargetRomData() const override {
        if (fileHandle) return fileHandle->readRaw();
        return {};
    }
};


/// Callback for reading whole file. Closes file in destructor.
class FullFileLoadCB : public LFileGeneralLoadCB {
    int64_t targetDataSz = -1;
    std::shared_ptr<File> shared_ptr;

public:
    inline explicit FullFileLoadCB(const std::string &fname, bool lower_fname = false) {
        fileHandle = nullptr;
        open(fname, lower_fname);
    }

    inline ~FullFileLoadCB() override { close(); }

    bool open(const std::string &fname, bool lower_fname = false);

    void close();

    void beginFullFileBlock();

    int64_t getTargetDataSize() override { return targetDataSz; }
};

class StreamingFileLoadCB : public IBaseLoad {
    uint32_t deadline_seconds = 0;
    std::string fName;
    std::ifstream input;
    std::streamoff data_size;
    int read_offset = 0;

public:
    explicit StreamingFileLoadCB(const std::string &fname);

    void read(void *ptr, int size) override;

    int tryRead(void *ptr, int size) override;

    int tell() override;

    void seekto(int) override;

    bool seekrel(int) override;

    const char *getTargetName() override { return fName.c_str(); }
};


/// @}

/// @}
