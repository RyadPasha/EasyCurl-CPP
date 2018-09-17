/**
  * @project: Easy Curl C++ Wrapper
  *
  * @purpose: This library provides a wrapper of the cURL library in C++
  * @version: 1.7
  *
  *
  * @author: Mohamed Riyad
  * @created on: 13 Sep, 2018
  *
  * @url: http://ryadpasha.com
  * @email: me@ryadpasha.com
  * @license: GNU General Public License v3.0
  *
  * @see: https://github.com/ryadpasha/easycurl-cpp
  */

#include <cstring>
#include "easycurl.h"

EasyCurl::EasyCurl(const std::string &url)
{
    m_handle = curl_easy_init();
    if ( m_handle == NULL )
        throw Exception("Unable to initialize curl handler");
    if ( url.length() == 0 )
        throw Exception("URL can't be of zero length");
    m_url = url;
}

EasyCurl::~EasyCurl()
{
    curl_easy_cleanup(m_handle);
}

void EasyCurl::setHTTPHeaders(const headers_t &hdrs) const
{
    if (! hdrs.empty())
    {
        struct curl_slist *chunk = NULL;
        for (headers_t::const_iterator i = hdrs.begin() ; i != hdrs.end() ; ++i) {
            std::string h(i->first);
            h += ": ";
            h += i->second;
            chunk = curl_slist_append(chunk, h.c_str());
        }
        CURLcode res = curl_easy_setopt(m_handle, CURLOPT_HTTPHEADER, chunk);
        if ( res != CURLE_OK)
            throw Exception(res);
    }
}

bool EasyCurl::isHTTPError() const
{
    long httpReturnCode;
    curl_easy_getinfo(m_handle, CURLINFO_RESPONSE_CODE, &httpReturnCode);
    if (httpReturnCode < 200 || httpReturnCode > 300) {
       return true;
    }
    return false;
}

std::string EasyCurl::fetch()
{
    setOptions();
    curlPerform();
    return m_data;
}

std::string EasyCurl::post(const std::string &data, const headers_t &hdrs)
{
    CURLcode res;

    res = curl_easy_setopt(m_handle, CURLOPT_POST, 1);
    if ( res != CURLE_OK)
        throw Exception(res);

    res = curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, data.c_str());
    if ( res != CURLE_OK)
        throw Exception(res);


    res = curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, data.length());
    if ( res != CURLE_OK)
        throw Exception(res);

    setOptions();

    setHTTPHeaders(hdrs);

    curlPerform();
    return m_data;
}

std::string EasyCurl::put(const std::string &data, const headers_t &hdrs)
{
    CURLcode res;

    setOptions();

    res = curl_easy_setopt(m_handle, CURLOPT_UPLOAD, 1);
    if ( res != CURLE_OK)
        throw Exception(res);

    res = curl_easy_setopt(m_handle, CURLOPT_READFUNCTION, easycurl_read_data_cb);
    if ( res != CURLE_OK)
        throw Exception(res);

    res = curl_easy_setopt(m_handle, CURLOPT_READDATA, this);
    if ( res != CURLE_OK)
        throw Exception(res);

    m_upload_data = data;

    res = curl_easy_setopt(m_handle, CURLOPT_INFILESIZE, static_cast<long>(data.size()));
    if ( res != CURLE_OK)
        throw Exception(res);

    setHTTPHeaders(hdrs);

    curlPerform();
    return m_data;
}

void EasyCurl::setOptions()
{
    CURLcode res;

    //set the url
    res = curl_easy_setopt(m_handle, CURLOPT_URL, m_url.c_str());
    if ( res != CURLE_OK)
        throw Exception(res);

    //progress bar is not require
    res = curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, 1L);
    if ( res != CURLE_OK )
        throw Exception(res);

    res = curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYPEER, 0);
    if ( res != CURLE_OK )
        throw Exception(res);

    // TODO: 0 is 'insecure'. Make it a param in EasyCurl
    res = curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYHOST, 0);
    if ( res != CURLE_OK )
        throw Exception(res);

    //set the callback function
    res = curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, easycurl_write_data_cb);
    if ( res != CURLE_OK )
        throw Exception(res);

    //set pointer in call back function
    res = curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, this);
    if ( res != CURLE_OK )
        throw Exception(res);
}

void EasyCurl::curlPerform()
{
    CURLcode res;
    res = curl_easy_perform(m_handle);
    if ( res != CURLE_OK )
        throw Exception(res);
}

size_t EasyCurl::append_data(void* ptr, size_t size, size_t nmemb)
{
    size_t bytes = size * nmemb;
    const char *s = static_cast<char*>(ptr);
    m_data += std::string(s, s + bytes);
    return bytes;
}

size_t EasyCurl::upload_data(void* ptr, size_t size, size_t nmemb)
{
    size_t upload_max = size * nmemb;
    size_t to_send = std::min(m_upload_data.size(), upload_max);
    memcpy(ptr, m_upload_data.data(), to_send);
    m_upload_data.erase(0, to_send);
    return to_send;
}


size_t easycurl_write_data_cb(void *ptr, size_t size, size_t nmemb, void* pInstance)
{
    EasyCurl* obj = reinterpret_cast<EasyCurl*>(pInstance);
    return obj->append_data(ptr, size, nmemb);
}

size_t easycurl_read_data_cb(void *ptr, size_t size, size_t nmemb, void* pInstance)
{
    EasyCurl* obj = reinterpret_cast<EasyCurl*>(pInstance);
    return obj->upload_data(ptr, size, nmemb);
}
