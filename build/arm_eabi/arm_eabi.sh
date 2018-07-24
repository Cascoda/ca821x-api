mkdir out
cd out && cmake -DCMAKE_TOOLCHAIN_FILE=../arm_none_eabi.cmake ../../../
make
