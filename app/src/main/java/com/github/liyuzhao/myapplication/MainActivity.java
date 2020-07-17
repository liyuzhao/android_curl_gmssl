package com.github.liyuzhao.myapplication;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import android.util.Log;
import android.util.Pair;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.easemob.emssl.EMCryptUtils;
import com.easemob.emssl.EMHttpCallback;
import com.easemob.emssl.EMHttpManager;
import com.easemob.emssl.EMRequestManager;
import com.easemob.emssl.MPNetManager;
import com.easemob.emssl.utils.EMMisc;
import com.github.liyuzhao.curl.R;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    TextView mTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        MPNetManager.getInstance().init(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btnGet = findViewById(R.id.get);
        Button btnPostForm = findViewById(R.id.postform);
        Button btnPostJson = findViewById(R.id.postjson);
        Button btnPut = findViewById(R.id.put);
        Button btnPostFile = findViewById(R.id.postfile);
        Button btnDownload = findViewById(R.id.download);
        Button btnGetSync = findViewById(R.id.get_sync);
        Button btnPostFileSync = findViewById(R.id.postfile_sync);

        mTextView = findViewById(R.id.txt_info);

        btnGet.setOnClickListener(this);
        btnPostForm.setOnClickListener(this);
        btnPostJson.setOnClickListener(this);
        btnPut.setOnClickListener(this);
        btnPostFile.setOnClickListener(this);
        btnDownload.setOnClickListener(this);
        btnGetSync.setOnClickListener(this);
        btnPostFileSync.setOnClickListener(this);
        cryptoTestcase();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        MPNetManager.getInstance().unInit();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.get:
                testCaseGet();
                break;
            case R.id.postform:
                testCasePostForm();
                break;
            case R.id.postjson:
                testCasePostJson();
                break;
            case R.id.put:
                testCasePutJson();
                break;
            case R.id.postfile:
                testCasePostFile();
                break;
            case R.id.download:
                testCaseDownload();
                break;
            case R.id.get_sync:
                testCaseGetSync();
                break;
            case R.id.postfile_sync:
                testCasePostFileSync();
                break;
        }
    }


    public EMRequestManager getRequest() {
        EMRequestManager requestManager = EMHttpManager.getInstance().createRequest();
        requestManager.setHost("https://www.baidu.com");
        requestManager.setCertPath(EMMisc.getAppDir(this.getApplicationContext()) + EMMisc.CERT_NAME);
        return requestManager;
    }

    public EMRequestManager getNginxRequest() {
        EMRequestManager requestManager = EMHttpManager.getInstance().createRequest();
        requestManager.setHost("https://www.baidu.com");
        requestManager.setCertPath(EMMisc.getAppDir(this.getApplicationContext()) + EMMisc.CERT_NAME);
        return requestManager;
    }

    private void testCaseGetSync() {
        mTextView.setText("GetSync TestCase Begins");
        new Thread(new Runnable() {
            @Override
            public void run() {
                Pair<Integer, String> result = new MPNetManager.Builder(getNginxRequest())
                        .addHeader("customheader1", "value").
                        addHeader("customheader2", "value").
                        addUrlParam("customparam1", "value").
                        addUrlParam("customparam2", "value")
                        .syncGet();
                final int httpcode = result.first;
                final String response = result.second;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mTextView.setText("GetSync TestCase OK:" + httpcode + ",resp:" + response);
                    }
                });
            }
        }).start();

    }

    private void testCasePostFileSync() {
        mTextView.setText("PostFileSync TestCase Begins");
        new Thread(new Runnable() {
            @Override
            public void run() {
                final Pair<Integer, String> result = new MPNetManager.Builder()
//                .setHost("https://example.com")
                        .setUrlPath("https://a2-v2.easemob.com:443/easemob-demo/chatdemoui/chatfiles")
//                        .setJsonContent("{\"key\":\"value\"}")
                        .addHeader("Authorization", "Bearer YWMtgJidTGm7EeqP9U2--v22UU1-S6DcShHjkNXh_7qs2vXbRVmKoSMR5JyHK0eock48AwMAAAFw8efCVgBPGgBGVstEEh0HGaZuxmjiSm-Zydky970fkPY7XwLNeRDPkA")
                        .addHeader("restrict-access", "true")
                        .setFileKeyName("file").
                                setFileName("testfile.file")
                        .setLocalFilePath(EMMisc.getAppDir(MainActivity.this) + EMMisc.CERT_NAME)
                        .syncPostFile(new EMHttpCallback() {
                            @Override
                            public void success(final String respones) {
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        mTextView.setText("PostFileSync TestCase OK");
                                        Log.e("JAVA_TAG", respones);
                                    }
                                });
                            }

                            @Override
                            public void progress(final float persent) {
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        mTextView.setText("PostFileSync TestCase Progress:" + persent + "%");
                                        Log.e("JAVA_TAG", "progress");
                                    }
                                });
                            }

                            @Override
                            public void fail(final int errcode) {

                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        mTextView.setText("PostFileSync TestCase Failed:" + errcode);
                                    }
                                });
                            }
                        });

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mTextView.setText("PostFileSync Ok:" + result.first + ",resp:" + result.second);
                    }
                });
            }
        }).start();

    }

    private void testCaseGet() {
        mTextView.setText("Get TestCase Begins");
        new MPNetManager.Builder(getRequest())
                .addHeader("customheader1", "value").
                addHeader("customheader2", "value").
                addUrlParam("customparam1", "value").
                addUrlParam("customparam2", "value").
                get(new EMHttpCallback() {
                    @Override
                    public void success(String respones) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("Get TestCase OK");
                            }
                        });

                    }

                    @Override
                    public void progress(float persent) {
                        Log.e("JAVA_TAG", "progress");
                    }

                    @Override
                    public void fail(final int errcode) {
                        Log.e("JAVA_TAG", "fail:" + errcode);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("Get TestCase Failed:" + errcode);
                            }
                        });
                    }
                });
    }

    private void testCasePostForm() {
        mTextView.setText("PostForm TestCase Begins");

        EMRequestManager request = EMHttpManager.getInstance().createRequest();
        request.setHost("https://example.com");
        request.addBasicHeader("MyCookie", "1234567");
        request.setCertPath(EMMisc.getAppDir(this) + EMMisc.CERT_NAME);

        new MPNetManager.Builder(getRequest())
                .addFormData("postkey1", "postdata1")
                .addFormData("postkey2", "postdata2")
                .postFormData(new EMHttpCallback() {
                    @Override
                    public void success(final String respones) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PostForm TestCase OK");
                                Log.e("JAVA_TAG", respones);
                            }
                        });
                    }

                    @Override
                    public void progress(float persent) {
                        Log.e("JAVA_TAG", "progress");
                    }

                    @Override
                    public void fail(final int errcode) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PostForm TestCase Failed:" + errcode);
                            }
                        });

                    }
                });
    }

    private void testCasePostJson() {
        mTextView.setText("PostJson TestCase Begins");
        new MPNetManager.Builder(getRequest())
//                .setHost("https://example.com")
                .setJsonContent("{\"key\":\"value\"}")
                .postJson(new EMHttpCallback() {
                    @Override
                    public void success(final String respones) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PostJson TestCase OK");
                                Log.e("JAVA_TAG", respones);
                            }
                        });
                    }

                    @Override
                    public void progress(float persent) {
                        Log.e("JAVA_TAG", "progress");
                    }

                    @Override
                    public void fail(final int errcode) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PostJson TestCase Failed:" + errcode);
                            }
                        });

                    }
                });

    }

    private void testCasePutJson() {
        mTextView.setText("PutJson TestCase Begins");
        new MPNetManager.Builder(getRequest())
//                .setHost("https://example.com")
                .setJsonContent("{\"key\":\"value\"}")
                .putJson(new EMHttpCallback() {
                    @Override
                    public void success(final String respones) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PutJson TestCase OK");
                                Log.e("JAVA_TAG", respones);
                            }
                        });
                    }

                    @Override
                    public void progress(float persent) {
                        Log.e("JAVA_TAG", "progress");
                    }

                    @Override
                    public void fail(final int errcode) {

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PutJson TestCase Failed:" + errcode);
                            }
                        });
                    }
                });
    }



    private void testCasePostFile() {
        mTextView.setText("PostFile TestCase Begins");

        new MPNetManager.Builder(getRequest())
//                .setHost("https://example.com")
                .setUrlPath("upload.cgi")
                .setJsonContent("{\"key\":\"value\"}")
                .setFileKeyName("file").
                setFileName("testfile.file")
                .setLocalFilePath(EMMisc.getAppDir(MainActivity.this) + EMMisc.CERT_NAME)
                .postFile(new EMHttpCallback() {
                    @Override
                    public void success(final String respones) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PostFile TestCase OK");
                                Log.e("JAVA_TAG", respones);
                            }
                        });
                    }

                    @Override
                    public void progress(final float persent) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PostFile TestCase Progress:" + persent + "%");
                                Log.e("JAVA_TAG", "progress");
                            }
                        });
                    }

                    @Override
                    public void fail(final int errcode) {

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("PostFile TestCase Failed:" + errcode);
                            }
                        });
                    }
                });
    }

    private void testCaseDownload() {
        mTextView.setText("Download TestCase Begins");
        EMRequestManager requestManager = EMHttpManager.getInstance().createRequest();
        requestManager.setCertPath(EMMisc.getAppDir(this.getApplicationContext()) + EMMisc.CERT_NAME);

        new MPNetManager.Builder(requestManager)
                .setUrlPath("https://download-sdk.oss-cn-beijing.aliyuncs.com/downloads/imsdkdemo_android-3.6.5.apk")
                .setLocalFilePath(EMMisc.getAppDir(MainActivity.this) + "boss.apk")
                .downloadFile(new EMHttpCallback() {
                    @Override
                    public void success(final String respones) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("Download TestCase OK");
                                Log.e("JAVA_TAG", respones);
                            }
                        });
                    }

                    @Override
                    public void progress(final float persent) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("Download TestCase Progress:" + persent + "%");
                                Log.e("JAVA_TAG", "progress");
                            }
                        });
                    }

                    @Override
                    public void fail(final int errcode) {

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTextView.setText("Download TestCase Failed:" + errcode);
                            }
                        });
                    }
                });
    }

    private void cryptoTestcase() {
        //a demo java aes-encryt data and openssl aes-deccryt data
        byte[] aseByte = new Crypto().aesEncrypt("0123456789abcdef", "fedcba9876543210", "AndroidTestcase12345678");
        String strDecryptString = EMCryptUtils.aesCbc(aseByte, aseByte.length, "0123456789abcdef", "fedcba9876543210");

        String strSha = EMCryptUtils.sha256("abcd");
        strSha = "cpp:" + strSha;
        String strShaJava = new Crypto().SHA256("abcd");
        strShaJava = "java:" + strShaJava;
        mTextView.setText(strSha);
    }
}
