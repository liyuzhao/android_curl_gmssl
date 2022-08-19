//
// Created by yutianzuo on 2018/5/14.
//

#ifndef USELIBCURL_REQUESTMANAGER_H
#define USELIBCURL_REQUESTMANAGER_H

#include <map>
#include <string>
#include "threadpool.h"

#include "../request/getrequest.h"
#include "../request/postfilerequest.h"
#include "../request/postrequest.h"
#include "../request/downloadrequest.h"
#include "../request/putrequest.h"
#include "../request/deleterequest.h"

class RequestManager
{
public:
    template<typename T>
    using MAP = std::map<T, T>;

    using STRING_MAP = MAP<std::string>;
    using STRING_MAP_IT = STRING_MAP::iterator;
    using CONST_STRING_MAP_IT = STRING_MAP::const_iterator;

    static void init(int n_threadpool_size)
    {
        if (!g_threadpool)
        {
            g_threadpool = new stdx::ThreadPool(n_threadpool_size, 16);
        }
    }


    static void uninit()
    {
        if (g_threadpool)
        {
            delete g_threadpool;
            g_threadpool = nullptr;
        }
    }

    /////////////////////////////////////////////////////////
    void add_basic_url_params(const std::string &param_key, const std::string &param_value)
    {
        m_basic_params[param_key] = param_value;
    }

    void add_basic_headers(const std::string &header_key, const std::string &header_value)
    {
        m_basic_headers[header_key] = header_value;
    }

    void set_host(const std::string &host)
    {
        m_host = host;
    }

    void set_cert_path(const std::string& str_cert)
    {
        m_cert_path = str_cert;
    }

    // 设置双向认证 的client证书
    void set_ssl_cert_path(const std::string &ssl_cert_path, const std::string &ssl_key_path, const std::string &ssl_key_passwd) {
        m_ssl_cert_path = ssl_cert_path;
        m_ssl_key_path = ssl_key_path;
        m_ssl_key_passwd = ssl_key_passwd;
    }

    void set_proxy_path(const std::string& str_proxy)
    {
        m_proxy_path = str_proxy;
    }

    void set_cookie(const std::string& str_cookie)
    {
        m_cookie = str_cookie;
    }

    template<typename Callback>
    void get(const std::string &str_path, const STRING_MAP &headers,
             const STRING_MAP &url_params, Callback call_back, size_t seq)
    {
        HttpGetRequest *get = new HttpGetRequest();
        inner_add_headers(get, headers);
        std::string str_url = get_url(str_path, url_params);
        get->set_url(str_url, m_skip_ssl, m_use_gmtls);
        get->set_proxy(m_proxy_path);
        get->set_cert(m_cert_path);
        get->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        get->set_cookie(m_cookie);
        get->set_callback(call_back);
        get->set_request_seq(seq);

        g_threadpool->commit([get]() -> void
                             {
                                 get->go();
                                 delete get;
                             });

    }

    int get_ret(const std::string &str_path, const STRING_MAP &headers,
             const STRING_MAP &url_params, std::string &str_res_txt)
    {
        HttpGetRequest *get = new HttpGetRequest();
        inner_add_headers(get, headers);
        std::string str_url = get_url(str_path, url_params);
        get->set_url(str_url, m_skip_ssl, m_use_gmtls);
        get->set_proxy(m_proxy_path);
        get->set_cert(m_cert_path);
        get->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        get->set_cookie(m_cookie);
        get->go();
        long http_code = get->get_httpcode();
        str_res_txt = get->get_res_txt();
        delete get;

        return http_code;
    }

    template<typename Callback>
    void post_form(const std::string &str_path, const STRING_MAP &headers,
                   const STRING_MAP &form_params, Callback call_back, size_t seq)
    {
        HttpPostFormDataRequest *post = new HttpPostFormDataRequest(true);
        inner_add_headers(post, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        post->set_url(str_url, m_skip_ssl, m_use_gmtls);
        post->set_proxy(m_proxy_path);
        post->set_cert(m_cert_path);
        post->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        post->set_cookie(m_cookie);
        post->set_request_seq(seq);
        std::string str_form;
        for (CONST_STRING_MAP_IT it = form_params.begin(); it != form_params.end(); ++it)
        {
            str_form.append(it->first);
            str_form.append("=");
            str_form.append(it->second);
            str_form.append("&");
        }
        if (str_form.size())
        {
            str_form.erase(str_form.size() - 1);
        }
        post->set_postformdata(str_form);
        post->set_callback(call_back);

        g_threadpool->commit([post]() -> void
                             {
                                 post->go();
                                 delete post;
                             });
    }

    template<typename Callback, typename CookieCallback>
    void post_form(const std::string &str_path, const STRING_MAP &headers,
                   const STRING_MAP &form_params, Callback call_back, size_t seq, CookieCallback cookie_callback)
    {
        HttpPostFormDataRequest *post = new HttpPostFormDataRequest(true);
        inner_add_headers(post, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        post->set_url(str_url, m_skip_ssl, m_use_gmtls);
        post->set_proxy(m_proxy_path);
        post->set_cert(m_cert_path);
        post->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        post->set_cookie(m_cookie);
        post->set_request_seq(seq);
        std::string str_form;
        for (CONST_STRING_MAP_IT it = form_params.begin(); it != form_params.end(); ++it)
        {
            str_form.append(it->first);
            str_form.append("=");
            str_form.append(it->second);
            str_form.append("&");
        }
        if (str_form.size())
        {
            str_form.erase(str_form.size() - 1);
        }
        post->set_postformdata(str_form);
        post->set_callback(call_back);
        post->set_cookie_callback(cookie_callback);


        g_threadpool->commit([post]() -> void
                             {
                                 post->go();
                                 delete post;
                             });
    }

    template<typename Callback, typename CookieCallback>
    void post_json(const std::string &str_path, const STRING_MAP &headers,
                   const std::string &json, Callback call_back, size_t seq, CookieCallback cookie_callback)
    {
        HttpPostFormDataRequest *post = new HttpPostFormDataRequest(false);
        inner_add_headers(post, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        post->set_url(str_url, m_skip_ssl, m_use_gmtls);
        post->set_proxy(m_proxy_path);
        post->set_cert(m_cert_path);
        post->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        post->set_cookie(m_cookie);
        post->set_request_seq(seq);
        post->set_postformdata(json);
        post->set_callback(call_back);
        post->set_cookie_callback(cookie_callback);

        g_threadpool->commit([post]() -> void
                             {
                                 post->go();
                                 delete post;
                             });
    }

    template<typename Callback>
    void post_json(const std::string &str_path, const STRING_MAP &headers,
                   const std::string &json, Callback call_back, size_t seq)
    {
        HttpPostFormDataRequest *post = new HttpPostFormDataRequest(false);
        inner_add_headers(post, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        post->set_url(str_url, m_skip_ssl, m_use_gmtls);
        post->set_proxy(m_proxy_path);
        post->set_cert(m_cert_path);
        post->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        post->set_cookie(m_cookie);
        post->set_request_seq(seq);
        post->set_postformdata(json);
        post->set_callback(call_back);

        g_threadpool->commit([post]() -> void
                             {
                                 post->go();
                                 delete post;
                             });
    }

    int post_json_ret(const std::string &str_path, const STRING_MAP &headers,
                   const std::string &json, std::string &str_res_txt)
    {
        HttpPostFormDataRequest *post = new HttpPostFormDataRequest(false);
        inner_add_headers(post, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        post->set_url(str_url, m_skip_ssl, m_use_gmtls);
        post->set_proxy(m_proxy_path);
        post->set_cert(m_cert_path);
        post->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        post->set_cookie(m_cookie);
        post->set_postformdata(json);
        post->go();

        long http_code = post->get_httpcode();
        str_res_txt = post->get_res_txt();
        delete post;

        return http_code;
    }


    template<typename Callback>
    void post_file(const std::string &str_path, const STRING_MAP &headers,
                   const std::string &str_formname, const STRING_MAP &form_params,
                   const std::string &str_jsonname, const std::string json,
                   const std::string &str_filekeyname, const std::string &str_filename,
                   const std::string &str_filepath, Callback call_back, size_t seq)
    {
        HttpPostFileRequest *post = new HttpPostFileRequest();
        inner_add_headers(post, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        post->set_url(str_url, m_skip_ssl, m_use_gmtls); //should be invoke first
        post->set_proxy(m_proxy_path);
        post->set_cert(m_cert_path);
        post->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        post->set_cookie(m_cookie);
        post->set_request_seq(seq);
        if (str_filepath.size())
        {
            post->set_filepath(str_filekeyname, str_filepath, str_filename, true);
        }
        if (form_params.size())
        {
            std::string str_form;
            for (CONST_STRING_MAP_IT it = form_params.begin(); it != form_params.end(); ++it)
            {
                str_form.append(it->first);
                str_form.append("=");
                str_form.append(it->second);
                str_form.append("&");
            }
            if (str_form.size())
            {
                str_form.erase(str_form.size() - 1);
            }
            post->set_formdata(str_formname, str_form);
        }
        if (json.size())
        {
            post->set_jsondata(str_jsonname, json);
        }
        post->set_callback(call_back);
        g_threadpool->commit([post]() -> void
                             {
                                 post->go();
                                 delete post;
                             });

    }

    template<typename Callback>
    int post_file_ret(const std::string &str_path, const STRING_MAP &headers,
                   const std::string &str_formname, const STRING_MAP &form_params,
                   const std::string &str_jsonname, const std::string json,
                   const std::string &str_filekeyname, const std::string &str_filename,
                   const std::string &str_filepath, Callback call_back, size_t seq, std::string &str_res_txt)
    {
        HttpPostFileRequest *post = new HttpPostFileRequest();
        inner_add_headers(post, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        post->set_url(str_url, m_skip_ssl, m_use_gmtls); //should be invoke first
        post->set_proxy(m_proxy_path);
        post->set_cert(m_cert_path);
        post->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        post->set_cookie(m_cookie);
        post->set_request_seq(seq);
        if (str_filepath.size())
        {
            post->set_filepath(str_filekeyname, str_filepath, str_filename, true);
        }
        if (form_params.size())
        {
            std::string str_form;
            for (CONST_STRING_MAP_IT it = form_params.begin(); it != form_params.end(); ++it)
            {
                str_form.append(it->first);
                str_form.append("=");
                str_form.append(it->second);
                str_form.append("&");
            }
            if (str_form.size())
            {
                str_form.erase(str_form.size() - 1);
            }
            post->set_formdata(str_formname, str_form);
        }
        if (json.size())
        {
            post->set_jsondata(str_jsonname, json);
        }
        post->set_callback(call_back);

        post->go();

        long http_code = post->get_httpcode();
        str_res_txt = post->get_res_txt();
        delete post;

        return http_code;
    }

    template<typename Callback>
    void put(const std::string &str_path, const std::string json,
             const STRING_MAP &headers, Callback callback, size_t seq
    )
    {
        HttpPutJsonRequest *put = new HttpPutJsonRequest();
        inner_add_headers(put, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        put->set_url(str_url, m_skip_ssl, m_use_gmtls);
        put->set_proxy(m_proxy_path);
        put->set_json(json);
        put->set_cert(m_cert_path);
        put->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        put->set_cookie(m_cookie);
        put->set_callback(callback);
        put->set_request_seq(seq);

        g_threadpool->commit([put]() -> void
                             {
                                 put->go();
                                 delete put;
                             });
    }

    int put_ret(const std::string &str_path, const std::string json,
             const STRING_MAP &headers, std::string &str_res_txt
    )
    {
        HttpPutJsonRequest *put = new HttpPutJsonRequest();
        inner_add_headers(put, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        put->set_url(str_url, m_skip_ssl, m_use_gmtls);
        put->set_proxy(m_proxy_path);
        put->set_json(json);
        put->set_cert(m_cert_path);
        put->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        put->set_cookie(m_cookie);
        put->go();
        long httpcode = put->get_httpcode();
        str_res_txt = put->get_res_txt();
        delete put;

        return httpcode;
    }



    template<typename Callback>
    void deleteMethod(const std::string &str_path, const std::string json,
            const STRING_MAP &headers, Callback callback, size_t seq
    )
    {
        HttpDeleteRequest *del = new HttpDeleteRequest();
        inner_add_headers(del, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        del->set_url(str_url, m_skip_ssl, m_use_gmtls);
        del->set_proxy(m_proxy_path);
        del->set_json(json);
        del->set_cert(m_cert_path);
        del->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        del->set_cookie(m_cookie);
        del->set_callback(callback);
        del->set_request_seq(seq);

        g_threadpool->commit([del]() -> void
                             {
                                 del->go();
                                 delete del;
                             });
    }

    int delete_ret(const std::string &str_path, const std::string json,
                      const STRING_MAP &headers, std::string &str_res_txt
    )
    {
        HttpDeleteRequest *del = new HttpDeleteRequest();
        inner_add_headers(del, headers);
        std::string str_url = get_url(str_path, STRING_MAP());
        del->set_url(str_url, m_skip_ssl, m_use_gmtls);
        del->set_proxy(m_proxy_path);
        del->set_json(json);
        del->set_cert(m_cert_path);
        del->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        del->set_cookie(m_cookie);
        del->go();
        long httpcode = del->get_httpcode();
        str_res_txt = del->get_res_txt();
        delete del;

        return httpcode;
    }



    template<typename Callback>
    void download(const std::string &str_path, const STRING_MAP &headers, const STRING_MAP &url_params,
                  const std::string &str_download_filepath,
                  Callback callback, size_t seq)
    {
        HttpGetDownloadRequest *get = new HttpGetDownloadRequest();
        inner_add_headers(get, headers);
        std::string str_url = get_url(str_path, url_params);
        get->set_url(str_url, m_skip_ssl, m_use_gmtls);
        get->set_proxy(m_proxy_path);
        get->set_cert(m_cert_path);
        get->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        get->set_cookie(m_cookie);
        get->set_download_filepath(str_download_filepath);
        get->set_callback(callback);
        get->set_request_seq(seq);

        g_threadpool->commit([get]() -> void
                             {
                                 get->go();
                                 delete get;
                             });
    }


    template<typename Callback>
    int download_ret(const std::string &str_path, const STRING_MAP &headers, const STRING_MAP &url_params,
                  const std::string &str_download_filepath,
                  Callback callback, size_t seq, std::string &str_res_txt)
    {
        HttpGetDownloadRequest *get = new HttpGetDownloadRequest();
        inner_add_headers(get, headers);
        std::string str_url = get_url(str_path, url_params);
        get->set_url(str_url, m_skip_ssl, m_use_gmtls);
        get->set_proxy(m_proxy_path);
        get->set_cert(m_cert_path);
        get->set_ssl_cert(m_ssl_cert_path, m_ssl_key_path, m_ssl_key_passwd);
        get->set_cookie(m_cookie);
        get->set_download_filepath(str_download_filepath);
        get->set_callback(callback);
        get->set_request_seq(seq);
        get->go();
        long httpcode = get->get_httpcode();
        str_res_txt = get->get_res_txt();
        delete get;

        return httpcode;
    }


private:
    bool host_valid()
    {
        return m_host.find("http") == 0;
    }

    template<typename Request>
    void inner_add_headers(Request *request, const STRING_MAP &headers)
    {
        for (auto it = m_basic_headers.begin(); it != m_basic_headers.end(); ++it)
        {
            std::string header(it->first);
            header.append(":");
            header.append(it->second);
            request->add_header(header);
        }
        for (auto it = headers.begin(); it != headers.end(); ++it)
        {
            std::string header(it->first);
            header.append(":");
            header.append(it->second);
            request->add_header(header);
        }
    }

    std::string get_url(std::string str_path, const STRING_MAP &params)
    {
        std::string str_url;
        if (host_valid())
        {
            str_url = m_host;
            if (m_host.at(m_host.size() - 1) != '/')
            {
                str_url.append("/");
            }

            if (str_path.size() && str_path.at(0) == '/')
            {
                str_path.erase(0, 1);
            }

            str_url.append(str_path);
        }
        else
        {
            str_url = str_path;
        }

        if (!str_url.empty() && str_url.at(str_url.size() - 1) == '/')
        {
            str_url.erase(str_url.size() - 1);
        }

        if (!str_url.empty() && str_url.at(str_url.size() - 1) != '?')
        {
            str_url.append("?");
        }

        for (auto it = params.begin(); it != params.end(); ++it)
        {
            str_url.append(it->first);
            str_url.append("=");
            str_url.append(it->second);
            str_url.append("&");
        }

        for (auto it = m_basic_params.begin(); it != m_basic_params.end(); ++it)
        {
            str_url.append(it->first);
            str_url.append("=");
            str_url.append(it->second);
            str_url.append("&");
        }


        if (!str_url.empty() && str_url.at(str_url.size() - 1) == '&')
        {
            str_url.erase(str_url.size() - 1);
        }

        if (!str_url.empty() && str_url.at(str_url.size() - 1) == '?')
        {
            str_url.erase(str_url.size() - 1);
        }


        return str_url;
    }

    std::string m_host;
    std::string m_cert_path;
    std::string m_proxy_path;
    std::string m_cookie;
    bool m_skip_ssl = false;
    bool m_use_gmtls = false;
    STRING_MAP m_basic_params;
    STRING_MAP m_basic_headers;

    // 双向认证 添加
    std::string m_ssl_cert_path;
    std::string m_ssl_key_path;
    std::string m_ssl_key_passwd;

    static stdx::ThreadPool *g_threadpool;

};

stdx::ThreadPool *RequestManager::g_threadpool = nullptr;

#endif //USELIBCURL_REQUESTMANAGER_H
