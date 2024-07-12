#include "main.h"
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <android/log.h>
#include <cstring>
#include <thread>
#include "zygisk.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "MyModule", __VA_ARGS__)


// -- ZYGISK

class MyModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        // Use JNI to fetch our process name
        const char *processName = env->GetStringUTFChars(args->nice_name, nullptr);
        const char *appDataDir = env->GetStringUTFChars(args->app_data_dir, nullptr);
        preSpecialize(processName, appDataDir);
        env->ReleaseStringUTFChars(args->nice_name, processName);
        env->ReleaseStringUTFChars(args->app_data_dir, appDataDir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (createThread) {
            std::thread hackThread(inject, targetProcessName);
            LOGD("Thread created");
        }
        
    }

private:
    Api *api;
    JNIEnv *env;
    bool createThread = false;


    void preSpecialize(const char *processName, const char *appDataDir) {
        
        // Check is the current process is match with targeted process
        if (strcmp(processName, targetProcessName) == 0) {
            LOGD("Success, setup a thread");
            createThread = true;
        } else {
            LOGD("Skip, process unknown");
        }
    }

};

// Register our module class
REGISTER_ZYGISK_MODULE(MyModule)

// -- end ZYGISK

// -- HOOK IMGUI

// INPUT HANDLER

// EGLSWAPBUFFER HANDLER


// INJECT OUR MENU
void inject(const char *targetProcessName) {
    // HOOK SYMBOL
}

// -- END HOOK IMGUI