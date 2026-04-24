
#include <iostream>
using namespace std;

class object {
protected:
    string contain;
public:
    // 构造函数。
    object() = default;
    object(string _contain_) : contain(_contain_) {}
    
    // copy 函数，将 o 指针对应的类的信息拷贝到调用该函数的类对象上。
    // 保证 o 与 this 是同一种派生类指针，但会以 object * 的形式传入。
    // 具体可以参考 main 函数中的调用方式。
    virtual void copy(object* o) = 0;
    
    // 参数为某个日期，返回一个字符串，代表此日期快递的派送状态。
    virtual string send_status(int y, int m, int d) = 0;
    
    // 返回派送快递的方法。详见各个派生类的说明。
    virtual string type() = 0;
    
    // 输出保存于类内的信息，具体格式详见各个派生类的说明。
    virtual void print() {
        cout << "[object] contain: " << contain << endl;
        // 注意：本题评测时使用的 object 类会与这里的版本有微小的区别，但不会对于完成本题有影响。
    }
    
    // 析构函数。
    virtual ~object() {}
};
