#include "parseInk.h"

int parseInk(FILE* file, parseInk_addStroke_t addStroke, void* userParam)
{
  int strokeCount, strokeIndex;

  if (fscanf(file, "%d", &strokeCount) != 1)
    return EXIT_FAILURE;

  for (strokeIndex = 0; strokeIndex < strokeCount; ++strokeIndex)
  {
    int pointCount, pointIndex;
    struct { float x; float y; }* points;
    int retval;

    if (fscanf(file, "%d", &pointCount) != 1)
      return EXIT_FAILURE;

    points = malloc(pointCount * sizeof(*points));
    if (points == NULL)
      return EXIT_FAILURE;

    for (pointIndex = 0; pointIndex < pointCount; ++pointIndex)
    {
      if (fscanf(file, "%f %f", &points[pointIndex].x, &points[pointIndex].y) != 2)
      {
        free(points);
        return EXIT_FAILURE;
      }
    }

    retval = addStroke(
        &points->x, sizeof(*points),
        &points->y, sizeof(*points),
        pointCount,
        userParam);

    free(points);

    if (retval != EXIT_SUCCESS)
      return retval;
  }

  return EXIT_SUCCESS;
}
