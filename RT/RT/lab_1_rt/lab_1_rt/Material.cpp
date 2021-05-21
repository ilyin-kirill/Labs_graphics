#include "Material.h"

SurfHit& SurfHit::operator= (const SurfHit& rhs)
{
    if (this == &rhs)
        return (*this);

    hit = rhs.hit;
    hitPoint = rhs.hitPoint;
    normal = rhs.normal;
    m_ptr = rhs.m_ptr;
    t = rhs.t;

    return (*this);
}

float3 reflect(const float3& v, const float3& normal)
{
    return v - 2 * dot(v, normal) * normal;
}


bool IdealMirror::Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out)
{
    float3 reflection_dir = reflect(normalize(ray_in.d), surf.normal);

    ray_out = Ray(surf.hitPoint + surf.normal * 1e-5, reflection_dir);

    attenuation = color;

    return (dot(ray_out.d, surf.normal) > 0);
}


bool Diffuse::Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out)
{
 /*   float randX = abs(float(rand()) / RAND_MAX);
    float randY = abs(float(rand()) / RAND_MAX);

    float3 tang, btang;
    tang.x = 0.0f;
    tang.y = -1.0f;
    tang.z = surf.normal.y / surf.normal.x;
    tang = normalize(tang);

    btang = normalize((surf.normal, tang));

    float3 diff = tang * (sqrt(randX)) * cos(2 * PI * randY) + btang * (sqrt(randX)) * sin(2 * PI * randY) + surf.normal * sqrt(1 - randX);
    diff = normalize(diff);

    ray_out = Ray(surf.hitPoint + surf.normal * 1e-5, diff);

    attenuation = color * dot(ray_out.d, surf.normal);

    return true;

    btang = normalize(cross(tang, surf.normal));*/

    float kd = dot(normalize(ray_out.d), normalize(surf.normal));


    attenuation = color * kd;//���� ����� ������� �� ����


    return(kd > 0);
}




bool Light::Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out)
{
    attenuation = attenuation * color;
    return true;
};