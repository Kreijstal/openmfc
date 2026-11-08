#pragma once
#include <cstdint>
#include <cstring>

namespace openmfc { namespace detail {
// DB_NUMERIC stores an unsigned, little-endian 128-bit coefficient. Four
// 32-bit limbs keep its decimal arithmetic available to MSVC without compiler
// extensions, including the native multiply's modulo-2^128 overflow behavior.
class Numeric128 {
public:
    Numeric128() = default;
    explicit Numeric128(const void* bytes) {std::memcpy(words,bytes,sizeof(words));}
    void Store(void* bytes) const {std::memcpy(bytes,words,sizeof(words));}
    bool IsZero() const {return !(words[0]|words[1]|words[2]|words[3]);}
    void AppendDigit(unsigned digit) {
        std::uint64_t carry=digit;
        for(auto& word:words) {
            const std::uint64_t value=std::uint64_t(word)*10+carry;
            word=static_cast<std::uint32_t>(value);
            carry=value>>32;
        }
    }
    unsigned RemoveDigit() {
        std::uint64_t remainder=0;
        for(int i=3;i>=0;--i) {
            const std::uint64_t value=(remainder<<32)|words[i];
            words[i]=static_cast<std::uint32_t>(value/10);
            remainder=value%10;
        }
        return static_cast<unsigned>(remainder);
    }
private:
    std::uint32_t words[4]{};
};
static_assert(sizeof(Numeric128)==16,"DB_NUMERIC coefficient width");
}}
