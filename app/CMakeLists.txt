# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

PROJECT(OpenCV4Demo)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
include_directories(src/main/cpp/include)


#设置生成的so动态库最后输出的路径
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})


add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp)

add_library( # Sets the name of the library.
        image-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/image-lib.cpp)
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

#set(my_lib_path ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})
set(libs ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${libs}")

#set(libs ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})


#add_library(opencv_java4 SHARED IMPORTED)
#set_target_properties(opencv_java4 PROPERTIES
#        IMPORTED_LOCATION ${libs}/libopencv_java4.so)
#add_library(libopencv_calib3d STATIC IMPORTED)
#set_target_properties(libopencv_calib3d PROPERTIES IMPORTED_LOCATION
#        ${lib_build_DIR}/${ANDROID_ABI}/libopencv_calib3d.a)

find_library( # Sets the name of the path variable.
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

find_library( # Sets the name of the path variable.
        jnigraphics-lib
        jnigraphics  )
# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${jnigraphics-lib}
        )
target_link_libraries( # Specifies the target library.
        image-lib

        # Links the target library to the log library
        # included in the NDK.
        opencv_java4
        android
        ${jnigraphics-lib}
        ${log-lib}
        )