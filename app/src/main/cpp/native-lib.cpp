#include <jni.h>
#include <string>
#include <android/sensor.h>
#include <android/looper.h>
#include <stdio.h>
#include <unistd.h>
#include <android/log.h>
#include <time.h>
#include <iostream>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TestJNIActivity", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "DebugJNIActivity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "ErrorJNIActivity", __VA_ARGS__))
#define LOOPER_ID 1
#define SAMP_PER_SEC 100

 jobject g_object;
 jmethodID g_method;
 JNIEnv *g_env;

 JavaVM *jvm;





ASensorEventQueue* sensorEventQueue;
int accCounter = 0;
int64_t lastAccTime = 0;

int gyroCounter = 0;
int64_t lastGyroTime = 0;

int magCounter = 0;
int64_t lastMagTime = 0;

float cAcc[3];

static int get_sensor_events(int fd, int events, void* data);
struct tm* start;
struct tm* finish;

extern "C"
jstring
Java_netlab_fakturk_nativeacc_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject thiz) {
    std::string hello = "Hello from C++";
//    LOGD("hebele");

    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_netlab_fakturk_nativeacc_MainActivity_setAccXFromC(
        JNIEnv *env,
        jobject thiz, jfloat a)
{
    jclass clazz = (*env).GetObjectClass(thiz);
    jmethodID setAccX = env->GetMethodID(clazz, "setAccX", "(F)V");

    env->CallVoidMethod(thiz,setAccX,4.0);
}

extern "C"
JNIEXPORT void JNICALL
Java_netlab_fakturk_nativeacc_MainActivity_setAccYFromC(
        JNIEnv *env,
        jobject thiz, jfloat a)
{
    jclass clazz = (*env).GetObjectClass(thiz);
    jmethodID setAccY = env->GetMethodID(clazz, "setAccY", "(F)V");

    env->CallVoidMethod(thiz,setAccY,5.0);
}

extern "C"
JNIEXPORT void JNICALL
Java_netlab_fakturk_nativeacc_MainActivity_setAccZFromC(
        JNIEnv *env,
        jobject thiz, jfloat a)
{
    jclass clazz = (*env).GetObjectClass(thiz);
    jmethodID setAccZ = env->GetMethodID(clazz, "setAccZ", "(F)V");

    env->CallVoidMethod(thiz,setAccZ,6.0);
}

extern "C"
jfloatArray Java_netlab_fakturk_nativeacc_MainActivity_getAcc(JNIEnv *env, jobject thiz)
{
    jfloatArray acc = (*env).NewFloatArray(3);
    float cAcc[3];
    cAcc[0]=0;
    cAcc[1]=1;
    cAcc[2]=2;
    (*env).SetFloatArrayRegion(acc,0,3,cAcc);
    return acc;
}

extern "C"
void Java_netlab_fakturk_nativeacc_MainActivity_sensorValue(JNIEnv* env, jobject thiz)
{
    ASensorEvent event;
    int events, ident;
    ASensorManager* sensorManager;
    const ASensor* accSensor;
//    const ASensor* gyroSensor;
//    const ASensor* magSensor;
    void* sensor_data = malloc(1000);



    LOGI("sensorValue() - ALooper_forThread()");

// Identify the looper associated with the calling thread, or create one if it does not exist.
// A looper is a message loop for a thread and will handle the sensor event callbacks.

    ALooper* looper = ALooper_forThread();

    if(looper == NULL)
    {
        looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    }

    sensorManager = ASensorManager_getInstance();

    accSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
//    gyroSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_GYROSCOPE);
//    magSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_MAGNETIC_FIELD);

        jclass clazz = (*env).GetObjectClass(thiz);
    if (clazz == NULL) {
        std::cout << "Failed to find class" << std::endl;
    }

    jmethodID setAccX = env->GetMethodID(clazz, "setAccX", "(F)V");
    g_method = setAccX;
    if (g_method == NULL) {
        std::cout << "Unable to get method ref" << std::endl;
    }
    g_object = env->NewGlobalRef(thiz);
    g_env = env;

    (*env).GetJavaVM(&jvm);


    g_env->CallVoidMethod(g_object,g_method,4.0);

    sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, 3, get_sensor_events, sensor_data);
    g_env->CallVoidMethod(g_object,g_method,5.0);

    ASensorEventQueue_enableSensor(sensorEventQueue, accSensor);
//    ASensorEventQueue_enableSensor(sensorEventQueue, gyroSensor);
//    ASensorEventQueue_enableSensor(sensorEventQueue, magSensor);

    //Sampling rate: 100Hz
    int a = ASensor_getMinDelay(accSensor);
//    int b = ASensor_getMinDelay(gyroSensor);
//    int c = ASensor_getMinDelay(magSensor);
//    LOGI("min-delay: %d, %d, %d",a,b,c);

//    typedef int (*ALooper_callbackFunc)(int fd, int events, void* data);

    LOGI("min-delay: %d",a);
    ASensorEventQueue_setEventRate(sensorEventQueue, accSensor, 100000);

//    ASensorEventQueue_setEventRate(sensorEventQueue, gyroSensor, 100000);
//    ASensorEventQueue_setEventRate(sensorEventQueue, magSensor, 100000);
//    std::cout<<"fatih"<<std::endl;
//    LOGD("akturk");

    LOGI("sensorValue() - START");



}




static int get_sensor_events(int fd, int events, void* data) {
//    LOGI("Beginning of get sensors events");
    ASensorEvent event;
    //ASensorEventQueue* sensorEventQueue;
    JNIEnv *env ;
    int getEnvStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        std::cout << "GetEnv: not attached" << std::endl;
        if (jvm->AttachCurrentThread((JNIEnv **) (void **) &env, NULL) != 0) {
            std::cout << "Failed to attach" << std::endl;
        }
    } else if (getEnvStat == JNI_OK) {
        //
    } else if (getEnvStat == JNI_EVERSION) {
        std::cout << "GetEnv: version not supported" << std::endl;
    }

    env->CallVoidMethod(g_object, g_method, 7.0);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
    }

//    jvm->DetachCurrentThread();



//    (*jvm).AttachCurrentThread(&env, NULL);
    jfloat jfloat1 = 6.0;

//    LOGI("before clazz");
//    jclass clazz = (*env).GetObjectClass(g_object);
//    LOGI("before setAccX");
//
//    jmethodID setAccX = env->GetMethodID(c, "setAccX", "(F)V");
//    env->ExceptionClear();
//    env->CallVoidMethod(g_object,g_method,jfloat1);

    while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0) {
        if(event.type == ASENSOR_TYPE_ACCELEROMETER) {
//            LOGI("inside if for acc");
            cAcc[0] = event.acceleration.x;
            cAcc[1] = event.acceleration.y;
            cAcc[2] = event.acceleration.z;


//            g_env.CallVoidMethod(g_object,g_method,5.0);

//            LOGI("after ");
//            if(g_env->ExceptionOccurred()){
//                //panic! Light fires! The British are coming!!!
//                LOGI("OMG");
//                g_env->ExceptionClear();
//            }


//            LOGI("accl(x,y,z,t): %f %f %f %lld", event.acceleration.x, event.acceleration.y, event.acceleration.z, event.timestamp);
//            if(accCounter == 0 || accCounter == 1000)
//            {
//                LOGI("Acc-Time: %lld (%f)", event.timestamp,((double)(event.timestamp-lastAccTime))/1000000000.0);
                lastAccTime = event.timestamp;
//                accCounter = 0;
//            }

//            accCounter++;
        }
        else if(event.type == ASENSOR_TYPE_GYROSCOPE) {
            //LOGI("accl(x,y,z,t): %f %f %f %lld", event.acceleration.x, event.acceleration.y, event.acceleration.z, event.timestamp);
            if(gyroCounter == 0 || gyroCounter == 1000)
            {

                LOGI("Gyro-Time: %lld (%f)", event.timestamp,((double)(event.timestamp-lastGyroTime))/1000000000.0);
                lastGyroTime = event.timestamp;
                gyroCounter = 0;
            }

            gyroCounter++;
        }
        else if(event.type == ASENSOR_TYPE_MAGNETIC_FIELD) {
            //LOGI("accl(x,y,z,t): %f %f %f %lld", event.acceleration.x, event.acceleration.y, event.acceleration.z, event.timestamp);
            if(magCounter == 0 || magCounter == 1000)
            {
                LOGI("Mag-Time: %lld (%f)", event.timestamp,((double)(event.timestamp-lastMagTime))/1000000000.0);
                lastMagTime = event.timestamp;
                magCounter = 0;
            }

            magCounter++;
        }

    }
    //should return 1 to continue receiving callbacks, or 0 to unregister
    return 1;
}