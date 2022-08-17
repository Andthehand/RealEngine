#pragma once
#include <iostream>
#include <vector>

struct Point {
	float x = 0;
	float y = 0;
};

class MachineLearning {
public:
	//θ = theta

	//y = hθ(x) = θzero + (θone * x)
	static float LogFunction(float graphX, std::vector<float> theta = m_Theta);

	//J(θzero, θone) = the cost fuinction to determine how off/the "Error" of θ is
	//Returns the sum of all of the "Error" of the functions
	static float CostFunction(std::vector<float> theta = m_Theta, std::vector<Point>& points = m_Points);

	static inline std::vector<Point> GetPoints() { return m_Points; }
	static inline void SetPoints(std::vector<Point> points) { m_Points = points; }

	static inline std::vector<float> GetTheta() { return m_Theta; }
	static inline void SetTheta(std::vector<float> theta) { m_Theta = theta; }

private:
	static inline std::vector<float> m_Theta = { 0.0f, 1.0f };

	static inline std::vector<Point> m_Points = {
		{0.0f, 0.0f},
		{4.0f, 2.0f},
		{2.0f, 1.0f},
		{1.0f, 0.5f}
	};
};
