# TASOLLER configuration UI

modified firmware to add configuration GUI support. [forked from custom `host_aprom` firmware](https://gitea.tendokyu.moe/tasoller/host-aprom)

<img width="1408" height="979" alt="image" src="https://github.com/user-attachments/assets/f4d0e1c6-7d6d-4485-9f50-652f602b2ff5" />

## Force selected air sensors off

With the patched firmware and GUI, select sensors under **Force AIR off (unblocked)**, then
Apply to RAM. Selected sensors report connected/unblocked regardless of the beam;
other sensors and FN buttons keep working. Save persists the selection across
reboots; Reset clears it. Requires firmware/API 1.4 (capability bit 7).

Host-side firmware check (no device/toolchain required):
```sh
cc -std=c11 -Wall -Wextra -Werror -Isrc tests/air_off.c -o /tmp/air_off && /tmp/air_off
```
