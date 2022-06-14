#pragma once
#include <cstdint>
#include <iostream>

struct ChunkCoordinates {
    //Not using glm::vec2 because it's x and y not x and z
    //Since y is up
    int x = 0;
    int z = 0;

    static inline ChunkCoordinates Abs(const ChunkCoordinates& cord) {
        return ChunkCoordinates{ std::abs(cord.x), std::abs(cord.z) };
    }
};

static ChunkCoordinates operator+(const ChunkCoordinates& a, const int b) {
    return ChunkCoordinates{ a.x + b, a.z + b };
}

static ChunkCoordinates operator*(const ChunkCoordinates& a, const int b) {
    return ChunkCoordinates{ a.x * b, a.z * b };
}

static ChunkCoordinates& operator+=(ChunkCoordinates& a, const ChunkCoordinates& b) {
    return a = ChunkCoordinates{ a.x + b.x, a.z + b.z };
}

static ChunkCoordinates& operator-=(ChunkCoordinates& a, const ChunkCoordinates& b) {
    return a = ChunkCoordinates{ a.x - b.x, a.z - b.z };
}

static bool operator!=(const ChunkCoordinates& a, const ChunkCoordinates& b) {
    return a.x != b.x || a.z != b.z;
}

static bool operator==(const ChunkCoordinates& a, const ChunkCoordinates& b) {
    return a.x == b.x && a.z == b.z;
}

class ChunkCoordinateHash {
public:
    size_t operator()(const ChunkCoordinates& val) const {
        static_assert(sizeof(size_t) == 8);
        return (static_cast<size_t>(val.x) << 32ull) + (static_cast<size_t>(val.z) & 0xffffffff);
    }
};