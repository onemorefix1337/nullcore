#pragma once

#include <cstdint>
#include <functional>

namespace Nullcore {

    class UUID {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const { return m_UUID; }

    private:
        uint64_t m_UUID;
    };

}

// специализация хеша ДЛЯ std::unordered_map
namespace std {
    template<>
    struct hash<Nullcore::UUID> {
        size_t operator()(const Nullcore::UUID& uuid) const noexcept {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}