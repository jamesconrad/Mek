#include "Interpolation.h"

float Interpolation::arclength(int i)
{
	glm::vec3 d = curve[i].v + (curve[i - 1].v * -1.f);
	float ad = sqrt(pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2));
	return ad + curve[i - 1].arcl;
}

void Interpolation::buildCurve()
{
	ce c;
	curveFin = false;
	while (!curveFin && points.size() > 1)
	{
		interpolate(0.04);
		
		c.v = pos;
		c.seg = stage;
		c.tval = time;
		c.arcl = 0;
		curve.push_back(c);
	}
	c.v = points.back();
	c.seg = stage;
	c.tval = 1.0f;
	c.arcl = 0;
	curve.push_back(c);
	//distance function
	//mag(p2 - p1) + alp1
	for (int i = 1; i < curve.size(); i++)
	{
		curve[i].arcl = arclength(i);
	}

	for (int i = 0; i < curve.size(); i++)
	{
		curve[i].arcl /= curve.back().arcl;
	}
}

void Interpolation::interpolate(float dTime)
{
	std::vector<glm::vec3> lerpItems;

	if (time >= 1.0f)
		time = 1.0f;
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
		pos.z = ((-t3 + 2 * t2 - t)*(prev.z) + (3 * t3 - 5 * t2 + 2)*(points[stage].z) + (-3 * t3 + 4 * t2 + t)* (points[stage + 1].z) + (t3 - t2)*(last.z)) / 2;
		pos.y = ((-t3 + 2 * t2 - t)*(prev.y) + (3 * t3 - 5 * t2 + 2)*(points[stage].y) + (-3 * t3 + 4 * t2 + t)* (points[stage + 1].y) + (t3 - t2)*(last.y)) / 2;
	}

	if (stage + 1 < points.size() && state == SLERP)
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

void Interpolation::speedControlInterp(float dTime)
{
	isFinished = false;
	if (curve.size() > 1)
	{
		//stage 1 lerp for distance
		//d is incremented by vel and reset to 0
		if (time > 1.0f)
			time = 1.0f;
		float dst = lerp(0.0f, 1.0f, time);
		//stage 2 find closest points
		int p1 = 0, p2 = 0;
		for (int i = 1; i < curve.size(); i++)
		{
			if (curve[i].arcl > dst)
			{
				p2 = i;
				p1 = i - 1;
				break;
			}
		}

	
		float il = invlerp(curve[p1].arcl, curve[p2].arcl, dst);
	
		if (time >= 1.0f)
		{
			pos = curve.back().v;
		}
		else
			pos = lerp(curve[p1].v, curve[p2].v, il);

		time += dTime;
		
		if (time >= 1)
		{
			//glm::vec3 tempPos = curve[p2].v;
			finalCheck = curve[p2].v;
			time = 0;
			pos = finalCheck;
			if (finalCheck == curve.back().v)
			{
				isFinished = true;
			}
			else
			{
				isFinished = false;
			}
		}

	}
}