#ifndef CRYPTOTRADER_SINGLETONSABLON_HPP
#define CRYPTOTRADER_SINGLETONSABLON_HPP

template<typename Tip>
class SingletonSablon {
protected:
    SingletonSablon() = default;
    virtual ~SingletonSablon() = default;
    
    SingletonSablon(const SingletonSablon&) = delete;
    SingletonSablon& operator=(const SingletonSablon&) = delete;
    
public:
    static Tip& obtine_instanta() {
        static Tip instanta;
        return instanta;
    }
};

#endif