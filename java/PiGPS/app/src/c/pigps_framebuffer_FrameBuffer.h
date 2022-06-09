/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class pigps_framebuffer_FrameBuffer */

#ifndef _Included_pigps_framebuffer_FrameBuffer
#define _Included_pigps_framebuffer_FrameBuffer
#ifdef __cplusplus
extern "C" {
#endif
#undef pigps_framebuffer_FrameBuffer_FPS
#define pigps_framebuffer_FrameBuffer_FPS 40L
/*
 * Class:     pigps_framebuffer_FrameBuffer
 * Method:    openDevice
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_pigps_framebuffer_FrameBuffer_openDevice
  (JNIEnv *, jobject, jstring);

/*
 * Class:     pigps_framebuffer_FrameBuffer
 * Method:    closeDevice
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_pigps_framebuffer_FrameBuffer_closeDevice
  (JNIEnv *, jobject, jlong);

/*
 * Class:     pigps_framebuffer_FrameBuffer
 * Method:    getDeviceWidth
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_pigps_framebuffer_FrameBuffer_getDeviceWidth
  (JNIEnv *, jobject, jlong);

/*
 * Class:     pigps_framebuffer_FrameBuffer
 * Method:    getDeviceHeight
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_pigps_framebuffer_FrameBuffer_getDeviceHeight
  (JNIEnv *, jobject, jlong);

/*
 * Class:     pigps_framebuffer_FrameBuffer
 * Method:    getDeviceBitsPerPixel
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_pigps_framebuffer_FrameBuffer_getDeviceBitsPerPixel
  (JNIEnv *, jobject, jlong);

/*
 * Class:     pigps_framebuffer_FrameBuffer
 * Method:    updateDeviceBuffer
 * Signature: (J[I)Z
 */
JNIEXPORT jboolean JNICALL Java_pigps_framebuffer_FrameBuffer_updateDeviceBuffer
  (JNIEnv *, jobject, jlong, jintArray);

#ifdef __cplusplus
}
#endif
#endif
