#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h"
#include "tonemap.h"

#define CHANNELS 3
#define IMAGE_PATH "image.hdr"
#define OUT_IMAGE_PATH_FMT "out-%s.jpg"

float3 tonemap(int func, float3 color)
{
    switch (func)
    {
    case NONE:
        return color;
    case REINHARD:
        return tm_reinhard(color);
    case EXTENDED_REINHARD:
        return tm_extended_reinhard(color, 3.5f);
    case EXTENDED_REINHARD_LUMINANCE:
        return tm_extended_reinhard_lumincance(color, 3.5f);
    case REINHARD_JODIE:
        return tm_reinhard_jodie(color);
    case UNCHARTED2_FILMIC:
        return tm_uncharted2_filmic(color);
    case ACES_FITTED:
        return tm_aces_fitted(color);
    case ACES_APPROXIMATED:
        return tm_aces_approximated(color);
    }
}

void main()
{
    // load image
    int width, height, channels;
    const float *img = stbi_loadf(IMAGE_PATH, &width, &height, &channels, CHANNELS);
    if (img == NULL)
    {
        fprintf(stderr, "could not load the image: %s\n", strerror(errno));
        return;
    }
    if (channels != CHANNELS)
    {
        fprintf(stderr, "image must be in RGB format\n");
    }

    uint8_t *out = new uint8_t[width * height * CHANNELS];

    for (int tm = 0; tm < _TM_FUNC_COUNT; tm++)
    {

        // process image
        for (int i = 0; i < width * height; i++)
        {
            float3 color = float3(img[i * CHANNELS], img[i * CHANNELS + 1], img[i * CHANNELS + 2]);
            color = tonemap(tm, color);
            color = clamp(color, 0.0f, 1.0f);
            float3_to_rgb(color, &out[i * CHANNELS]);
        }

        // write image
        const char *tonemap_fn_name = tonemap_function_name(tm);
        size_t out_image_path_len = strlen(OUT_IMAGE_PATH_FMT) - 2 + strlen(tonemap_fn_name) + 1;
        char *out_image_path = (char *)malloc(out_image_path_len);
        snprintf(out_image_path, out_image_path_len, OUT_IMAGE_PATH_FMT, tonemap_fn_name);
        out_image_path[out_image_path_len - 1] = '\0';
        stbi_write_jpg(out_image_path, width, height, CHANNELS, out, 0);
    }

    stbi_image_free((void *)img);
    delete out;
}