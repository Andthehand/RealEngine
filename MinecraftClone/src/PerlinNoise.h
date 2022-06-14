#pragma once

#define FNL_IMPL
//#include <FastNoise/FastNoiseLite.h>
#include <RealEngine.h>

class PerlinNoise {
public:
    PerlinNoise(int seed = 1234) 
        : m_Seed(seed) {
        //m_HeightNoise.SetSeed(m_Seed);
        //m_HeightNoise.SetFractalOctaves(2);
        //m_HeightNoise.SetFractalGain(1.0f);

        //m_HeightNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        //m_HeightNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    }

    inline int GetHeightValue(int x, int z) {
        //int height = (int)round(m_HeightNoise.GetNoise((float)x, (float)z) * 16);
        //return height;
        return 0;
    }
private:
    // Create and configure FastNoise object
    //FastNoiseLite m_HeightNoise;

    int m_Seed;
};