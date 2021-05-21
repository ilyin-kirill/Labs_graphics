#include "Geometry.h"


bool Plane::Intersect(const Ray& ray, float t_min, float t_max, SurfHit& surf) const
{
	surf.t = dot((point - ray.o), normal) / dot(ray.d, normal);

	if (surf.t > t_min && surf.t < t_max)
	{
		surf.hit = true;
		surf.hitPoint = ray.o + surf.t * ray.d;
		surf.normal = normal;
		surf.m_ptr = m_ptr;
		return true;
	}

	return false;
}

bool Sphere::Intersect(const Ray& ray, float t_min, float t_max, SurfHit& surf) const
{

	float3 k = ray.o - center;

	float a = dot(ray.d, ray.d);
	float b = dot(2 * k, ray.d);
	float c = dot(k, k) - r_sq;

	float d = b * b - 4 * a * c;

	if (d < 0) return false;

	surf.t = (-b - sqrt(d)) / 2 * a;

	if (surf.t > t_min && surf.t < t_max)
	{
		surf.hit = true;
		surf.hitPoint = ray.o + surf.t * ray.d;
		surf.normal = normalize(surf.hitPoint - center);
		surf.m_ptr = m_ptr;
		return true;
	}

	surf.t = (-b + sqrt(d)) / 2 * a;
	if (surf.t > t_min && surf.t < t_max)
	{
		surf.hit = true;
		surf.hitPoint = ray.o + surf.t * ray.d;
		surf.normal = normalize(surf.hitPoint - center);
		surf.m_ptr = m_ptr;
		return true;
	}

	return false;
}

bool Parallel::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	float dirfracX = 1.0f / ray.d.x;
	float dirfracY = 1.0f / ray.d.y;
	float dirfracZ = 1.0f / ray.d.z;
	float x1 = (t_min.x - ray.o.x) * dirfracX;
	float x2 = (t_max.x - ray.o.x) * dirfracX;
	float y1 = (t_min.y - ray.o.y) * dirfracY;
	float y2 = (t_max.y - ray.o.y) * dirfracY;
	float z1 = (t_min.z - ray.o.z) * dirfracZ;
	float z2 = (t_max.z - ray.o.z) * dirfracZ;

	float tMin = max(max(min(x1, x2), min(y1, y2)), min(z1, z2));
	float tMax = min(min(max(x1, x2), max(y1, y2)), max(z1, z2));


	surf.t = tMin;
	if (tMin < tMax && tMax > 0 && surf.t > tmin && surf.t < tmax) {
		surf.hit = true;
		surf.hitPoint = ray.o + surf.t * ray.d;
		surf.normal = normalize(surf.hitPoint);
		surf.m_ptr = m_ptr;
		return true;
	}
	return false;
}

bool Triangle::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	float3 E1 = b - a;
	float3 E2 = c - a;
	float3 T = ray.o - a;
	float3 D = ray.d;
	float3 P = cross(D, E2);
	float3 Q = cross(T, E1);
	float det = dot(E1, P);

	if (det < tmin && det > tmax) {
		return false;
	}

	float invDet = 1 / det;
	float u = dot(T, P) * invDet;

	if (u < 0 || u > 1) {
		return false;
	}

	float v = dot(ray.d, Q) * invDet;

	if (v < 0 || u + v > 1) {
		return false;
	}

	surf.t = dot(E2, Q) * invDet;
	if (surf.t > tmin && surf.t < tmax) {
		surf.hit = true;
		surf.hitPoint = float3(surf.t, u, v);
		surf.normal = cross((b - a), (c - a));
		surf.m_ptr = m_ptr;
		return true;
	}
	return false;
}

bool Square::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	//return false;
	float3 p1;//вершина квадрата (одна из)
	float3 n;//нормаль к квадрату
	float3 e1;//ширина Ќќ вектором
	float3 e2;//высота (аналогично)
	p1 = a;
	e1 = b - a;
	e2 = c - a;
	n = cross(e1, e2);
	float t;
	t = dot((p1 - ray.o), n) / dot(ray.d, n);
	if (t >= 0)
	{
		float3 intersect_point;
		intersect_point = (ray.o + ray.d);
		float3 v;
		v = intersect_point - p1;
		float width, height;
		width = length(e1);
		height = length(e2);
		float proj1, proj2;
		proj1 = dot(v, e1) / width;
		proj2 = dot(v, e2) / height;
		if ((proj1 < width && proj1>0) && (proj2 < height && proj2>0))
		{
			surf.hit = true;
			surf.hitPoint = intersect_point;
			surf.m_ptr = m_ptr;
			surf.t = length(ray.o - intersect_point);
			surf.normal = n;

			return true;
		}
		else return false;
	}
	else { return false; }
}
