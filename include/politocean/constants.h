#include <stdint.h>
#include <string.h>

namespace politocean::constants {
namespace spi {
static const uint8_t kTrigger = 0xff;
}
namespace sensors {
static const uint8_t count = 7;
namespace id {
inline static const char *kTemperature = "temperature";
}
namespace value {
static const uint8_t kTemperature = 1;
}
const uint8_t map(const char *id) {
  if (strcmp(id, id::kTemperature) == 0)
    return value::kTemperature;

  return 0;
}
const char *map(uint8_t value) {
  switch (value) {
  case value::kTemperature:
    return id::kTemperature;
  }

  return NULL;
}
} // namespace sensors
} // namespace politocean::constants