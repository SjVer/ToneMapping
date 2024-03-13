#include "color.h"

// see http://web.archive.org/web/20191226154550/http://cs.columbia.edu/CAVE/software/softlib/dorf.php
extern float known_irradiances[];
extern float known_intensities[];

extern size_t find_closest(float arr[], float value);

float camera_get_intensity(float irradiance, float iso)
{
    // map irradiance to [0, 1]
    irradiance = clamp(irradiance, 0.f, 1.f);
    irradiance /= iso;

    // find closest matching known irradiance value (lower bound)
    size_t idx = find_closest(known_irradiances, irradiance);

    // get the two closest known intensity values
    float intensity1 = known_intensities[idx];
    float intensity2 = known_intensities[idx + 1];

    // find the interpolation factor between the two closest known irradiance values
    float irradiance1 = known_irradiances[idx];
    float irradiance2 = known_irradiances[idx + 1];
    float t = (irradiance - irradiance1) / (irradiance2 - irradiance1);

    // interpolate between the two bordering intensities
    float intensity = lerp(intensity1, intensity2, t);
    
    return clamp(intensity, 0.f, 1.f);
}

float3 tmo_real_camera_response(float3 C, float iso)
{
    float r = camera_get_intensity(C.r, iso);
    float g = camera_get_intensity(C.g, iso);
    float b = camera_get_intensity(C.b, iso);
    return float3(r, g, b);
}

