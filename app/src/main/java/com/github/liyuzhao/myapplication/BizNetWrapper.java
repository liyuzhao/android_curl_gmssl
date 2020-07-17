package com.github.liyuzhao.myapplication;

public class BizNetWrapper {
//    EMRequestManager mRequest1;
//    EMRequestManager mRequest2;
//    EMRequestManager mRequest3;
//
//    private static BizNetWrapper sInstance = new BizNetWrapper();
//
//    private BizNetWrapper(){
//
//    }
//
//    public static BizNetWrapper getInstance() {
//        return sInstance;
//    }
//
//
//
//    public void init(Context context) {
//        EMHttpManager.getInstance().unInit();
//        EMHttpManager.getInstance().init(5, context);
//    }
//
//    public void uninit() {
//        EMHttpManager.getInstance().unInit();
//        mRequest1 = null;
//        mRequest2 = null;
//        mRequest3 = null;
//    }
//
//    public EMRequestManager getBizEMRequestManager(Context context) {
//        if (mRequest1 == null) {
//            mRequest1 = EMHttpManager.getInstance().getRequest();
////            mRequest1.setHost("https://sha1.badssl.com"); //an example : ca not valid
//            mRequest1.setHost("https://example.com");
////            mRequest1.setHost("https://172.17.2.211:44330");
//            mRequest1.addBasicHeader("MyCookie", "123456789");
//            mRequest1.addBasicHeader("MyCookie2", "123456789123456");
//            mRequest1.addBasicUrlParams("param1", "value");
//            mRequest1.addBasicUrlParams("param2", "value");
//            mRequest1.setCertPath(EMMisc.getAppDir(context) + EMMisc.CERT_NAME);
////            mRequest1.setProxy("http://172.18.100.56:8888"); //for debug
//        }
//        return mRequest1;
//    }
//
//    public EMRequestManager getBizEMRequestManager2(Context context) {
//        if (mRequest2 == null) {
//            mRequest2 = EMHttpManager.getInstance().getRequest();
//            mRequest2.setHost("http://example.com/");
//            mRequest2.setCertPath(EMMisc.getAppDir(context) + EMMisc.CERT_NAME);
////            mRequest2.setProxy("http://172.18.100.56:8888"); //for debug
//        }
//        return mRequest2;
//    }
//
//    public EMRequestManager getBizEMRequestManager3(Context context) {
//        if (mRequest3 == null) {
//            mRequest3 = EMHttpManager.getInstance().getRequest();
//            mRequest3.setHost("https://s3.meituan"
//                    + ".net/v1/mss_bf7e9f1c1cc54cfb819fc8ffcf965b40/boss/2.13.2/boss_assemble-2.13.2-release-guanjia"
//                    + ".apk");
//            mRequest3.setCertPath(EMMisc.getAppDir(context) + EMMisc.CERT_NAME);
////            mRequest3.setProxy("http://172.18.100.56:8888"); //for debug
//        }
//        return mRequest3;
//    }
//
//    public interface EMHttpCallbackBiz {
//        void success(BeanTest data);
//
//        void fail(int errcode);
//
//        void progress(float persent);
//    }
//
//    static public class UrlBuilder {
//        private EMRequestManager request;
//        private String mPath = ""; //important
//        private String mJson;
//        private Map<String, String> headers = new HashMap<>();
//        private Map<String, String> urlParams = new HashMap<>();
//        private Map<String, String> formDataParams = new HashMap<>();
//
//        private String mJsonName;
//        private String mFormName;
//        private String mFileKeyName;
//        private String mFileName;
//        private String mFilePath;
//        private String mDownFilePath;
//
//        public UrlBuilder with(EMRequestManager request) {
//            this.request = request;
//            return this;
//        }
//
//        public UrlBuilder addHeader(String key, String value) {
//            headers.put(key, value);
//            return this;
//        }
//
//        public UrlBuilder addUrlParam(String key, String value) {
//            urlParams.put(key, value);
//            return this;
//        }
//
//        public UrlBuilder setPath(String strPath) {
//            mPath = strPath;
//            return this;
//        }
//
//        public UrlBuilder addFormData(String key, String value) {
//            formDataParams.put(key, value);
//            return this;
//        }
//
//        public UrlBuilder setJson(String strJson) {
//            this.mJson = strJson;
//            return this;
//        }
//
//        public UrlBuilder setFormName(String formName) {
//            mFormName = formName;
//            return this;
//        }
//
//        public UrlBuilder setJsonName(String jsonName) {
//            mJsonName = jsonName;
//            return this;
//        }
//
//        public UrlBuilder setFileKeyName(String fileKeyName) {
//            mFileKeyName = fileKeyName;
//            return this;
//        }
//
//        public UrlBuilder setFileName(String fileName) {
//            mFileName = fileName;
//            return this;
//        }
//
//        public UrlBuilder setFilePath(String filePath) {
//            mFilePath = filePath;
//            return this;
//        }
//
//        public UrlBuilder setDownloadFilePath(String filePath) {
//            mDownFilePath = filePath;
//            return this;
//        }
//
//        public void get(final EMHttpCallbackBiz callback) {
//            request.get(mPath, headers, urlParams, new EMHttpCallback() {
//                @Override
//                public void success(String respones) {
//                    //gson thing...
//                    BeanTest bean = new BeanTest();
//                    bean.rep = respones;
//                    callback.success(bean);
//                }
//
//                @Override
//                public void progress(float persent) {
//                    callback.progress(persent);
//                }
//
//                @Override
//                public void fail(int errcode) {
//                    callback.fail(errcode);
//                }
//            });
//        }
//
//        public void postFormdata(final EMHttpCallbackBiz callbackBiz) {
//            request.postForm(mPath, headers, formDataParams, new EMHttpCallback() {
//                @Override
//                public void success(String respones) {
//                    //gson thing...
//                    BeanTest bean = new BeanTest();
//                    bean.rep = respones;
//                    callbackBiz.success(bean);
//                }
//
//                @Override
//                public void progress(float persent) {
//                    callbackBiz.progress(persent);
//                }
//
//                @Override
//                public void fail(int errcode) {
//                    callbackBiz.fail(errcode);
//                }
//            });
//        }
//
//        public void postJson(final EMHttpCallbackBiz callbackBiz) {
//            request.postJson(mPath, headers, mJson, new EMHttpCallback() {
//                @Override
//                public void success(String respones) {
//                    //gson thing...
//                    BeanTest bean = new BeanTest();
//                    bean.rep = respones;
//                    callbackBiz.success(bean);
//                }
//
//                @Override
//                public void progress(float persent) {
//                    callbackBiz.progress(persent);
//                }
//
//                @Override
//                public void fail(int errcode) {
//                    callbackBiz.fail(errcode);
//                }
//            });
//        }
//
//        public void putJson(final EMHttpCallbackBiz callbackBiz) {
//            request.putJson(mPath, headers, mJson, new EMHttpCallback() {
//                @Override
//                public void success(String respones) {
//                    //gson thing...
//                    BeanTest bean = new BeanTest();
//                    bean.rep = respones;
//                    callbackBiz.success(bean);
//                }
//
//                @Override
//                public void progress(float persent) {
//                    callbackBiz.progress(persent);
//                }
//
//                @Override
//                public void fail(int errcode) {
//                    callbackBiz.fail(errcode);
//                }
//            });
//        }
//
//        public void postFile(final EMHttpCallbackBiz callbackBiz) {
//            request.postFile(mPath, headers, mFormName, formDataParams, mJsonName, mJson, mFileKeyName, mFilePath,
//                    mFileName, new
//                            EMHttpCallback() {
//                                @Override
//                                public void success(String respones) {
//                                    //gson thing...
//                                    BeanTest bean = new BeanTest();
//                                    bean.rep = respones;
//                                    callbackBiz.success(bean);
//                                }
//
//                                @Override
//                                public void progress(float persent) {
//                                    callbackBiz.progress(persent);
//                                }
//
//                                @Override
//                                public void fail(int errcode) {
//                                    callbackBiz.fail(errcode);
//                                }
//                            });
//        }
//
//        public void download(final EMHttpCallbackBiz callbackBiz) {
//            request.download(mPath, headers, urlParams, mDownFilePath,
//                    new
//                            EMHttpCallback() {
//                                @Override
//                                public void success(String respones) {
//                                    //gson thing...
//                                    BeanTest bean = new BeanTest();
//                                    bean.rep = respones;
//                                    callbackBiz.success(bean);
//                                }
//
//                                @Override
//                                public void progress(float persent) {
//                                    callbackBiz.progress(persent);
//                                }
//
//                                @Override
//                                public void fail(int errcode) {
//                                    callbackBiz.fail(errcode);
//                                }
//                            });
//        }
//
//    }

}
