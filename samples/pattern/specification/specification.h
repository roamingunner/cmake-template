#ifndef _PATTEN_SPECIFICATION_H_
#define _PATTEN_SPECIFICATION_H_

namespace tpl{
    template <class T>
    class spec
    {
    private:
        /* data */
    public:
        spec(/* args */) {}
        virtual bool is_satisified(T *candidate) = 0;
        virtual ~spec(){};
    };
}


#endif