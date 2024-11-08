### Add a device to the simulator

 * To add any device define the following in `device_tree.h`:
 * `SLASH_DT_<NAME>_BASE`  xxxx
 * `SLASH_DT_<NAME>_STRIDE`  xxxx
 * `SLASH_DT_<NAME>_ARGS`  xxxx

 * Finally add the device in the `splash_dev_tree[]` `device_tree.c` using `SLASH_PERIPHERAL_REG(<NAME>, <&api_list>)`
