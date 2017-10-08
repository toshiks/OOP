/**
 * @file StatisticMultiset.h
 * @brief Realisation template of class StatisticMultiset
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 14.09.2017
 * @version 1.0
 */
#ifndef OOP_2_STATISTICMULTISET_H
#define OOP_2_STATISTICMULTISET_H

#include <fstream>
#include <set>
#include <vector>
#include <list>
#include <cmath>
#include <typeinfo>
#include <memory>
#include <cxxabi.h>

#include "Logger/Logger.h"
#include "StatisticMultisetException/StatisticMultisetException.h"


template < class Tp_ >
constexpr bool isRightType =
        ( std::is_signed < Tp_ >::value || std::is_unsigned < Tp_ >::value ) && !std::is_same < Tp_, char >::value;

/**
 * Template of class for storage set.
 * On the set defines operations:
 * <ol>
 * <li>Get minimum;</li>
 * <li>Get maximum;</li>
 * <li>Get average;</li>
 * <li>Get the number of elements is greater than the threshold;</li>
 * <li>Get the number of elements is less than the threshold.</li>
 * </ol>
 *
 * @tparam Tp_ type of class
 */
template < class Tp_ >
class StatisticMultiset
{
        static_assert(isRightType < Tp_ >, "template instantiation of Class are not number");
    public:
        StatisticMultiset ();
        ~StatisticMultiset ();

        void addNum (const Tp_ num);
        void addNum (const std::initializer_list < Tp_ > &numbers);
        void addNum (const std::multiset         < Tp_ > &numbers);
        void addNum (const std::vector           < Tp_ > &numbers);
        void addNum (const std::list             < Tp_ > &numbers);
        void addNum (const StatisticMultiset     < Tp_ > &statisticMultiset);
        void addNum (const std::string &fileName);

        Tp_ getMin () const;
        Tp_ getMax () const;

        float getAvg () const;

        int getCountUnder (float threshold) const;
        int getCountAbove (float threshold) const;

    protected:

        template <class Tc_>
        void addNumFromContainer (const Tc_ &num);

        int fCompare (float &first, float &second) const;

    private:
        void changeCurrentValues (const Tp_ &num);

        int getCountNumberOfPartOfSet(float threshold, std::pair < float, Tp_ > &countNum, bool flag) const;

        bool isEmpty() const;

        std::multiset < Tp_ > numbers_;
        Tp_ currentMin_;
        Tp_ currentMax_;

        mutable std::pair < float, Tp_ > countUnder_;
        mutable std::pair < float, Tp_ > countAbove_;

        float currentAvg_;

        logger::Logger loggerMultiset_;
};

template < class Tp_ >
StatisticMultiset < Tp_ >::StatisticMultiset () : loggerMultiset_("LOGGER", "E:\\CLionProjects\\OOP\\OOP\\OOP_2\\logger.txt")
{ }

template < class Tp_ >
StatisticMultiset < Tp_ >::~StatisticMultiset ()
{ }

/**
 * @brief Update minimum, maximum etc. for accelerate performance.
 *
 * @tparam Tp_ type of class
 * @param num the current number which add to set
 */
template < class Tp_ >
void StatisticMultiset < Tp_ >::changeCurrentValues (const Tp_ &num)
{
    float fnum = static_cast<float>(num);

    if ( this->numbers_.empty()) {
        this->currentMin_ = this->currentMax_ = num;
        this->currentAvg_ = fnum;

        this->countUnder_.first = this->countAbove_.first = fnum;
        this->countUnder_.second = this->countAbove_.second = 0;
    }

    this->currentMin_ = std::min(this->currentMin_, num);
    this->currentMax_ = std::max(this->currentMax_, num);
    this->currentAvg_ += fnum;

    if ( fCompare(fnum, this->countUnder_.first) > 0 ) {
        this->countUnder_.second++;
    }

    if ( fCompare(fnum, this->countAbove_.first) < 0 ) {
        this->countAbove_.second++;
    }

    this->loggerMultiset_.log(logger::Level::INFO, "Current min: " + std::to_string(currentMin_));
    this->loggerMultiset_.log(logger::Level::INFO, "Current max: " + std::to_string(currentMax_));
    this->loggerMultiset_.log(logger::Level::INFO, "Current avg: " + std::to_string(currentAvg_ / numbers_.size()));
}

/**
 * @brief Add number to set.
 *
 * @tparam Tp_ type of class
 * @param num the current number which add to set
 */
template < class Tp_ >
void StatisticMultiset < Tp_ >::addNum (const Tp_ num)
{
    changeCurrentValues(num);

    this->loggerMultiset_.log(logger::Level::INFO, "Add num: " + std::to_string(num));

    this->numbers_.emplace(num);
}

/**
 * @brief Add numbers from multiset.
 *
 * @tparam Tp_ type of class
 * @param numbers multiset of numbers of type Tp_
 */
template < class Tp_ >
void StatisticMultiset < Tp_ >::addNum (const std::multiset < Tp_ > &numbers)
{
    this->addNumFromContainer < std::multiset < Tp_ > >(numbers);
}

/**
 * @brief Add numbers from vector.
 *
 * @tparam Tp_ type of class
 * @param numbers vector of numbers of type Tp_
 */
template < class Tp_ >
void StatisticMultiset < Tp_ >::addNum (const std::vector < Tp_ > &numbers)
{
    this->addNumFromContainer < std::vector < Tp_ > >(numbers);
}

/**
 * @brief Add numbers from list.
 *
 * @tparam Tp_ type of class
 * @param numbers list of numbers of type Tp_
 */
template < class Tp_ >
void StatisticMultiset < Tp_ >::addNum (const std::list < Tp_ > &numbers)
{
    this->addNumFromContainer < std::list < Tp_ > >(numbers);
}

/**
 * @brief Add numbers from file.
 *
 * @tparam Tp_ type of class
 * @param fileName name of file where there are numbers of type Tp_
 */
template < class Tp_ >
void StatisticMultiset < Tp_ >::addNum (const std::string &fileName)
{
    std::ifstream in(fileName.c_str());

    if ( !in.is_open() || !in.good()) {
        std::string message = "File: '" + fileName + "' doesn't exist!\n";

        this->loggerMultiset_.log(logger::Level::FINE, message);

        throw FileDoesNotExistException(message);
    }

    Tp_ num;

    this->loggerMultiset_.log(logger::Level::INFO, "Read from " + fileName);

    while ( !in.eof() && in.good()) {
        in >> num;
        addNum(num);
    }

    in.close();
}

/**
 * @brief Add numbers from StatisticMultiset.
 *
 * @tparam Tp_ type of class
 * @param statisticMultiset StatisticMultiset of numbers of type Tp_
 */
template < class Tp_ >
void StatisticMultiset < Tp_ >::addNum (const StatisticMultiset < Tp_ > &statisticMultiset)
{
    this->addNum( statisticMultiset.numbers_ );
}

/**
 * @brief Add numbers from initializer list.
 *
 * @tparam Tp_ type of class
 * @param statisticMultiset initializer list of numbers of type Tp_
 */
template < class Tp_ >
void StatisticMultiset < Tp_ >::addNum (const std::initializer_list < Tp_ > &numbers)
{
    this->addNumFromContainer < std::initializer_list < Tp_ > >(numbers);
}

/**
 * @brief Demangle function's or type's name
 *
 * @param mangledName function's or type's name: typeid(fun()).name()
 * @return demangled function's name
 */
std::string demangle( const char* mangledName ) {

    std::size_t len = 0 ;
    int status = 0 ;
    std::unique_ptr< char , decltype(&std::free) > ptr(
            abi::__cxa_demangle( mangledName, nullptr, &len, &status ), &std::free ) ;
    return ptr.get() ;
}

/**
 * @brief Add numbers from containers.
 *
 * @tparam Tp_ type of class
 * @tparam _Tc type of container
 * @param numbers container with numbers:
 * <ul>
 * <li><code>   std::multiset          < Tp_ >   </code></li>
 * <li><code>   std::vector            < Tp_ >   </code></li>
 * <li><code>   std::list              < Tp_ >   </code></li>
 * <li><code>   std::initializer_list  < Tp_ >   </code></li>
 * </ul>
 */
template <class Tp_>
template <class _Tc>
void StatisticMultiset< Tp_ >::addNumFromContainer (const _Tc &numbers)
{
    if (!std::is_same< _Tc, std::multiset        < Tp_ > >::value &&
        !std::is_same< _Tc, std::vector          < Tp_ > >::value &&
        !std::is_same< _Tc, std::list            < Tp_ > >::value &&
        !std::is_same< _Tc, std::initializer_list< Tp_ > >::value)
        return;

    std::string nameType = demangle(typeid(numbers).name());

    if ( numbers.size() == 0 ) {
        this->loggerMultiset_.log(logger::Level::FINE, nameType + " is empty!");

        throw InvalidTypeException("Invalid argument!");
    }

    this->loggerMultiset_.log(logger::Level::INFO, "Add " + nameType + " to StatisticMultiset!");

    for ( auto &num: numbers ) {
        addNum(num);
    }
}

/**
 * @brief Getting the maximum number.
 *
 * @tparam Tp_ type of class
 * @return maximum of set
 */
template < class Tp_ >
Tp_ StatisticMultiset < Tp_ >::getMax () const
{
    this->isEmpty();

    this->loggerMultiset_.log(logger::Level::INFO, "Get max: " + std::to_string(currentMax_));

    return this->currentMax_;
}

/**
 * @brief Getting the minimum number.
 *
 * @tparam Tp_ type of class
 * @return minimum of set
 */
template < class Tp_ >
Tp_ StatisticMultiset < Tp_ >::getMin () const
{
    this->isEmpty();

    this->loggerMultiset_.log(logger::Level::INFO, "Get min: " + std::to_string(currentMin_));

    return this->currentMin_;
}

/**
 * @brief Getting the average.
 *
 * @tparam Tp_ type of class
 * @return average of set
 */
template < class Tp_ >
float StatisticMultiset < Tp_ >::getAvg () const
{
    this->isEmpty();

    float tmp = this->currentAvg_ / static_cast<float>(this->numbers_.size());

    this->loggerMultiset_.log(logger::Level::INFO, "Get avg: " + std::to_string(tmp));

    return tmp;
}

/**
 * @brief Get count of numbers of part of set
 *
 * @tparam Tp_ type of class
 * @param threshold the number to look for
 * @param countNum
 * @param flag if true than above, else under
 * @return count of numbers
 */
template < class Tp_ >
int StatisticMultiset< Tp_ >::getCountNumberOfPartOfSet (float threshold,
                                                         std::pair < float, Tp_ > &countNum, bool flag) const
{
    this->isEmpty();

    if ( fCompare(threshold, countNum.first) == 0 )
        return countNum.second;

    countNum.first = threshold;
    if (flag)
        countNum.second = std::distance(this->numbers_.upper_bound(threshold), this->numbers_.end());
    else
        countNum.second = std::distance(this->numbers_.begin(), this->numbers_.lower_bound(threshold));

    return countNum.second;
}

/**
 * @brief Get the number of elements is greater than the threshold.
 *
 * @tparam Tp_ type of class
 * @param threshold the number to look for
 * @return amount of numbers
 */
template < class Tp_ >
int StatisticMultiset < Tp_ >::getCountAbove (float threshold) const
{
    return this->getCountNumberOfPartOfSet(threshold, this->countAbove_, 1);
}

/**
 * @brief Get the number of elements is less than the threshold.
 *
 * @tparam Tp_ type of class
 * @param threshold the number to look for
 * @return amount of numbers
 */
template < class Tp_ >
int StatisticMultiset < Tp_ >::getCountUnder (float threshold) const
{
    return this->getCountNumberOfPartOfSet(threshold, this->countUnder_, 0);
}

/**
 * @brief Compare two float numbers with accuracy to 5 digits after point.
 *
 * @tparam Tp_ type of class
 * @param first float number
 * @param second float number
 * @return three values:
 * <ul>
 * <li>-1 - the first number is less than the second</li>
 * <li>0 - the numbers are equal.</li>
 * <li>1 - the first number is greater than the second</li>
 * </ul>
 */
template < class Tp_ >
int StatisticMultiset < Tp_ >::fCompare (float &first, float &second) const
{
    float tmp = std::fabs(second - first);

    this->loggerMultiset_.log(logger::Level::INFO, "Compare: " + std::to_string(first) + "  " + std::to_string(second));

    if ( tmp < 1e-5 ) {
        return 0;
    }

    if ( first > second )
        return 1;
    else
        return -1;
}


/**
 * @brief Generate Exception if set is empty.
 *
 * @tparam Tp_ type of class
 * @return false if set isn't empty
 */
template < class  Tp_ >
bool StatisticMultiset< Tp_ >::isEmpty () const
{
    if ( this->numbers_.size() == 0 ){
        this->loggerMultiset_.log(logger::Level::FINE, "StatisticMultiset is empty!");

        throw InvalidArgumentException("StatisticMultiset is empty!");
    }

    return false;
}

#endif //OOP_2_STATISTICMULTISET_H