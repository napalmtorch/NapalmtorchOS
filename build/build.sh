# vars
cmsgl="GCC   :   '"
nmsgl="NASM  :   '"
cmsgm="' -> '"
cmsge="'"
outdir="bin/objs/"
cdir=""
inf=""
outf=""

# Clear the console
clear

# Setup build directory
rm -r "bin"
mkdir "bin"
mkdir "bin/objs"

# boot.asm
cdir="src/boot/" outdir="bin/" inf="boot.asm" outf="boot.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# irqs.asm
cdir="src/kernel/hardware/interrupts/" outdir="bin/objs/" inf="irqs.asm" outf="irqs.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
outdir="bin/objs/"

# tswitch.asm
cdir="src/kernel/system/" outdir="bin/objs/" inf="tswitch.asm" outf="tswitch.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
outdir="bin/objs/"

# realmode.asm
cdir="src/kernel/hardware/common/" outdir="bin/objs/" inf="realmode.asm" outf="realmode.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
outdir="bin/objs/"


# ----------------------------------------------- CORE --------------------------------------------------------------
cdir="src/kernel/core/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- RUNTIME --------------------------------------------------------------
cdir="src/kernel/runtime/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- FILESYS --------------------------------------------------------------
cdir="src/kernel/filesys/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- GRAPHICS ------------------------------------------------------------
cdir="src/kernel/graphics/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- SYSTEM ------------------------------------------------------------
cdir="src/kernel/system/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done


# ----------------------------------------------- SYSTEM/COMMANDS ------------------------------------------------------------
cdir="src/kernel/system/commands/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- SYSTEM/COMMANDS/FILESYS ------------------------------------------------------------
cdir="src/kernel/system/commands/filesys/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- LIB ------------------------------------------------------------
cdir="src/lib/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HARDWARE/COMMON ----------------------------------------------------------
cdir="src/kernel/hardware/common/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HARDWARE/INTERRUPTS ------------------------------------------------
cdir="src/kernel/hardware/interrupts/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HARDWARE/DEVICES/VIDEO ------------------------------------------------
cdir="src/kernel/hardware/devices/video/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HARDWARE/DEVICES/INPUT ------------------------------------------------
cdir="src/kernel/hardware/devices/input/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HARDWARE/DEVICES/STORAGE ------------------------------------------------
cdir="src/kernel/hardware/devices/storage/"
for file in $cdir*.c
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-gcc -w -Iinclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -Wall -Wextra
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done


# Link message
linkmsg="Linking boot.o "
for file in bin/objs/*.o
do
    inf="$(basename $file)"
    linkmsg="$linkmsg$inf "
done
echo "$linkmsg"

# Link all files
cd 'bin/objs'
i686-elf-ld -T '../../build/linker.ld' -o '../kernel.bin' '../boot.o' *.o
cd '../../'

# Create ISO image
mkdir -p 'bin/isodir/boot/grub'
cp 'bin/kernel.bin' 'bin/isodir/boot/kernel.bin'
cp 'build/grub.cfg' 'bin/isodir/boot/grub/grub.cfg'
grub-mkrescue -o  'napalm_os.iso' 'bin/isodir'
cp 'napalm_os.iso' 'napalm_os_flp.img'

# compile user-mode applications
i686-elf-gcc "disk/test.c" -o "disk/program.app" -nostdlib -ffreestanding -Wall -Wextra

#nasm -felf32 "disk/entry.asm" -o "disk/entry.o"

#i686-elf-ld -T "disk/linker.ld" -o "disk/program.app" "disk/entry.o" "disk/test.o"
#i686-elf-ld -T "disk/linker.ld" -o "disk/program.app" "disk/test.o" "disk/start.o"


# create disk image
./fsmgr "makedisk"

# Run QEMU instance of operating system
qemu-system-i386 -m 1024M -vga std -hda 'hdd.img' -cdrom 'napalm_os.iso' -serial stdio -boot d -soundhw ac97 -enable-kvm -rtc base=localtime -cpu host
