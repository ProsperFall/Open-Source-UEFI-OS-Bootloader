/*
    Maximum: 2
    File Source:
--------------------------------
boot_record: id0
boot_device_guid: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
## file_system default = fat16/fat32
device_file_system: fat32, fat16
system_name: Baka OS
file: "/system/kernel","/system/initializer"
file_base_address: 0xFFFF_C000_0000_0000, 0xFF00_0000_0000_0000
file_real_address: random, random
call_table: 0xFF00_0000_C000_0000
call: "initd"
##use boot_record end id0, start new record or not
boot_record: id1
--------------------------------

    File Output:
        Boot.cfg:
--------------------------------

--------------------------------
        BootConfigBin:
--------------------------------

--------------------------------

*/