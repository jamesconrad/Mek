#include "Interpolation.h"



template <typename T>
T lerp(T d0, T d1, float t)
{
	return d0 * (1 - t) + d1 * t;
}

template <typename T>
T slerp(T d0, T d1, float t)
{
	return d0 * (sin(1 - t) / sin(1 / (cos(glm::dot(d0, d1))))) + d1 * (sin(t * 1 / cos(glm::dot(d0, d1))) / sin(1 / cos(glm::dot(d0, d1))));
}

template <typename T>
float invlerp(T d0, T d1, T res)
{
	return (res + (d0*-1)) / (d1 + (d0*-1));
}

void Interpolation::interpolate(float dTime)
{
	std::vector<glm::vec3> lerpItems;

	if (stage + 1 < points.size() && state == LINEAR)
	{
		pos = lerp(points[stage], points[stage + 1], time);
	}

	if (stage + 2 < points.size() && state == BEZIER2)
	{
		lerpItems.push_back(lerp(points[stage], points[stage + 1], time));
		lerpItems.push_back(lerp(points[stage + 1], points[stage + 2], time));

		pos = lerp(lerpItems[0], lerpItems[1], time);
		while (!lerpItems.empty())
			lerpItems.pop_back();
	}

	if (stage + 1 < points.size() && state == CATMULLROM)
	{
		float t = time;
		float t2 = time*time;
		float t3 = time*time*time;

		//catmull rom spline calc

		glm::vec3 prev(0, 0, 0);
		glm::vec3 last(0, 0, 0);

		if (stage == 0)
			prev = points[stage];
		else
			prev = points[stage - 1];

		if (stage + 2 == points.size())
			last = points[points.size() - 1];
		else
			last = points[stage + 2];
		pos.x = ((-t3 + 2 * t2 - t)*(prev.x) + (3 * t3 - 5 * t2 + 2)*(points[stage].x) + (-3 * t3 + 4 * t2 + t)* (points[stage + 1].x) + (t3 - t2)*(last.x)) / 2;
		pos.y = ((-t3 + 2 * t2 - t)*(prev.y) + (3 * t3 - 5 * t2 + 2)*(points[stage].y) + (-3 * t3 + 4 * t2 + t)* (points[stage + 1].y) + (t3 - t2)*(last.y)) / 2;

	}

	if (stage + 1 < points.size() && state == 4)
	{
		pos = slerp(points[stage], points[stage + 1], time);
	}

	//loop
	if (cycles >= points.size() - 1)
	{
		{
			stage = 0;
			time = 0;
			cycles = 0;
		}
		curveFin = true;
	}

	else if (state == 2)
	{
		if (points.size() % 2 == 0)
		{
			if (cycles >= points.size() - 3)
			{
				stage = 0;
				time = 0;
				cycles = 0;
			}
			curveFin = true;
		}
		else if (cycles >= points.size() - 2)
		{
			if (loop)
			{
				stage = 0;
				time = 0;
				cycles = 0;
			}
			curveFin = true;
		}
	}

	if (time >= 1.0f)
	{
		time = 0.0f;
		stage++;
	}

	if (!paused || !curveFin)
		time += (float)dTime;

	if (time >= 1.0f)
	{
		time = 0;
		if (state == 2)
		{
			stage += state;
			cycles += state;
		}
		else
		{
			stage++;
			cycles++;
		}
	}
}