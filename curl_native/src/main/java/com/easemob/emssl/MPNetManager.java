package com.easemob.emssl;

import android.content.Context;
import android.util.Pair;

import java.util.HashMap;
import java.util.Map;

public class MPNetManager {

    private static MPNetManager sInstance;
    private static final int DEFUALT_THREAD_POOL_SIZE = 5;

    private MPNetManager() {}

    public static MPNetManager getInstance() {
        synchronized (MPNetManager.class) {
            if (sInstance == null) {
                sInstance = new MPNetManager();
            }
        }
        return sInstance;
    }

    public void init(Context context, int threadPoolSize) {
        EMHttpManager.getInstance().unInit();
        EMHttpManager.getInstance().init(threadPoolSize, context);
    }

    public void init(Context context) {
        init(context, DEFUALT_THREAD_POOL_SIZE);
    }


    public void unInit() {
        EMHttpManager.getInstance().unInit();
    }

    public static class Builder {
        private EMRequestManager request;
        private String mPath = ""; // important
        private String mJson;
        private Map<String, String> headers = new HashMap<>();
        private Map<String, String> urlParams = new HashMap<>();
        private Map<String, Object> formDataParams = new HashMap<>();

        private String mJsonName;
        private String mFormName;
        private String mFileKeyName = "file";
        private String mFileName;
        private String mLocalFilePath;

        public Builder(EMRequestManager request){
            this.request = request;
        }

        public Builder(){
            this.request = EMHttpManager.getInstance().createRequest();
        }

        public Builder setHost(String host) {
            this.request.setHost(host);
            return this;
        }

        public Builder setCookie(String cookie) {
            this.request.setCookie(cookie);
            return this;
        }
//        private Builder with(EMRequestManager request) {
//            this.request = request;
//            return this;
//        }

        public Builder addHeader(String key, String value) {
            headers.put(key, value);
            return this;
        }

        public Builder addHeaders(Map<String, String> allHeaders) {
            if (allHeaders != null && !allHeaders.isEmpty()) {
                headers.putAll(allHeaders);
            }
            return this;
        }

        public Builder addUrlParam(String key, String value) {
            urlParams.put(key, value);
            return this;
        }

        public Builder addUrlParams(Map<String, String> allUrlParams) {
            if (allUrlParams != null && !allUrlParams.isEmpty()) {
                urlParams.putAll(allUrlParams);
            }
            return this;
        }

        public Builder setUrlPath(String urlPath) {
            mPath = urlPath;
            return this;
        }

        public Builder addFormData(String key, Object value) {
            formDataParams.put(key, value);
            return this;
        }

        public Builder addFormDatas(Map<String, Object> allPormDatas) {
            if (allPormDatas != null && !allPormDatas.isEmpty()) {
                formDataParams.putAll(allPormDatas);
            }
            return this;
        }

        public Builder setFormName(String formName) {
            this.mFormName = formName;
            return this;
        }

        public Builder setJsonContent(String jsonContent) {
            this.mJson = jsonContent;
            return this;
        }

        public Builder setJsonName(String jsonName) {
            this.mJsonName = jsonName;
            return this;
        }

        public Builder setFileKeyName(String fileKeyName) {
            mFileKeyName = fileKeyName;
            return this;
        }

        public Builder setFileName(String fileName) {
            mFileName = fileName;
            return this;
        }

        public Builder setLocalFilePath(String filePath) {
            mLocalFilePath = filePath;
            return this;
        }

        public void get(final EMHttpCallback callback) {
            request.get(mPath, headers, urlParams, callback);
        }

        public void postFormData(final EMHttpCallback callback) {
            request.postForm(mPath, headers, formDataParams, callback);
        }

        public void postFormData(final EMHttpCallback callback, EMCookieCallback cookieCallback) {
            request.postForm(mPath, headers, formDataParams, callback, cookieCallback);
        }

        public void postJson(final EMHttpCallback callback) {
            request.postJson(mPath, headers, mJson, callback);
        }

        public void postJson(final EMHttpCallback callback, EMCookieCallback cookieCallback) {
            request.postJson(mPath, headers, mJson, callback, cookieCallback);
        }

        public void putJson(final EMHttpCallback callback) {
            request.putJson(mPath, headers, mJson, callback);
        }

        public void delete(final EMHttpCallback callback) {
            request.delete(mPath, headers, mJson, callback);
        }

        /**
         * 需要设置 setLocalFilePath 、setFileName、setFileKeyName(默认:file)
         * @param callback
         */
        public void postFile(final EMHttpCallback callback) {
            request.postFile(mPath, headers, mFormName, formDataParams, mJsonName, mJson,
                    mFileKeyName, mLocalFilePath, mFileName, callback);
        }

        public void downloadFile(final EMHttpCallback callback) {
            request.download(mPath, headers, urlParams, mLocalFilePath, callback);
        }

        public Pair<Integer, String> syncGet() {
            return request.syncGet(mPath, headers, urlParams);
        }

        public Pair<Integer, String> syncPostJson() {
            return request.syncPostJson(mPath, headers, mJson);
        }

        public Pair<Integer, String> syncPutJson() {
            return request.syncPutJson(mPath, headers, mJson);
        }

        public Pair<Integer, String> syncDelete() {
            return request.syncDelete(mPath, headers, mJson);
        }

        public Pair<Integer, String> syncDownload(EMHttpCallback callback) {
            return request.syncDownload(mPath, headers, urlParams, mLocalFilePath, callback);
        }

        public Pair<Integer, String> syncPostFile(EMHttpCallback callback) {
            return request.syncPostFile(mPath, headers, mFormName, formDataParams, mJsonName, mJson,
                    mFileKeyName, mLocalFilePath, mFileName, callback);
        }

    }

}
