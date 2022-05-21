@echo off

echo select vdisk file="E:\System\UEFI.vhd" >ache

echo detach vdisk>>ache

echo list disk>>ache

diskpart /s ache
