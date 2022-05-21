@echo off

echo select vdisk file="E:\System\UEFI.vhd" >ache

echo attach vdisk>>ache

echo list disk>>ache

diskpart /s ache
