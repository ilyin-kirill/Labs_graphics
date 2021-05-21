#include <iostream>
#include <vector>
#include <random>

#include "LiteMath.h"
#include "Geometry.h"
#include "Camera.h"

using namespace HydraLiteMath;

struct Scene {
    std::vector<std::shared_ptr<GeoObject>> geoObjects;
};

void RenderScene(uint32_t w, uint32_t h, uint32_t num_samples, const Scene& scene, const Camera& cam, const std::string& filename)
{
    auto  background_color = float3(0.2f, 0.1f, 0.2f);
    auto  film = std::make_unique<Film>(w, h, num_samples);
    auto  tracer = std::make_unique<SimpleRT>(16, background_color);
    float invWidth = 1.0f / float(w);
    float invHeight = 1.0f / float(h);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            float3 pixel_color = float3(0.0f, 0.0f, 0.0f);

            for (int s = 0; s < num_samples; ++s)
            {
                Ray ray = cam.genRay(w, h, x, h - y); //���������� ��� �� ������ ����� ������� �������
                pixel_color += tracer->TraceRay(ray, scene.geoObjects, 0); //���������� ��� � �������� ����
            }
            pixel_color /= film->num_samples;      // ��������� ���������� �����
            pixel_color *= cam.getExposureTime();  // �������� �� ����� �������������� ������� - �������� ����������� ������
            film->SetPixelColor(x, y, pixel_color); //���������� ���� �� ����������� ������
        }
    }
    film->SaveImagePPM(filename); //��������� ��������
}

void create_scene()
{
    Scene myScene;

    float3        eye(0.0f, 2.0f, 25.0f);
    float3        lookat(0.0f, 2.0f, 0.0f);
    float3        up(0.0f, 1.0f, 0.0f);
    float         field_of_view = 90.0f;
    unsigned int  w = 1920;
    unsigned int  h = 1080;
    Camera        cam(eye, lookat, up, field_of_view, float(w) / float(h));

    auto plane = std::make_shared<Plane>(float3(+0.0f, -1.0f, +0.0f), float3(0.0f, 1.0f, 0.0f), new IdealMirror(float3(0.4f, 0.4f, 0.5f)));
    myScene.geoObjects.push_back(plane);

    auto sphere1 = std::make_shared<Sphere>(float3(+0.5f, +5.0f, +2.65f), 4, new IdealMirror(float3(1.0f, 1.0f, 1.0f)));
    myScene.geoObjects.push_back(sphere1);

    auto sphere2 = std::make_shared<Sphere>(float3(-5.5f, +3.0f, -13.5f), 5, new Diffuse(float3(0.8f, 0.5f, 0.7f)));
    myScene.geoObjects.push_back(sphere2);

    auto sphere3 = std::make_shared<Sphere>(float3(6.5f, 2.0f, +8.5f), 1.5, new Diffuse(float3(0.0f, 0.0f, 1.0f)));
    myScene.geoObjects.push_back(sphere3);

    auto parallel = std::make_shared<Parallel>(float3(-12.0f, -0.0f, +1.0f), float3(-11.0f, +3.0f, +4.0f), new IdealMirror(float3(1.0f, 0.5f, 0.5f)));
    myScene.geoObjects.push_back(parallel);

    auto triangle1 = std::make_shared<Triangle>(float3(-1.0f, 5.0f, +10.0f), float3(+4.0f, +11.0f, +11.0f), float3(-5.0f, +8.0f, +9.0f), new Diffuse(float3(0.4f, 0.0f, 0.3f)));
    myScene.geoObjects.push_back(triangle1);

    auto square = std::make_shared<Square>(float3(-6.0f, +4.0f, -7.0f), float3(-3.0f, +6.0f, -7.0f), float3(-4.0f, +4.0f, -8.0f), float3(-6.0f, +8.0f, -8.0f), new IdealMirror(float3(0.0f, 0.8f, 0.3f)));
    myScene.geoObjects.push_back(square);

    RenderScene(w, h, 1, myScene, cam, "output");
}

int main()
{
    create_scene();

    return 0;
}
