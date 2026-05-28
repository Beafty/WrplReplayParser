#pragma once

#include <vector>
// used simply because it allows for uint32_t as size_type
// luckily its default is already uint32_t
#include <dag/dag_vector.h>

// if you know the data will most likely be different on every iteration, then set compare to false
// or else it will try to compare every new object so it can not include repeated values
// this will be used alot in reflection, but for something like unit position not really
// skip_to is for stuff like
template<typename Derived, typename T, bool compare = true, bool skip_to = false>
class RewindMgr {
public:
    struct TimeState {
        uint32_t time_ms;
        T data;
    };

protected:
    dag::Vector<TimeState> timeStates;
    uint32_t curr_index = 0;

    inline uint32_t getTime(int index) {
        if (index < 0)
            return 0;
        if (index >= (int) timeStates.size())
            return 0xFFFFFFFF;
        return timeStates[index].time_ms;
    }

    Derived &self() { return *static_cast<Derived *>(this); }
    const Derived &self() const { return *static_cast<const Derived *>(this); }


    template<typename... Args>
    inline void goToIndex(uint32_t time_ms, Args... args) {
        auto curr_time = getTime(curr_index - 1);
        if (curr_time < time_ms) {
            auto iter = std::upper_bound(this->timeStates.begin() + curr_index, this->timeStates.end(), time_ms,
                                         [](uint32_t val, const TimeState &data) {
                                             return val < data.time_ms;
                                         });
            curr_index = std::distance(this->timeStates.begin(), iter);
            self().forward(timeStates[curr_index], std::forward<Args>(args)...);
        } else {
            auto iter = std::lower_bound(this->timeStates.begin() + curr_index, this->timeStates.end(), time_ms,
                                         [](const TimeState &data, uint32_t val) {
                                             return data.time_ms < val;
                                         });
            curr_index = std::distance(this->timeStates.begin(), iter);
            self().backward(this->timeStates[curr_index], args...);
        }
    }

    template<typename... Args>
    void iterateToIndex(uint32_t time_ms, Args &&... args) {
        const uint32_t sz = static_cast<uint32_t>(timeStates.size());
        if (sz == 0) return;

        while (curr_index > 0 && timeStates[curr_index - 1].time_ms > time_ms) {
            --curr_index;
            self().backward(timeStates[curr_index], std::forward<Args>(args)...);
        }

        while (curr_index < sz && timeStates[curr_index].time_ms <= time_ms) {
            self().forward(timeStates[curr_index], std::forward<Args>(args)...);
            ++curr_index;
        }
    }

public:
    const dag::Vector<const TimeState> &getStates() {
        return timeStates;
    }

    template<typename... Args>
    void rewindTo(uint32_t time_ms, Args... args) {
        curr_index = eastl::clamp(curr_index, (uint32_t) 0, this->timeStates.size());
        if constexpr (skip_to) {
            goToIndex(time_ms, args...);
        } else {
            iterateToIndex(time_ms, args...);
        }
    }

    TimeState &emplaceNew() {
        this->curr_index = this->timeStates.size() + 1;
        return this->timeStates.emplace_back();
    }

    TimeState &getState(uint32_t idx) {
        G_ASSERT(idx < timeStates.size());
        return timeStates[idx];
    }
};
