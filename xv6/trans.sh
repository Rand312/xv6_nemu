# make clean
# make 
cp -af ./kernel ../nemu/kernel-xv6
cp -af ./fs.img ../nemu/fs.img
# cp -af ./kernel.sym ../nemu/kernel.sym
# mv ../nemu/kernel_ entry
objcopy -S -O binary ../nemu/kernel-xv6 ../nemu/entry-xv6
cp -af ../nemu/entry-xv6 ../nemu/entry

