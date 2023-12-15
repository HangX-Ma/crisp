#pragma once

#define CRISP_VER_MAJOR 1
#define CRISP_VER_MINOR 0
#define CRISP_VER_PATCH 0

#define CRISP_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define CRISP_VERSION CRISP_TO_VERSION(CRISP_VER_MAJOR, CRISP_VER_MINOR, CRISP_VER_PATCH)