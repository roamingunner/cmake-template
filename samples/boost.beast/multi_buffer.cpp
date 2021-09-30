#include <iostream>
#include <string>
#include <boost/asio/buffer.hpp>
#include <boost/beast/core/ostream.hpp>
#include <boost/beast/core/read_size.hpp>
#include <boost/beast/core/multi_buffer.hpp>
#include <boost/beast/core/make_printable.hpp>

using namespace std;

int main(int argc, char const *argv[])
{
    /* code */
    {
        boost::beast::flat_buffer fb1;
        cout << "fb1.max_size = " << fb1.max_size() << " fb1.capacity = " << fb1.capacity() 
            << " fb1.size = " << fb1.capacity() << " buffer_bytes = " << boost::beast::buffer_bytes(fb1.cdata()) << endl;

        fb1.reserve(128);
        fb1.max_size(256);

        cout << "fb1.max_size = " << fb1.max_size() << " fb1.capacity = " << fb1.capacity() 
            << " fb1.size = " << fb1.capacity() << " buffer_bytes = " <<  boost::beast::buffer_bytes(fb1.cdata()) << endl;
        
        boost::beast::ostream(fb1) << "Hello, world!" << endl;

        std::cout << "fb1:" << boost::beast::make_printable(fb1.cdata()) << endl;
        fb1.consume(strlen("Hello"));
        std::cout << "fb1:" << boost::beast::make_printable(fb1.cdata()) << endl;


    }

    {
        boost::beast::flat_buffer fb2(64);
        cout << "fb2.max_size = " << fb2.max_size() << " fb2.capacity = " << fb2.capacity() 
            << " fb2.size = " << fb2.capacity() <<" buffer_bytes = " <<  boost::beast::buffer_bytes(fb2.cdata()) << endl;

        boost::beast::ostream(fb2) << "Hello, world!" << endl;
        std::cout << "fb2:" << boost::beast::make_printable(fb2.cdata()) << endl;

        fb2.shrink_to_fit();

        cout << "fb2.max_size = " << fb2.max_size() << " fb2.capacity = " << fb2.capacity() 
            << " fb2.size = " << fb2.capacity() << " buffer_bytes = " << boost::beast::buffer_bytes(fb2.cdata()) << endl;

        fb2.prepare(32);

        cout << "fb2.max_size = " << fb2.max_size() << " fb2.capacity = " << fb2.capacity() 
            << " fb2.size = " << fb2.capacity() << " buffer_bytes = " << boost::beast::buffer_bytes(fb2.cdata()) << endl;


    }




    return 0;
}
