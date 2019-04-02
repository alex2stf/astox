/*
 * xjvm.cpp
 *
 *  Created on: 31 Jul 2017
 *      Author: alex
 */



#include <jni.h>       /* where everything is defined */

int main() {
  JavaVM *jvm;       /* denotes a Java VM */
  JNIEnv *env;       /* pointer to native method interface */
  JDK1_1InitArgs vm_args; /* JDK 1.1 VM initialization arguments */
  vm_args.version = 0x00010001; /* New in 1.1.2: VM version */
  /* Get the default initialization arguments and set the class
   * path */
  JNI_GetDefaultJavaVMInitArgs(&vm_args);
  vm_args.classpath = ...;
  /* load and initialize a Java VM, return a JNI interface
   * pointer in env */
  JNI_CreateJavaVM(&jvm, &env, &vm_args);
  /* invoke the Main.test method using the JNI */
  jclass cls = env->FindClass("Main");
  jmethodID mid = env->GetStaticMethodID(cls, "test", "(I)V");
  env->CallStaticVoidMethod(cls, mid, 100);
  /* We could have created an Object and called methods on it instead */
  /* We are done. */
  jvm->DestroyJavaVM();
}
