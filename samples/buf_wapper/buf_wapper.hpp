#ifndef _BUF_WAPPER_H_
#define _BUF_WAPPER_H_
#include <stdlib.h>
#include <stdexcept>
#include <string.h>

template<typename T>
class buf_wapper
{
private:
    /* data */
    void *d;
    size_t s;
public:
    buf_wapper():d(nullptr),s(0){}
    buf_wapper(T* data, size_t size):d(static_cast<void *>(data)),s(size){}
    buf_wapper(size_t num) : s(sizeof(T) * num){
        d = malloc(s);
        if (!d){
            throw std::runtime_error("malloc failed");
        }
        memset(d, 0, s);
    }
    buf_wapper(const buf_wapper & o) = delete;
    buf_wapper& operator=(const buf_wapper& o) = delete;
    buf_wapper(buf_wapper&& o) noexcept:d(o.d), s(o.s){
        o.d = nullptr;
        o.s = 0 ;
    }
    T* get(){
        return static_cast<T *>(d);
    }
    T* release(){
        T* ptr = static_cast<T *>(d);
        d = nullptr;
        s = 0;
        return ptr;
    }
    void replace(T *data, size_t size){
        if (d){
            free(d);
        }
        d = data;
        s = size;
    }
    size_t size(){
        return s;
    }

    ~buf_wapper() {
        if (d){
            free(d);
        }
    }
};
#endif
