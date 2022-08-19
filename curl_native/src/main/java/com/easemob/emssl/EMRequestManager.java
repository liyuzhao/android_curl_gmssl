package com.easemob.emssl;


import android.util.Log;
import android.util.Pair;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicInteger;

public class EMRequestManager {
    private static AtomicInteger gSeq = new AtomicInteger(0);
    private String seqHash; // this is actually native request_manager

    public EMRequestManager() {
        seqHash = getHash();
    }

    private String getHash() {
        return UUID.randomUUID().toString().replace("-", "");
    }

    /**
     *
     * @param strKey "Cookie"
     * @param strValue "xxx"
     */
    public void addBasicHeader(String strKey, String strValue) {
        EMJniCurl.addBasicHeader(seqHash, strKey, strValue);
    }

    public void addBasicUrlParams(String strKey, String strValue) {
        EMJniCurl.addBasicURLParam(seqHash, strKey, strValue);
    }

    public void setHost(String strHost) {
        EMJniCurl.setHost(seqHash, strHost);
    }

    public void setCertPath(String strCert) {
        Log.e("EMRequestManager", "setCertPath:" + strCert);
        EMJniCurl.setCertPath(seqHash, strCert);
    }

    public void setSSLCertPath(String strSSLCertPath, String strSSLKeyPath, String strSSLKeyPasswd) {
        Log.e("EMRequestManager", "setSSLCertPath:" + strSSLCertPath + ",strSSLKeyPath:" + strSSLKeyPath + ",strSSLKeyPasswd:" + strSSLKeyPasswd);
        EMJniCurl.setSSLCertPath(seqHash, strSSLCertPath, strSSLKeyPath, strSSLKeyPasswd);
    }

    public void setCookie(String cookie) {
        EMJniCurl.setCookie(seqHash, cookie);
    }

    public void setProxy(String strProxy) {
        EMJniCurl.setProxy(seqHash, strProxy);
    }

    public void get(String strPath, Map<String, String> headers, Map<String, String> params, EMHttpCallback callback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        List<String> paramKeyList = null;
        List<String> paramValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }

        if (params != null) {
            paramKeyList = new ArrayList<>();
            paramValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : params.entrySet()) {
                paramKeyList.add(entry.getKey());
                paramKeyList.add(entry.getValue());
            }
        }

        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }
        EMJniCurl.get(seqHash, seq, strPath, headerKeyList, headerValueList, paramKeyList, paramValueList);
    }

    public void postForm(String strPath, Map<String, String> headers, Map<String, Object> formdatas, EMHttpCallback callback) {
         postForm(strPath, headers, formdatas, callback, null);
    }

    public void postForm(String strPath, Map<String, String> headers, Map<String, Object> formdatas, EMHttpCallback callback, EMCookieCallback cookieCallback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        List<String> paramKeyList = null;
        List<String> paramValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }

        if (formdatas != null) {
            paramKeyList = new ArrayList<>();
            paramValueList = new ArrayList<>();
            for (Map.Entry<String, Object> entry : formdatas.entrySet()) {
                paramKeyList.add(entry.getKey());
                paramValueList.add(String.valueOf(entry.getValue()));
            }
        }

        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }

        if (cookieCallback != null) {
            EMHttpManager.getInstance().addCookieCallback(seq, cookieCallback);
        }
        EMJniCurl.postFromData(seqHash, seq, strPath, headerKeyList, headerValueList, paramKeyList, paramValueList);
    }

    public void postJson(String strPath, Map<String, String> headers, String strJson, EMHttpCallback callback) {
        postJson(strPath, headers, strJson, callback, null);
    }

    public void postJson(String strPath, Map<String, String> headers, String strJson, EMHttpCallback callback, EMCookieCallback cookieCallback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }
        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }
        if (cookieCallback != null) {
            EMHttpManager.getInstance().addCookieCallback(seq, cookieCallback);
        }
        EMJniCurl.postJson(seqHash, seq, strPath, headerKeyList, headerValueList, strJson);
    }

    public void postFile(String strPath, Map<String, String> headers, String strFormName, Map<String, Object> formdatas,
                         String strJsonName, String Json, String strFileKeyName, String strFilePath, String strFileName,
                         EMHttpCallback callback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        List<String> paramKeyList = null;
        List<String> paramValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }

        if (formdatas != null) {
            paramKeyList = new ArrayList<>();
            paramValueList = new ArrayList<>();
            for (Map.Entry<String, Object> entry : formdatas.entrySet()) {
                paramKeyList.add(entry.getKey());
                paramValueList.add(String.valueOf(entry.getValue()));
            }
        }

        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }

        EMJniCurl.postFile(seqHash, seq, strPath, headerKeyList, headerValueList, strFormName, paramKeyList, paramValueList,
                strJsonName, Json, strFileKeyName, strFilePath, strFileName);
    }

    public void putJson(String strPath, Map<String, String> headers, String strJson, EMHttpCallback callback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList  = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }
        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }
        EMJniCurl.putJson(seqHash, seq, strPath, headerKeyList, headerValueList, strJson);
    }

    public void delete(String strPath, Map<String, String> headers, String strJson, EMHttpCallback callback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList  = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }
        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }
        EMJniCurl.delete(seqHash, seq, strPath, headerKeyList, headerValueList, strJson);
    }

    public void download(String strPath, Map<String, String> headers, Map<String, String> params,
                         String strFilePath, EMHttpCallback callback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        List<String> paramKeyList = null;
        List<String> paramValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }

        if (params != null) {
            paramKeyList = new ArrayList<>();
            paramValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : params.entrySet()) {
                paramKeyList.add(entry.getKey());
                paramValueList.add(entry.getValue());
            }
        }

        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }

        EMJniCurl.download(seqHash, seq, strPath, headerKeyList, headerValueList, paramKeyList, paramValueList, strFilePath);
    }


    // ============================================================ synchronized methods =====================================================================
    public Pair<Integer, String> syncGet(String strPath, Map<String, String> headers, Map<String, String> params) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        List<String> paramKeyList = null;
        List<String> paramValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }

        if (params != null) {
            paramKeyList = new ArrayList<>();
            paramValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : params.entrySet()) {
                paramKeyList.add(entry.getKey());
                paramKeyList.add(entry.getValue());
            }
        }
        return EMJniCurl.syncGet(seqHash, strPath, headerKeyList, headerValueList, paramKeyList, paramValueList);
    }

    public Pair<Integer, String> syncPostJson(String strPath, Map<String, String> headers, String strJson) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }
        return EMJniCurl.syncPostJson(seqHash, strPath, headerKeyList, headerValueList, strJson);
    }

    public Pair<Integer, String> syncPostFile(String strPath, Map<String, String> headers, String strFormName, Map<String, Object> formdatas,
                         String strJsonName, String Json, String strFileKeyName, String strFilePath, String strFileName,
                         EMHttpCallback callback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        List<String> paramKeyList = null;
        List<String> paramValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }

        if (formdatas != null) {
            paramKeyList = new ArrayList<>();
            paramValueList = new ArrayList<>();
            for (Map.Entry<String, Object> entry : formdatas.entrySet()) {
                paramKeyList.add(entry.getKey());
                paramValueList.add(String.valueOf(entry.getValue()));
            }
        }

        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }
        return EMJniCurl.syncPostFile(seqHash, seq, strPath, headerKeyList, headerValueList, strFormName, paramKeyList, paramValueList,
                strJsonName, Json, strFileKeyName, strFilePath, strFileName);
    }

    public Pair<Integer, String> syncPutJson(String strPath, Map<String, String> headers, String strJson) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList  = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }
        return EMJniCurl.syncPutJson(seqHash, strPath, headerKeyList, headerValueList, strJson);
    }

    public Pair<Integer, String> syncDelete(String strPath, Map<String, String> headers, String strJson) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList  = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }
        return EMJniCurl.syncDelete(seqHash, strPath, headerKeyList, headerValueList, strJson);
    }

    public Pair<Integer, String> syncDownload(String strPath, Map<String, String> headers, Map<String, String> params,
                             String strFilePath, EMHttpCallback callback) {
        List<String> headerKeyList = null;
        List<String> headerValueList = null;
        List<String> paramKeyList = null;
        List<String> paramValueList = null;
        if (headers != null) {
            headerKeyList = new ArrayList<>();
            headerValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                headerKeyList.add(entry.getKey());
                headerValueList.add(entry.getValue());
            }
        }

        if (params != null) {
            paramKeyList = new ArrayList<>();
            paramValueList = new ArrayList<>();
            for (Map.Entry<String, String> entry : params.entrySet()) {
                paramKeyList.add(entry.getKey());
                paramValueList.add(entry.getValue());
            }
        }

        int seq = gSeq.incrementAndGet();
        if (callback != null) {
            EMHttpManager.getInstance().addCallback(seq, callback);
        }

        return EMJniCurl.syncDownload(seqHash, seq, strPath, headerKeyList, headerValueList, paramKeyList, paramValueList, strFilePath);
    }



}
