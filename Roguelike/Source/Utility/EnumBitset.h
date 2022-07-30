#pragma once

template <class T>
class EnumBitset {
public:
    static void On(int32& value, const T flag) {
        value = static_cast<int32>(static_cast<int32>(value) | static_cast<int32>(1 << static_cast<int32>(flag)));
    }
    static void Off(int32& value, const T& flag) {
        value = static_cast<int32>(static_cast<int32>(value) & ~static_cast<int32>(1 << static_cast<int32>(flag)));
    }
    static bool IsOn(const int32& value, const T& flag) {
        return (value & (1 << static_cast<int32>(flag))) > 0;
    }
    static bool IsOff(const int32& value, const T& flag) {
        return (value & (1 << static_cast<int32>(flag))) == 0;
    }

private:
    EnumBitset() {};
    ~EnumBitset() {};
};
