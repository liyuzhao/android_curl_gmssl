package com.easemob.emssl;

public interface EMHttpCallback {
    void success(String respones);
    void progress(float persent);
    void fail(int errcode);
}
