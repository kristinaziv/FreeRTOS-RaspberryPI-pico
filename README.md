Executable e-trice model for RPi Pico with FReeRTOS.
E-trice generated classses are in etrice-src folder, which are implicitly starting tasks, do synchronization based on model.
In src folder there is pure code written using Free-RTOS, manual usage of Queue-s and Semaphores for task synchronization, expilicit starting tasks with given priority.

To run each tasks, just uncomment corresponding line in CMakeLists.txt file.
#add_subdirectory(src)
#add_subdirectory(etrice-src/TemplateModel)
