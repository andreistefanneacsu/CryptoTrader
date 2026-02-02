#ifndef CRYPTOTRADER_CONTAINERSABLON_HPP
#define CRYPTOTRADER_CONTAINERSABLON_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>

template<typename Tip>
class ContainerSablon {
private:
    std::vector<Tip> elemente;
    std::function<bool(const Tip&, const Tip&)> comparator;

public:
    explicit ContainerSablon(
        std::function<bool(const Tip&, const Tip&)> comp = std::less<Tip>())
        : comparator(std::move(comp)) {}

    void adauga(const Tip& element) {
        auto it = std::lower_bound(elemente.begin(), elemente.end(), element, comparator);
        elemente.insert(it, element);
    }

    void elimina(const Tip& element) {
        auto it = std::find(elemente.begin(), elemente.end(), element);
        if (it != elemente.end()) {
            elemente.erase(it);
        }
    }

    Tip& cauta(const std::function<bool(const Tip&)>& conditie) {
        auto it = std::find_if(elemente.begin(), elemente.end(), conditie);
        if (it == elemente.end()) {
            throw std::runtime_error("Elementul nu a fost găsit");
        }
        return *it;
    }

    const Tip& cauta(const std::function<bool(const Tip&)>& conditie) const {
        auto it = std::find_if(elemente.begin(), elemente.end(), conditie);
        if (it == elemente.end()) {
            throw std::runtime_error("Elementul nu a fost găsit");
        }
        return *it;
    }

    std::vector<Tip> filtreaza(const std::function<bool(const Tip&)>& conditie) const {
        std::vector<Tip> rezultat;
        std::copy_if(elemente.begin(), elemente.end(),
                     std::back_inserter(rezultat), conditie);
        return rezultat;
    }

    void pentru_fiecare(const std::function<void(const Tip&)>& operatie) const {
        std::for_each(elemente.begin(), elemente.end(), operatie);
    }

    std::size_t size() const noexcept { return elemente.size(); }
    std::size_t dimensiune() const noexcept { return elemente.size(); }
    bool empty() const noexcept { return elemente.empty(); }
    bool este_gol() const noexcept { return elemente.empty(); }

    void clear() noexcept { elemente.clear(); }
    void goleste() noexcept { elemente.clear(); }

    bool contine(const Tip& element) const {
        return std::find(elemente.begin(), elemente.end(), element) != elemente.end();
    }

    Tip& operator[](std::size_t index) { return elemente.at(index); }
    const Tip& operator[](std::size_t index) const { return elemente.at(index); }

    auto begin() noexcept { return elemente.begin(); }
    auto end() noexcept { return elemente.end(); }
    auto begin() const noexcept { return elemente.begin(); }
    auto end() const noexcept { return elemente.end(); }

    Tip& primul() { return elemente.front(); }
    Tip& ultimul() { return elemente.back(); }
    const Tip& primul() const { return elemente.front(); }
    const Tip& ultimul() const { return elemente.back(); }
};

#endif
