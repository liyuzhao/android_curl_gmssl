package com.easemob.emssl;

import android.content.Context;
import android.util.SparseArray;

import com.easemob.emssl.utils.EMMisc;

import java.util.ArrayList;
import java.util.List;

public class EMHttpManager {

    public static final int RESULT_FAILED = -1;
    public static final int RESULT_ALL_SUCCESS = 0;
    public static final int RESULT_DOWNLOAD_PROGRESS = 1;
    public static final int RESULT_UPLOAD_PROGRESS = 2;

    private List<EMRequestManager> mRequest = new ArrayList<>();
    private final SparseArray<EMHttpCallback> mCallbackMap = new SparseArray<>();

    private final SparseArray<EMCookieCallback> mCookieMap = new SparseArray<>();

    private static EMHttpManager sInstance = new EMHttpManager();

    public static EMHttpManager getInstance() {
        return sInstance;
    }

    private EMHttpManager() {

    }

    public void init(int threadPoolSize, Context context) {
        EMJniCurl.init(threadPoolSize, this);
        EMMisc.copyCertFile(context);
    }

    public void unInit() {
        EMJniCurl.unInit();
    }

    public EMRequestManager createRequest() {
        EMRequestManager request = new EMRequestManager();
        synchronized (mRequest) {
            mRequest.add(request);
        }
        return request;
    }

    public void addCallback(int seq, EMHttpCallback callback) {
        synchronized (mCallbackMap) {
            mCallbackMap.put(seq, callback);
        }
    }

    public void addCookieCallback(int seq, EMCookieCallback callback) {
        synchronized (mCookieMap) {
            mCookieMap.put(seq, callback);
        }
    }

    /**
     * Jni callback, dont proguard
     * callback invoke in mainthread
     * @param type
     * @param strResponse
     * @param persent
     * @param seq
     * @param errorCode
     */
    public void callBack(int type, final String strResponse, final float persent, int seq, final int errorCode) {
        final EMHttpCallback callback = mCallbackMap.get(seq);
        final String strResponse_safe = strResponse == null ? "" : strResponse;
        if (callback != null) {
            if (type == RESULT_FAILED) {
//                ThreadUtils.runOnUiThread(new Runnable() {
//                    @Override
//                    public void run() {
//                        callback.fail(errorCode);
//                    }
//                });
                callback.fail(errorCode);
                mCallbackMap.remove(seq);
            } else {
                if (type == RESULT_DOWNLOAD_PROGRESS || type == RESULT_UPLOAD_PROGRESS) {
//                    ThreadUtils.runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            callback.progress(persent);
//                        }
//                    });
                    callback.progress(persent);
                } else if (type == RESULT_ALL_SUCCESS) {
//                    ThreadUtils.runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            callback.success(strResponse_safe);
//                        }
//                    });
                    callback.success(strResponse_safe);
                    mCallbackMap.remove(seq);
                } else {
                    mCookieMap.remove(seq);
                }
            }
        } else {
            mCallbackMap.remove(seq);
        }

    }

    public void cookieCallBack(int type, final String strCookie, int seq) {
        final EMCookieCallback callback = mCookieMap.get(seq);
        final String strCookie_safe = strCookie == null ? "" : strCookie;
        if (callback != null) {
            if (type == RESULT_ALL_SUCCESS) {
                callback.success(strCookie_safe);
                mCookieMap.remove(seq);
            } else {
                mCookieMap.remove(seq);
            }
        } else {
            mCookieMap.remove(seq);
        }


    }

}
