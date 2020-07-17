package com.easemob.emssl;

/**
 * crypto tool kits
 * @author liyuzhao
 * @date 2020-03-18
 */
public class EMCryptUtils {

    public static String sha256(String value) {
        return EMJniCurl.sha256(value);
    }

    public static String aesCbc(byte[] encrytData, int len, String key, String iv) {
        return EMJniCurl.aesCbc(encrytData, len, key, iv);
    }

}
