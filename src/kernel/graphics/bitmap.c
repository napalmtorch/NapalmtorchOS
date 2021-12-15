#include <kernel/graphics/bitmap.h>
#include <kernel/core/kernel.h>

image_t image_create(uint32_t w, uint32_t h)
{
    image_t image;
    image.width = w > 0 ? w : 1;
    image.height = h > 0 ? h : 1;
    image.size = image.width * image.height * 4;
    image.data = tcalloc(image.width * image.height * 4, MEMSTATE_BITMAP);
    return image;
}