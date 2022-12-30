#include "MachineLearning.h"

glm::vec5 MachineLearning::MultivariateLinearRegression(glm::vec5 x, glm::vec5 theta) {
	return theta * x;
}

float MachineLearning::LogFunction(float graphX, std::vector<float> theta) {
	return theta[0] + (theta[1] * graphX);
}

float MachineLearning::CostFunction(std::vector<float> theta, std::vector<Point>& points) {
	float cost = 0;
	float m = (float)points.size();

	for (int i = 0; i < m; i++) {
		//(hθ(x) - y)^2
		cost += std::powf(LogFunction(points[i].x, theta) - points[i].y, 2);
	}

	//m/2m * the top for statment
	//m/2m is to cancel out to the power of 2
	cost = 1 / (2 * m) * cost;

	return cost;
}
