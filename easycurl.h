/**
  * @project: East Curl C++ Wrapper
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

#include <string>
#include <map>
#include <stdexcept>
#include <cstdlib>
#include <curl/curl.h>

extern "C" {
  size_t easycurl_write_data_cb(void *ptr, size_t size, size_t nmemb, void* pInstance);
  size_t easycurl_read_data_cb (void *ptr, size_t size, size_t nmemb, void* pInstance);
}

//! An awesome wrapper around Curl C Library
class EasyCurl
{
public:
    //! Exception is used for curl errors
    class Exception : public std::runtime_error
    {
    public:
        Exception(std::string const& message): std::runtime_error(message) {}
        Exception(CURLcode error): std::runtime_error(curl_easy_strerror(error)) {}
    };

    EasyCurl(const std::string &url);
    ~EasyCurl();

    typedef std::map<std::string,std::string> headers_t;

    //! Execute GET request
    std::string fetch();

    std::string post(const std::string &data, const headers_t &hdrs);

    std::string put(const std::string &data, const headers_t &hdrs);

    //! true if HTTP error occurs (404 for example)
    bool isHTTPError() const;

    //! append received data into internal buffer by write_data_callback
    size_t append_data(void* ptr, size_t size, size_t nmemb);
    size_t upload_data(void* ptr, size_t size, size_t nmemb);

private:
    //! no compiler-generated copy constructor
    EasyCurl(const EasyCurl &);

    void setOptions();
    void setHTTPHeaders(const headers_t &hdrs) const;

    void curlPerform();

    CURL *m_handle;
    std::string m_url;
    std::string m_data;
    std::string m_upload_data;
};
