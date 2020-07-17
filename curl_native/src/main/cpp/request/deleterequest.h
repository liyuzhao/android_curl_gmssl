//
// Created by yutianzuo on 2018/5/8.
//

#ifndef USELIBCURL_DELETEREQUEST_H
#define USELIBCURL_DELETEREQUEST_H

#include "request.h"
#include "../include/curl/curl.h"

class HttpDeleteRequest : public HttpRequest<HttpDeleteRequest>
{
public:
    HttpDeleteRequest(HttpDeleteRequest &&) = delete;

    void operator=(HttpDeleteRequest &&) = delete;

    HttpDeleteRequest(const HttpDeleteRequest &src) = delete;

    HttpDeleteRequest &operator=(const HttpDeleteRequest &src) = delete;

    HttpDeleteRequest() : HttpRequest<HttpDeleteRequest>()
    {
    }

    void set_json(const std::string& str_data)
    {
        m_json_stru.str_json = str_data;
        m_json_stru.json = m_json_stru.str_json.c_str();
        m_json_stru.len = m_json_stru.str_json.size();
        m_json_stru.offset = 0;
    }
    void config_curl()
    {
        if (!is_valid())
        {
            return;
        }
        add_header("Content-Type: application/json;charset=utf-8");
        curl_easy_setopt(m_curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE"); // DELETE
        curl_easy_setopt(m_curl_handle, CURLOPT_READDATA, &m_json_stru);
        curl_easy_setopt(m_curl_handle, CURLOPT_READFUNCTION, read_callback);
    }

private:
    typedef struct
    {
        std::string str_json;
        const char *json = nullptr;
        int len = 0;
        int offset = 0;
    } JSONDATA;

    static size_t read_callback(char *buffer, size_t size, size_t nitems, void *instream)
    {

        JSONDATA *json = (JSONDATA *) instream;

        if (json->offset >= json->len)
        {
            return 0;
        }
        int cpy_len = 0;
        int offset = json->offset;
        if (json->len > size * nitems)
        {
            cpy_len = size * nitems;
            offset += cpy_len;
        }
        else
        {
            cpy_len = json->len;
            offset += cpy_len;
        }
        memcpy(buffer, json->json + json->offset, cpy_len);
        json->offset = offset;
        return cpy_len;

    }

    JSONDATA m_json_stru;

};

#endif //USELIBCURL_DELETEREQUEST_H
