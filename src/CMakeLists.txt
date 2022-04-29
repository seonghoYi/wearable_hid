add_executable(${PRJ_NAME}
  main.cpp
  
  ap/ap.cpp
  bsp/bsp.c
  common/core/qbuffer.c
  hw/hw.c
  hw/driver/gpio.c
  hw/driver/led.c
  hw/driver/uart.c
  hw/driver/i2c.c

  hw/driver/cli.c

  hw/driver/imu/mpu6050.cpp
  hw/driver/imu/MadgwickAHRS.cpp
  hw/driver/imu/imu.cpp
  hw/driver/imu.cpp

)


target_include_directories(${PRJ_NAME} PUBLIC
  ap
  bsp
  common
  common/core
  common/hw/include
  hw
  hw/include/imu

)

target_link_libraries(${PRJ_NAME}
  pico_stdlib
  pico_multicore
  hardware_clocks
  hardware_pll
  hardware_gpio
  hardware_i2c
)


pico_add_extra_outputs(${PRJ_NAME})
pico_enable_stdio_usb(${PRJ_NAME}  1)
pico_enable_stdio_uart(${PRJ_NAME} 0)