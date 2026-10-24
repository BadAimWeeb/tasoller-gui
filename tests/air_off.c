// Run: cc -std=c11 -Wall -Wextra -Werror -Isrc tests/air_off.c -o /tmp/air_off && /tmp/air_off
#include <assert.h>
#include <stddef.h>
#include "config_api.h"
#include "pins.h"

int main(void) {
    assert(sizeof(flash_t) == 380);
    assert(offsetof(flash_t, u8AirOffMask) == 379);
    flash_t config = { .u8Sens = 8 };
    uint8_t data[CONFIG_API_SIZE];
    for (unsigned mask = 0; mask < 256; mask++) {
        Config_Encode(&config, data);
        data[3] = mask;
        assert(Config_Validate(data, sizeof data) == (mask < 64 ? CONFIG_OK : CONFIG_VALUE));
        if (mask >= 64) continue;
        Config_Apply(&config, data);
        Config_Encode(&config, data);
        assert(data[3] == mask);
        for (unsigned buttons = 0; buttons < 256; buttons++) {
            uint8_t reported = Digital_ApplyAirOff(buttons, mask);
            assert((reported & 3) == (buttons & 3)); // FN unchanged
            for (unsigned air = 0; air < 6; air++) {
                unsigned bit = 1u << (air + 2);
                assert((reported & bit) == ((mask & (1u << air)) ? 0 : (buttons & bit)));
                // IO4 inverts the internal bit: selected sensors always report unblocked.
                if (mask & (1u << air)) assert(!(reported & bit));
            }
        }
    }
    // A v5 -> v6 storage migration must preserve all independent profiles.
    config.u32Magic = 0x54617305;
    Keymap_Defaults(config.u8Keymap);
    Keymap_ProfilesInit(&config);
    memset(config.u8KeymapProfiles[1], 7, 32);
    Keymap_LoadProfiles(&config);
    assert(config.u8KeymapProfiles[1][0] == 7);
    config.u32Magic = 0x54617306;
    Keymap_LoadProfiles(&config);
    assert(config.u8KeymapProfiles[1][0] == 7);
    return 0;
}
