LOCAL_PATH := $(call my-dir)

# dng sdk shared library for target
# ========================================================

include $(CLEAR_VARS)

dng_sdk_files := \
    source/dng_1d_function.cpp \
    source/dng_1d_table.cpp \
    source/dng_abort_sniffer.cpp \
    source/dng_area_task.cpp \
    source/dng_bad_pixels.cpp \
    source/dng_bottlenecks.cpp \
    source/dng_camera_profile.cpp \
    source/dng_color_space.cpp \
    source/dng_color_spec.cpp \
    source/dng_date_time.cpp \
    source/dng_exceptions.cpp \
    source/dng_exif.cpp \
    source/dng_file_stream.cpp \
    source/dng_filter_task.cpp \
    source/dng_fingerprint.cpp \
    source/dng_gain_map.cpp \
    source/dng_globals.cpp \
    source/dng_host.cpp \
    source/dng_hue_sat_map.cpp \
    source/dng_ifd.cpp \
    source/dng_image.cpp \
    source/dng_image_writer.cpp \
    source/dng_info.cpp \
    source/dng_iptc.cpp \
    source/dng_jpeg_image.cpp \
    source/dng_jpeg_memory_source.cpp \
    source/dng_lens_correction.cpp \
    source/dng_linearization_info.cpp \
    source/dng_lossless_jpeg.cpp \
    source/dng_matrix.cpp \
    source/dng_memory.cpp \
    source/dng_memory_stream.cpp \
    source/dng_misc_opcodes.cpp \
    source/dng_mosaic_info.cpp \
    source/dng_mutex.cpp \
    source/dng_negative.cpp \
    source/dng_opcode_list.cpp \
    source/dng_opcodes.cpp \
    source/dng_orientation.cpp \
    source/dng_parse_utils.cpp \
    source/dng_pixel_buffer.cpp \
    source/dng_point.cpp \
    source/dng_preview.cpp \
    source/dng_pthread.cpp \
    source/dng_rational.cpp \
    source/dng_read_image.cpp \
    source/dng_rect.cpp \
    source/dng_ref_counted_block.cpp \
    source/dng_reference.cpp \
    source/dng_render.cpp \
    source/dng_resample.cpp \
    source/dng_safe_arithmetic.cpp \
    source/dng_shared.cpp \
    source/dng_simple_image.cpp \
    source/dng_spline.cpp \
    source/dng_stream.cpp \
    source/dng_string.cpp \
    source/dng_string_list.cpp \
    source/dng_tag_types.cpp \
    source/dng_temperature.cpp \
    source/dng_tile_iterator.cpp \
    source/dng_tone_curve.cpp \
    source/dng_utils.cpp \
    source/dng_xy_coord.cpp \
    source/dng_xmp.cpp

LOCAL_MODULE := libdng_sdk
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(dng_sdk_files)

LOCAL_CFLAGS := -DUNIX_ENV=1 -DqDNGBigEndian=0 -DqDNGThreadSafe=1 -DqDNGUseLibJPEG=1 -DqDNGUseXMP=0 -DqDNGValidate=0 -DqDNGValidateTarget=1 -DqAndroid=1 -fexceptions -Wsign-compare -Wno-reorder -Wframe-larger-than=20000 -frtti

# Ignore unused parameters.
LOCAL_CFLAGS += -Wno-unused-parameter
# Some integral return types are annotated with "const."
LOCAL_CFLAGS += -Wno-ignored-qualifiers

LOCAL_CLANG := true
LOCAL_SANITIZE := unsigned-integer-overflow signed-integer-overflow

LOCAL_SHARED_LIBRARIES := libz libjpeg

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/source

include $(BUILD_SHARED_LIBRARY)

# dng sdk unittests for target
# ========================================================

include $(CLEAR_VARS)

LOCAL_MODULE := dng_validate
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := \
    $(dng_sdk_files) \
    source/dng_validate.cpp

LOCAL_CFLAGS := -DUNIX_ENV=1 -DqDNGBigEndian=0 -DqDNGThreadSafe=1 -DqDNGUseLibJPEG=1 -DqDNGUseXMP=0 -DqDNGValidate=1 -DqDNGValidateTarget=1 -DqAndroid=1 -fexceptions -Wsign-compare -Wno-reorder -Wframe-larger-than=20000 -frtti

LOCAL_SHARED_LIBRARIES := libz libjpeg

include $(BUILD_EXECUTABLE)
