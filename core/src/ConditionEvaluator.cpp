#include "AchievementEngine.hpp"

namespace {
template <typename T>
const T* getIf(const MockStateValue* v) { return std::get_if<T>(v); }
}

bool evaluateCondition(const Condition& cond, const MockState& state) {
    auto it = state.find(cond.key);
    if (it == state.end()) return false;

    const auto& sv = it->second;

    switch (cond.type) {
        case ConditionType::StateEquals: {
            if (const auto* b = std::get_if<bool>(&cond.value)) {
                if (const auto* sb = getIf<bool>(&sv)) return *sb == *b;
            }
            if (const auto* d = std::get_if<double>(&cond.value)) {
                if (const auto* sd = getIf<double>(&sv)) return *sd == *d;
                if (const auto* si = getIf<int>(&sv)) return static_cast<double>(*si) == *d;
            }
            if (const auto* s = std::get_if<std::string>(&cond.value)) {
                if (const auto* ss = getIf<std::string>(&sv)) return *ss == *s;
            }
            if (const auto* i = std::get_if<int>(&cond.value)) {
                if (const auto* si = getIf<int>(&sv)) return *si == *i;
            }
            return false;
        }
        case ConditionType::StateGreaterThan: {
            const auto* d = std::get_if<double>(&cond.value);
            if (!d) return false;
            if (const auto* sd = getIf<double>(&sv)) return *sd > *d;
            if (const auto* si = getIf<int>(&sv)) return static_cast<double>(*si) > *d;
            return false;
        }
        case ConditionType::StateLessThan: {
            const auto* d = std::get_if<double>(&cond.value);
            if (!d) return false;
            if (const auto* sd = getIf<double>(&sv)) return *sd < *d;
            if (const auto* si = getIf<int>(&sv)) return static_cast<double>(*si) < *d;
            return false;
        }
        case ConditionType::StateBitSet: {
            const auto* bit = std::get_if<int>(&cond.value);
            if (!bit || *bit < 0 || *bit > 30) return false;
            if (const auto* si = getIf<int>(&sv)) return ((*si >> *bit) & 1) == 1;
            return false;
        }
    }
    return false;
}
