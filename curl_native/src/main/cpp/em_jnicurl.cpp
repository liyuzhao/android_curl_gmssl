#include <string>
#include "android_utils.h"
#include "com_easemob_emssl_EMJniCurl.h"
#include "manager/httpmanager.h"
#include "sha.h"
#include "aes_cbc.h"


JavaVM *g_jvm;
jobject g_obj = nullptr;

class JNIEnvPtr {
public:
    JNIEnvPtr() : env_{nullptr}, need_detach_{false} {
        if (g_jvm->GetEnv((void**) &env_, JNI_VERSION_1_6) ==
            JNI_EDETACHED) {
            g_jvm->AttachCurrentThread(&env_, nullptr);
            need_detach_ = true;
        }
    }

    ~JNIEnvPtr() {
        if (need_detach_) {
            g_jvm->DetachCurrentThread();
        }
    }

    JNIEnv* operator->() {
        return env_;
    }

private:
    JNIEnvPtr(const JNIEnvPtr&) = delete;
    JNIEnvPtr& operator=(const JNIEnvPtr&) = delete;

private:
    JNIEnv* env_;
    bool need_detach_;
};




jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_jvm = vm;
    return JNI_VERSION_1_6;
}

static void GlobalCallBackFunc(int result, const std::string &respones, float persent, size_t seq,
                               int errcode, void *extra) {
//    LOGE("%s", respones.c_str());

    JNIEnvPtr jnienv_holder;

//    JNIEnv *env = NULL;
//    if (g_jvm) {
//        g_jvm->AttachCurrentThread(&env, NULL);
//    }

    if (g_obj) {
        jclass clazz = jnienv_holder->GetObjectClass(g_obj);
        jmethodID method = jnienv_holder->GetMethodID(clazz, "callBack", "(ILjava/lang/String;FII)V");
        jstring jrespones = nullptr;
        try {
            jrespones = jnienv_holder->NewStringUTF(respones.c_str());
        } catch (...) {

        }
        jnienv_holder->CallVoidMethod(g_obj, method, result, jrespones, persent, seq, errcode);
    }


//    if (g_jvm) {
//        g_jvm->DetachCurrentThread();
//    }
}



static void GlobalCookieCallBackFunc(int result, const std::string &str_cookie, size_t seq,
                               void *extra) {
//    LOGE("cookie:%s", str_cookie.c_str());

    JNIEnvPtr jnienv_holder;

//    JNIEnv *env = NULL;
//    if (g_jvm) {
//        g_jvm->AttachCurrentThread(&env, NULL);
//    }

    if (g_obj) {
        jclass clazz = jnienv_holder->GetObjectClass(g_obj);
        jmethodID method = jnienv_holder->GetMethodID(clazz, "cookieCallBack", "(ILjava/lang/String;I)V");
        jstring jrespones = nullptr;
        try {
            jrespones = jnienv_holder->NewStringUTF(str_cookie.c_str());
        } catch (...) {

        }
        jnienv_holder->CallVoidMethod(g_obj, method, result, jrespones, seq);
    }


//    if (g_jvm) {
//        g_jvm->DetachCurrentThread();
//    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_init
        (JNIEnv *env, jclass type, jint threadPoolSize, jobject callBack) {
    HttpManager::init(threadPoolSize);
    if (callBack) {
        if (g_obj) {
            env->DeleteGlobalRef(g_obj);
        }
        g_obj = env->NewGlobalRef(callBack);
    }
}



extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_unInit
        (JNIEnv *env, jclass type) {
    HttpManager::uninit();
    if (g_obj) {
        env->DeleteGlobalRef(g_obj);
        g_obj = NULL;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_addBasicHeader
        (JNIEnv *env, jclass type, jstring strHash_, jstring strKey_, jstring strValue_) {
    const char *strHash = nullptr;
    const char *strKey = nullptr;
    const char *strValue = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strKey_) {
        strKey = env->GetStringUTFChars(strKey_, 0);
    }
    if (strValue_) {
        strValue = env->GetStringUTFChars(strValue_, 0);
    }

    if (strHash && strKey && strValue) {
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->add_basic_headers(strKey, strValue);
    }

    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }

    if (strKey_ && strKey) {
        env->ReleaseStringUTFChars(strKey_, strKey);
    }

    if (strValue_ && strValue) {
        env->ReleaseStringUTFChars(strValue_, strValue);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_addBasicURLParam
        (JNIEnv *env, jclass type, jstring strHash_, jstring strKey_, jstring strValue_) {
    const char *strHash = nullptr;
    const char *strKey = nullptr;
    const char *strValue = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strKey_) {
        strKey = env->GetStringUTFChars(strKey_, 0);
    }
    if (strValue_) {
        strValue = env->GetStringUTFChars(strValue_, 0);
    }
    if (strHash && strKey && strValue) {
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->add_basic_url_params(strKey, strValue);
    }
    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strKey_ && strKey) {
        env->ReleaseStringUTFChars(strKey_, strKey);
    }
    if (strValue_ && strValue) {
        env->ReleaseStringUTFChars(strValue_, strValue);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_setHost
        (JNIEnv *env, jclass type, jstring strHash_, jstring strHost_) {
    const char *strHash = nullptr;
    const char *strHost = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strHost_) {
        strHost = env->GetStringUTFChars(strHost_, 0);
    }
    if (strHash && strHost) {
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->set_host(strHost);
    }
    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strHost_ && strHost) {
        env->ReleaseStringUTFChars(strHost_, strHost);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_setCertPath
        (JNIEnv *env, jclass type, jstring strHash_, jstring strCertPath_) {
    const char *strHash = nullptr;
    const char *strCertPath = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strCertPath_) {
        strCertPath = env->GetStringUTFChars(strCertPath_, 0);
    }
    if (strHash && strCertPath) {
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->set_cert_path(strCertPath);
    }
    if(strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if(strCertPath_ && strCertPath) {
        env->ReleaseStringUTFChars(strCertPath_, strCertPath);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_setSSLCertPath(JNIEnv *env, jclass type, jstring strHash_, jstring strSSLCertPath_, jstring strSSLKeyPath_,
                                                jstring strSSLKeyPasswd_) {
    const char *strHash = nullptr;
    const char *strSSLCertPath = nullptr;
    const char *strSSLKeyPath = nullptr;
    const char *strSSLKeyPasswd = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strSSLCertPath_) {
        strSSLCertPath = env->GetStringUTFChars(strSSLCertPath_, 0);
    }
    if(strSSLKeyPath_) {
        strSSLKeyPath = env->GetStringUTFChars(strSSLKeyPath_, 0);
    }
    if(strSSLKeyPasswd_) {
        strSSLKeyPasswd = env->GetStringUTFChars(strSSLKeyPasswd_, 0);
    }

    if (strHash && strSSLCertPath && strSSLKeyPath && strSSLKeyPasswd) {
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->set_ssl_cert_path(strSSLCertPath, strSSLKeyPath, strSSLKeyPasswd);
    }
    if(strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if(strSSLCertPath_ && strSSLCertPath) {
        env->ReleaseStringUTFChars(strSSLCertPath_, strSSLCertPath);
    }
    if (strSSLKeyPath_ && strSSLKeyPath){
        env->ReleaseStringUTFChars(strSSLKeyPath_, strSSLKeyPath);
    }
    if(strSSLKeyPasswd_ && strSSLKeyPasswd){
        env->ReleaseStringUTFChars(strSSLKeyPasswd_, strSSLKeyPasswd);
    }

}
//set_ssl_cert_path


extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_setProxy
        (JNIEnv *env, jclass type, jstring strHash_, jstring proxy_) {
    const char *strHash = nullptr;
    const char *proxy = nullptr;
    if(strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if(proxy_) {
        proxy = env->GetStringUTFChars(proxy_, 0);
    }
    if (strHash && proxy) {
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->set_cert_path(proxy);
    }
    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (proxy_ && proxy) {
        env->ReleaseStringUTFChars(proxy_, proxy);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_setCookie
        (JNIEnv *env, jclass type, jstring strHash_, jstring strCookie_) {
    const char *strHash = nullptr;
    const char *strCookie = nullptr;
    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if(strCookie_) {
        strCookie = env->GetStringUTFChars(strCookie_, 0);
    }

    std::string m_cookie;
    if(strCookie) {
        m_cookie = strCookie;
    }

    if(strHash && strCookie) {
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->set_cookie(m_cookie);
    }

    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }

    if (strCookie_ && strCookie) {
        env->ReleaseStringUTFChars(strCookie_, strCookie);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_setSkipSSL(JNIEnv *env, jclass clazz, jboolean skip_ssl) {


}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_setUseGmTLS(JNIEnv *env, jclass clazz, jboolean use_gm_tls) {

}



using JNIMAP = std::map<std::string, std::string>;

static JNIMAP gen_map(JNIEnv *env, jobject headers_keys, jobject headers_values) {
    JNIMAP map_ret;
    if (headers_keys && headers_values) {
        jclass cls_list_key = env->GetObjectClass(headers_keys);
        jclass cls_list_value = env->GetObjectClass(headers_values);
        if (cls_list_key && cls_list_value) {
            jmethodID arraylist_key_get = env->GetMethodID(cls_list_key, "get",
                                                           "(I)Ljava/lang/Object;");
            jmethodID arraylist_key_size = env->GetMethodID(cls_list_key, "size", "()I");

            jmethodID arraylist_value_get = env->GetMethodID(cls_list_value, "get",
                                                             "(I)Ljava/lang/Object;");
            jmethodID arraylist_value_size = env->GetMethodID(cls_list_value, "size", "()I");

            if (arraylist_key_get && arraylist_key_size && arraylist_value_get &&
                arraylist_value_size) {
                jint len_key = env->CallIntMethod(headers_keys, arraylist_key_size);
                jint len_value = env->CallIntMethod(headers_values, arraylist_value_size);
                if (len_key > 0 && len_key == len_value) {
                    for (int i = 0; i < len_key; ++i) {
                        jstring jstr_key = (jstring) env->CallObjectMethod(headers_keys,
                                                                           arraylist_key_get, i);
                        jstring jstr_value = (jstring) env->CallObjectMethod(headers_values,
                                                                             arraylist_value_get,
                                                                             i);
                        if (jstr_key && jstr_value) {
                            const char *sz_key = (char *) env->GetStringUTFChars(jstr_key,
                                                                                 NULL);
                            const char *sz_value = (char *) env->GetStringUTFChars(jstr_value,
                                                                                   NULL);
                            if (sz_key && sz_value) {

                                map_ret[sz_key] = sz_value;

                                env->ReleaseStringUTFChars(jstr_key, sz_key);
                                env->ReleaseStringUTFChars(jstr_value, sz_value);
                            }
                        }
                    }
                }
            }
        }
    }
    return map_ret;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_get
        (JNIEnv *env, jclass type, jstring strHash_, jint requestSeq,
                jstring strPath_, jobject headers_keys, jobject headers_values,
                jobject params_keys, jobject params_values) {
    const char *strHash = nullptr;
    const char *strPath = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strHash && strPath) {
        JNIMAP inner_headers = gen_map(env, headers_keys, headers_values);
        JNIMAP inner_url_params = gen_map(env, params_keys, params_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->get(strPath, inner_headers, inner_url_params, GlobalCallBackFunc, requestSeq);
    }
    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
}



extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_postFromData
        (JNIEnv *env, jclass type, jstring strHash_, jint requestSeq,
                jstring strPath_, jobject headers_keys, jobject headers_values,
                jobject params_keys, jobject params_values) {
    const char *strHash = nullptr;
    const char *strPath = nullptr;
    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strHash && strPath) {
        JNIMAP  inner_headers = gen_map(env, headers_keys, headers_values);
        JNIMAP  inner_params = gen_map(env, params_keys, params_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->post_form(strPath, inner_headers, inner_params, GlobalCallBackFunc, requestSeq);
    }
    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_postJson
        (JNIEnv *env, jclass type, jstring strHash_,
                jint requestSeq, jstring strPath_,
                jobject headers_keys, jobject headers_values,
                jstring strJson_) {
    const char *strHash = nullptr;
    const char *strPath = nullptr;
    const char *strJson = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strJson_) {
        strJson = env->GetStringUTFChars(strJson_, 0);
    }
    std::string str_json;
    if (strJson) {
        str_json = strJson;
    }
    if (strHash && strPath) {
        JNIMAP  inner_headers = gen_map(env, headers_keys, headers_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->post_json(strPath, inner_headers, str_json, GlobalCallBackFunc, requestSeq, GlobalCookieCallBackFunc);
    }
    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
    if (strJson_ && strJson) {
        env->ReleaseStringUTFChars(strJson_, strJson);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_putJson
        (JNIEnv *env, jclass type, jstring strHash_, jint requestSeq, jstring strPath_,
                jobject headers_keys, jobject headers_values, jstring strJson_) {
    const char *strHash = nullptr;
    const char *strPath = nullptr;
    const char *strJson = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strJson_) {
        strJson = env->GetStringUTFChars(strJson_, 0);
    }
    std::string str_json;
    if (strJson) {
        str_json = strJson;
    }

    if (strHash && strPath) {
        JNIMAP  inner_headers = gen_map(env, headers_keys, headers_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->put(strPath, str_json, inner_headers, GlobalCallBackFunc, requestSeq);
    }

    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
    if (strJson_ && strJson) {
        env->ReleaseStringUTFChars(strJson_, strJson);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_delete
        (JNIEnv *env, jclass type, jstring strHash_, jint requestSeq, jstring strPath_,
                jobject headers_keys, jobject headers_values, jstring strJson_) {
    const char *strHash = nullptr;
    const char *strPath = nullptr;
    const char *strJson = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strJson_) {
        strJson = env->GetStringUTFChars(strJson_, 0);
    }

    std::string str_json;
    if (strJson) {
        str_json = strJson;
    }

    if (strHash && strPath) {
        JNIMAP  inner_headers = gen_map(env, headers_keys, headers_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->deleteMethod(strPath, str_json, inner_headers, GlobalCallBackFunc, requestSeq);
    }

    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
    if (strJson_ && strJson) {
        env->ReleaseStringUTFChars(strJson_, strJson);
    }


}



extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_postFile
        (JNIEnv *env, jclass type, jstring strHash_, jint requestSeq,
                jstring strPath_, jobject headers_keys, jobject headers_values,
                jstring strFormName_, jobject params_keys, jobject params_values,
                jstring strJsonName_, jstring strJson_, jstring strFileKeyName_,
                jstring strFilePath_, jstring strFileName_) {

    const char *strHash = nullptr;
    const char *strPath = nullptr;
    const char *strFormName = nullptr;
    const char *strJsonName = nullptr;
    const char *strJson = nullptr;
    const char *strFileKeyName = nullptr;
    const char *strFilePath = nullptr;
    const char *strFileName = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strFormName_) {
        strFormName = env->GetStringUTFChars(strFormName_, 0);
    }
    if (strJsonName_) {
        strJsonName = env->GetStringUTFChars(strJsonName_, 0);
    }
    if (strJson_) {
        strJson = env->GetStringUTFChars(strJson_, 0);
    }
    if (strFileKeyName_) {
        strFileKeyName = env->GetStringUTFChars(strFileKeyName_, 0);
    }
    if (strFilePath_) {
        strFilePath = env->GetStringUTFChars(strFilePath_, 0);
    }
    if (strFileName_) {
        strFileName = env->GetStringUTFChars(strFileName_, 0);
    }

    std::string str_formname;
    std::string str_jsonname;
    std::string str_json;
    std::string str_filekeyname;
    std::string str_filename;
    std::string str_filepath;

    if (strFormName) {
        str_formname = strFormName;
    }
    if (strJsonName) {
        str_jsonname = strJsonName;
    }
    if (strJson) {
        str_json = strJson;
    }
    if (strFileKeyName) {
        str_filekeyname = strFileKeyName;
    }
    if (strFileName) {
        str_filename = strFileName;
    }
    if (strFilePath) {
        str_filepath = strFilePath;
    }

    if (strHash && strPath) {
        JNIMAP inner_headers = gen_map(env, headers_keys, headers_values);
        JNIMAP inner_form_params = gen_map(env, params_keys, params_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->post_file(strPath, inner_headers, str_formname, inner_form_params, str_jsonname,
                str_json, str_filekeyname, str_filename, str_filepath, GlobalCallBackFunc, requestSeq);
    }

    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
    if (strFormName_ && strFormName) {
        env->ReleaseStringUTFChars(strFormName_, strFormName);
    }
    if (strJsonName_ && strJsonName) {
        env->ReleaseStringUTFChars(strJsonName_, strJsonName);
    }
    if (strJson_ && strJson) {
        env->ReleaseStringUTFChars(strJson_, strJson);
    }
    if (strFileKeyName_ && strFileKeyName) {
        env->ReleaseStringUTFChars(strFileKeyName_, strFileKeyName);
    }
    if (strFilePath_ && strFilePath) {
        env->ReleaseStringUTFChars(strFilePath_, strFilePath);
    }
    if (strFileName_ && strFileName) {
        env->ReleaseStringUTFChars(strFileName_, strFileName);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_easemob_emssl_EMJniCurl_download
        (JNIEnv *env, jclass type, jstring strHash_, jint requestSeq,
                jstring strPath_, jobject headers_keys, jobject headers_values,
                jobject params_keys, jobject params_values, jstring strFilePath_) {
    const char *strPath = nullptr;
    const char *strFilePath = nullptr;
    const char *strHash = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strFilePath_) {
        strFilePath = env->GetStringUTFChars(strFilePath_, 0);
    }

    std::string str_filepath;
    if (strFilePath) {
        str_filepath = strFilePath;
    }

    if (strHash && strPath) {
        JNIMAP inner_headers = gen_map(env, headers_keys, headers_values);
        JNIMAP inner_url_params = gen_map(env, params_keys, params_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        p->download(strPath, inner_headers, inner_url_params, str_filepath, GlobalCallBackFunc, requestSeq);
    }

    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
    if (strFilePath_ && strFilePath) {
        env->ReleaseStringUTFChars(strFilePath_, strFilePath);
    }

}

// ============================================================ synchronized methods =====================================================================


jbyteArray getJByteArray(JNIEnv *env, const char *str, unsigned int len) {
    jbyteArray jArray = env->NewByteArray(len);
    env->SetByteArrayRegion(jArray, 0, len, reinterpret_cast<const jbyte*>(str));
    return jArray;
}

jstring getJStringObject(JNIEnv *env, const std::string &str) {
    jclass clazz = env->FindClass("java/lang/String");
    jbyteArray jArray = getJByteArray(env, (char*)str.c_str(), str.length());
    jstring jstr = (jstring)(*env).NewObject(clazz, (*env).GetMethodID(clazz, "<init>", "([B)V"), jArray);
    (*env).DeleteLocalRef(jArray);
    return jstr;
}

jobject getJInteger(JNIEnv *env, int32_t val) {
    jclass clazz = env->FindClass("java/lang/Integer");
    return (*env).NewObject(clazz, (*env).GetMethodID(clazz, "<init>", "(I)V"), val);
}



/*
 * Class:     com_easemob_emssl_EMJniCurl
 * Method:    syncGet
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/util/List;Ljava/util/List;Ljava/util/List;Ljava/util/List;)I
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_easemob_emssl_EMJniCurl_syncGet
        (JNIEnv *env, jclass type, jstring strHash_, jstring strPath_, jobject headers_keys, jobject headers_values, jobject params_keys, jobject params_values) {
        const char *strHash = nullptr;
        const char *strPath = nullptr;

        if (strHash_) {
            strHash = env->GetStringUTFChars(strHash_, 0);
        }
        if (strPath_) {
            strPath = env->GetStringUTFChars(strPath_, 0);
        }

        long httpcode = 0;

        std::string str_resp;
        if (strHash && strPath) {
            JNIMAP inner_headers = gen_map(env, headers_keys, headers_values);
            JNIMAP inner_url_params = gen_map(env, params_keys, params_values);
            RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
            httpcode = p->get_ret(strPath, inner_headers, inner_url_params, str_resp);
        }
        if (strHash_ && strHash) {
            env->ReleaseStringUTFChars(strHash_, strHash);
        }
        if (strPath_ && strPath) {
            env->ReleaseStringUTFChars(strPath_, strPath);
        }

        jclass clazz = env->FindClass("android/util/Pair");
        jmethodID construct = env->GetMethodID(clazz,"<init>","(Ljava/lang/Object;Ljava/lang/Object;)V");
        jstring second_key = getJStringObject(env, str_resp);
        jobject first_key = getJInteger(env, httpcode);
        jobject obj_pair = env->NewObject(clazz,construct,first_key, second_key);

        env->DeleteLocalRef(first_key);
        env->DeleteLocalRef(second_key);
        env->DeleteLocalRef(clazz);

        return obj_pair;
}




/*
 * Class:     com_easemob_emssl_EMJniCurl
 * Method:    syncPostJson
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/util/List;Ljava/util/List;Ljava/lang/String;)I
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_easemob_emssl_EMJniCurl_syncPostJson
        (JNIEnv *env, jclass type, jstring strHash_, jstring strPath_, jobject headers_keys, jobject headers_values, jstring strJson_) {
    const char *strHash = nullptr;
    const char *strPath = nullptr;
    const char *strJson = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strJson_) {
        strJson = env->GetStringUTFChars(strJson_, 0);
    }
    std::string str_json;
    if (strJson) {
        str_json = strJson;
    }
    std::string str_resp;
    long httpcode = 0;
    if (strHash && strPath) {
        JNIMAP  inner_headers = gen_map(env, headers_keys, headers_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        httpcode = p->post_json_ret(strPath, inner_headers, str_json, str_resp);
    }
    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
    if (strJson_ && strJson) {
        env->ReleaseStringUTFChars(strJson_, strJson);
    }

    jclass clazz = env->FindClass("android/util/Pair");
    jmethodID construct = env->GetMethodID(clazz,"<init>","(Ljava/lang/Object;Ljava/lang/Object;)V");
    jstring second_key = getJStringObject(env, str_resp);
    jobject first_key = getJInteger(env, httpcode);
    jobject obj_pair = env->NewObject(clazz,construct,first_key, second_key);

    env->DeleteLocalRef(first_key);
    env->DeleteLocalRef(second_key);
    env->DeleteLocalRef(clazz);
    return obj_pair;
}

/*
 * Class:     com_easemob_emssl_EMJniCurl
 * Method:    syncPutJson
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/util/List;Ljava/util/List;Ljava/lang/String;)I
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_easemob_emssl_EMJniCurl_syncPutJson
        (JNIEnv *env, jclass type, jstring strHash_, jstring strPath_, jobject headers_keys, jobject headers_values, jstring strJson_) {
    const char *strHash = nullptr;
    const char *strPath = nullptr;
    const char *strJson = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strJson_) {
        strJson = env->GetStringUTFChars(strJson_, 0);
    }
    std::string str_json;
    if (strJson) {
        str_json = strJson;
    }
    std::string str_resp;
    long httpcode = 0;
    if (strHash && strPath) {
        JNIMAP  inner_headers = gen_map(env, headers_keys, headers_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        httpcode = p->put_ret(strPath, str_json, inner_headers, str_resp);
    }
    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
    if (strJson_ && strJson) {
        env->ReleaseStringUTFChars(strJson_, strJson);
    }

    jclass clazz = env->FindClass("android/util/Pair");
    jmethodID construct = env->GetMethodID(clazz,"<init>","(Ljava/lang/Object;Ljava/lang/Object;)V");
    jstring second_key = getJStringObject(env, str_resp);
    jobject first_key = getJInteger(env, httpcode);
    jobject obj_pair = env->NewObject(clazz,construct,first_key, second_key);

    env->DeleteLocalRef(first_key);
    env->DeleteLocalRef(second_key);
    env->DeleteLocalRef(clazz);

    return obj_pair;
}

/*
 * Class:     com_easemob_emssl_EMJniCurl
 * Method:    syncDelete
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/util/List;Ljava/util/List;Ljava/lang/String;)I
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_easemob_emssl_EMJniCurl_syncDelete
        (JNIEnv *env, jclass type, jstring strHash_, jstring strPath_, jobject headers_keys, jobject headers_values, jstring strJson_) {

    const char *strHash = nullptr;
    const char *strPath = nullptr;
    const char *strJson = nullptr;

    if (strHash_) {
        strHash = env->GetStringUTFChars(strHash_, 0);
    }
    if (strPath_) {
        strPath = env->GetStringUTFChars(strPath_, 0);
    }
    if (strJson_) {
        strJson = env->GetStringUTFChars(strJson_, 0);
    }

    std::string str_json;
    if (strJson) {
        str_json = strJson;
    }
    std::string str_resp;
    long httpcode = 0;
    if (strHash && strPath) {
        JNIMAP  inner_headers = gen_map(env, headers_keys, headers_values);
        RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
        httpcode = p->delete_ret(strPath, str_json, inner_headers, str_resp);
    }

    if (strHash_ && strHash) {
        env->ReleaseStringUTFChars(strHash_, strHash);
    }
    if (strPath_ && strPath) {
        env->ReleaseStringUTFChars(strPath_, strPath);
    }
    if (strJson_ && strJson) {
        env->ReleaseStringUTFChars(strJson_, strJson);
    }

    jclass clazz = env->FindClass("android/util/Pair");
    jmethodID construct = env->GetMethodID(clazz,"<init>","(Ljava/lang/Object;Ljava/lang/Object;)V");
    jstring second_key = getJStringObject(env, str_resp);
    jobject first_key = getJInteger(env, httpcode);
    jobject obj_pair = env->NewObject(clazz,construct,first_key, second_key);

    env->DeleteLocalRef(first_key);
    env->DeleteLocalRef(second_key);
    env->DeleteLocalRef(clazz);

    return obj_pair;
}

/*
 * Class:     com_easemob_emssl_EMJniCurl
 * Method:    syncPostFile
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/util/List;Ljava/util/List;Ljava/lang/String;Ljava/util/List;Ljava/util/List;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_easemob_emssl_EMJniCurl_syncPostFile
        (JNIEnv *env, jclass type, jstring strHash_, jint requestSeq, jstring strPath_, jobject headers_keys, jobject headers_values, jstring strFormName_,
                jobject params_keys, jobject params_values, jstring strJsonName_, jstring strJson_, jstring strFileKeyName_, jstring strFilePath_, jstring strFileName_) {

        const char *strHash = nullptr;
        const char *strPath = nullptr;
        const char *strFormName = nullptr;
        const char *strJsonName = nullptr;
        const char *strJson = nullptr;
        const char *strFileKeyName = nullptr;
        const char *strFilePath = nullptr;
        const char *strFileName = nullptr;

        if (strHash_) {
            strHash = env->GetStringUTFChars(strHash_, 0);
        }
        if (strPath_) {
            strPath = env->GetStringUTFChars(strPath_, 0);
        }
        if (strFormName_) {
            strFormName = env->GetStringUTFChars(strFormName_, 0);
        }
        if (strJsonName_) {
            strJsonName = env->GetStringUTFChars(strJsonName_, 0);
        }
        if (strJson_) {
            strJson = env->GetStringUTFChars(strJson_, 0);
        }
        if (strFileKeyName_) {
            strFileKeyName = env->GetStringUTFChars(strFileKeyName_, 0);
        }
        if (strFilePath_) {
            strFilePath = env->GetStringUTFChars(strFilePath_, 0);
        }
        if (strFileName_) {
            strFileName = env->GetStringUTFChars(strFileName_, 0);
        }

        std::string str_formname;
        std::string str_jsonname;
        std::string str_json;
        std::string str_filekeyname;
        std::string str_filename;
        std::string str_filepath;

        if (strFormName) {
            str_formname = strFormName;
        }
        if (strJsonName) {
            str_jsonname = strJsonName;
        }
        if (strJson) {
            str_json = strJson;
        }
        if (strFileKeyName) {
            str_filekeyname = strFileKeyName;
        }
        if (strFileName) {
            str_filename = strFileName;
        }
        if (strFilePath) {
            str_filepath = strFilePath;
        }
        std::string str_resp;
        long httpcode = 0;

        if (strHash && strPath) {
            JNIMAP inner_headers = gen_map(env, headers_keys, headers_values);
            JNIMAP inner_form_params = gen_map(env, params_keys, params_values);
            RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
            httpcode = p->post_file_ret(strPath, inner_headers, str_formname, inner_form_params, str_jsonname,
                                        str_json, str_filekeyname, str_filename, str_filepath, GlobalCallBackFunc, requestSeq, str_resp);
        }

        if (strHash_ && strHash) {
            env->ReleaseStringUTFChars(strHash_, strHash);
        }
        if (strPath_ && strPath) {
            env->ReleaseStringUTFChars(strPath_, strPath);
        }
        if (strFormName_ && strFormName) {
            env->ReleaseStringUTFChars(strFormName_, strFormName);
        }
        if (strJsonName_ && strJsonName) {
            env->ReleaseStringUTFChars(strJsonName_, strJsonName);
        }
        if (strJson_ && strJson) {
            env->ReleaseStringUTFChars(strJson_, strJson);
        }
        if (strFileKeyName_ && strFileKeyName) {
            env->ReleaseStringUTFChars(strFileKeyName_, strFileKeyName);
        }
        if (strFilePath_ && strFilePath) {
            env->ReleaseStringUTFChars(strFilePath_, strFilePath);
        }
        if (strFileName_ && strFileName) {
            env->ReleaseStringUTFChars(strFileName_, strFileName);
        }
    jclass clazz = env->FindClass("android/util/Pair");
    jmethodID construct = env->GetMethodID(clazz,"<init>","(Ljava/lang/Object;Ljava/lang/Object;)V");
    jstring second_key = getJStringObject(env, str_resp);
    jobject first_key = getJInteger(env, httpcode);
    jobject obj_pair = env->NewObject(clazz,construct,first_key, second_key);

    env->DeleteLocalRef(first_key);
    env->DeleteLocalRef(second_key);
    env->DeleteLocalRef(clazz);

    return obj_pair;
}

/*
 * Class:     com_easemob_emssl_EMJniCurl
 * Method:    syncDownload
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/util/List;Ljava/util/List;Ljava/util/List;Ljava/util/List;Ljava/lang/String;)I
 */
extern "C"
JNIEXPORT jobject JNICALL Java_com_easemob_emssl_EMJniCurl_syncDownload
        (JNIEnv *env, jclass type, jstring strHash_, jint requestSeq, jstring strPath_, jobject headers_keys, jobject headers_values, jobject params_keys, jobject params_values, jstring strFilePath_) {

        const char *strPath = nullptr;
        const char *strFilePath = nullptr;
        const char *strHash = nullptr;

        if (strHash_) {
            strHash = env->GetStringUTFChars(strHash_, 0);
        }
        if (strPath_) {
            strPath = env->GetStringUTFChars(strPath_, 0);
        }
        if (strFilePath_) {
            strFilePath = env->GetStringUTFChars(strFilePath_, 0);
        }

        std::string str_filepath;
        if (strFilePath) {
            str_filepath = strFilePath;
        }
        std::string str_resp;
        long httpcode = 0;

        if (strHash && strPath) {
            JNIMAP inner_headers = gen_map(env, headers_keys, headers_values);
            JNIMAP inner_url_params = gen_map(env, params_keys, params_values);
            RequestManager *p = HttpManager::get_instance()->get_request_manager(strHash);
            httpcode = p->download_ret(strPath, inner_headers, inner_url_params, str_filepath, GlobalCallBackFunc, requestSeq, str_resp);
        }

        if (strHash_ && strHash) {
            env->ReleaseStringUTFChars(strHash_, strHash);
        }
        if (strPath_ && strPath) {
            env->ReleaseStringUTFChars(strPath_, strPath);
        }
        if (strFilePath_ && strFilePath) {
            env->ReleaseStringUTFChars(strFilePath_, strFilePath);
        }
    jclass clazz = env->FindClass("android/util/Pair");
    jmethodID construct = env->GetMethodID(clazz,"<init>","(Ljava/lang/Object;Ljava/lang/Object;)V");
    jstring second_key = getJStringObject(env, str_resp);
    jobject first_key = getJInteger(env, httpcode);
    jobject obj_pair = env->NewObject(clazz,construct,first_key, second_key);

    env->DeleteLocalRef(first_key);
    env->DeleteLocalRef(second_key);
    env->DeleteLocalRef(clazz);

    return obj_pair;
}




// ============================================================ encrypt methods =====================================================================

extern "C"
JNIEXPORT jstring JNICALL
Java_com_easemob_emssl_EMJniCurl_sha256
(JNIEnv *env, jclass type, jstring value_) {
    const char *value = nullptr;
    if (value_) {
        value = env->GetStringUTFChars(value_, 0);
    }
    std::string str_value;
    if (value) {
        str_value = value;
    }
    std::string str_ret = sha256(str_value);

    if (value_ && value) {
        env->ReleaseStringUTFChars(value_, value);
    }
    return env->NewStringUTF(str_ret.c_str());
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_easemob_emssl_EMJniCurl_aesCbc
(JNIEnv *env, jclass type, jbyteArray encryptData_, jint len, jstring key_, jstring iv_) {
    if (!encryptData_ || !key_ || !iv_) {
        return nullptr;
    }

    jbyte *encryptData = env->GetByteArrayElements(encryptData_, NULL);
    const char *key = env->GetStringUTFChars(key_, 0);
    const char *iv = env->GetStringUTFChars(iv_, 0);

    std::string str_key(key), str_iv(iv);

    std::string str_ret = decrypt_aescbc((const unsigned char *)encryptData, len, str_key, str_iv);

    env->ReleaseByteArrayElements(encryptData_, encryptData, 0);
    env->ReleaseStringUTFChars(key_, key);
    env->ReleaseStringUTFChars(iv_, iv);

    return env->NewStringUTF(str_ret.c_str());
}