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

#include "Logger.h"
#include "StatisticMultisetException.h"


template < class _Tp >
constexpr bool isRightType =
        ( std::is_signed < _Tp >::value || std::is_unsigned < _Tp >::value ) && !std::is_same < _Tp, char >::value;

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
 * @tparam _Tp type of class
 */
template < class _Tp >
class StatisticMultiset
{
        static_assert(isRightType < _Tp >, "template instantiation of Class are not number");
    public:
        StatisticMultiset ();
        ~StatisticMultiset ();

        void addNum (const _Tp num);
        void addNum (const std::initializer_list < _Tp > &numbers);
        void addNum (const std::multiset         < _Tp > &numbers);
        void addNum (const std::vector           < _Tp > &numbers);
        void addNum (const std::list             < _Tp > &numbers);
        void addNum (const StatisticMultiset     < _Tp > &statisticMultiset);
        void addNum (const std::string &fileName);

        _Tp getMin () const;
        _Tp getMax () const;

        float getAvg () const;

        int getCountUnder (float threshold) const;
        int getCountAbove (float threshold) const;

    protected:

        template <class _Tc>
        void addNumFromContainer (const _Tc &num);

        int fCompare (float &first, float &second) const;

    private:
        void changeCurrentValues (const _Tp &num);

        int getCountNumberOfPartOfSet(float threshold, std::pair < float, _Tp > &countNum, bool flag) const;

        bool isEmpty() const;

        std::multiset < _Tp > _numbers;
        _Tp _currentMin;
        _Tp _currentMax;

        mutable std::pair < float, _Tp > _countUnder;
        mutable std::pair < float, _Tp > _countAbove;

        float _currentAvg;

        logger::Logger _loggerMultiset;
};

template < class _Tp >
StatisticMultiset < _Tp >::StatisticMultiset () : _loggerMultiset("LOGGER", "E:\\CLionProjects\\OOP\\OOP_2\\logger.txt")
{ }

template < class _Tp >
StatisticMultiset < _Tp >::~StatisticMultiset ()
{ }

/**
 * @brief Update minimum, maximum etc. for accelerate performance.
 *
 * @tparam _Tp type of class
 * @param num the current number which add to set
 */
template < class _Tp >
void StatisticMultiset < _Tp >::changeCurrentValues (const _Tp &num)
{
    float fnum = static_cast<float>(num);

    if ( this->_numbers.empty()) {
        this->_currentMin = this->_currentMax = num;
        this->_currentAvg = fnum;

        this->_countUnder.first = this->_countAbove.first = fnum;
        this->_countUnder.second = this->_countAbove.second = 0;
    }

    this->_currentMin = std::min(this->_currentMin, num);
    this->_currentMax = std::max(this->_currentMax, num);
    this->_currentAvg += fnum;

    if ( fCompare(fnum, this->_countUnder.first) > 0 ) {
        this->_countUnder.second++;
    }

    if ( fCompare(fnum, this->_countAbove.first) < 0 ) {
        this->_countAbove.second++;
    }

    this->_loggerMultiset.log(logger::Level::INFO, "Current min: " + std::to_string(_currentMin));
    this->_loggerMultiset.log(logger::Level::INFO, "Current max: " + std::to_string(_currentMax));
    this->_loggerMultiset.log(logger::Level::INFO, "Current avg: " + std::to_string(_currentAvg / _numbers.size()));
}

/**
 * @brief Add number to set.
 *
 * @tparam _Tp type of class
 * @param num the current number which add to set
 */
template < class _Tp >
void StatisticMultiset < _Tp >::addNum (const _Tp num)
{
    changeCurrentValues(num);

    this->_loggerMultiset.log(logger::Level::INFO, "Add num: " + std::to_string(num));

    this->_numbers.emplace(num);
}

/**
 * @brief Add numbers from multiset.
 *
 * @tparam _Tp type of class
 * @param numbers multiset of numbers of type _Tp
 */
template < class _Tp >
void StatisticMultiset < _Tp >::addNum (const std::multiset < _Tp > &numbers)
{
    this->addNumFromContainer < std::multiset < _Tp > >(numbers);
}

/**
 * @brief Add numbers from vector.
 *
 * @tparam _Tp type of class
 * @param numbers vector of numbers of type _Tp
 */
template < class _Tp >
void StatisticMultiset < _Tp >::addNum (const std::vector < _Tp > &numbers)
{
    this->addNumFromContainer < std::vector < _Tp > >(numbers);
}

/**
 * @brief Add numbers from list.
 *
 * @tparam _Tp type of class
 * @param numbers list of numbers of type _Tp
 */
template < class _Tp >
void StatisticMultiset < _Tp >::addNum (const std::list < _Tp > &numbers)
{
    this->addNumFromContainer < std::list < _Tp > >(numbers);
}

/**
 * @brief Add numbers from file.
 *
 * @tparam _Tp type of class
 * @param fileName name of file where there are numbers of type _Tp
 */
template < class _Tp >
void StatisticMultiset < _Tp >::addNum (const std::string &fileName)
{
    std::ifstream in(fileName.c_str());

    if ( !in.is_open() || !in.good()) {
        std::string message = "File: '" + fileName + "' doesn't exist!\n";

        this->_loggerMultiset.log(logger::Level::FINE, message);

        throw FileDoesNotExistException(message);
    }

    _Tp num;

    this->_loggerMultiset.log(logger::Level::INFO, "Read from " + fileName);

    while ( !in.eof() && in.good()) {
        in >> num;
        addNum(num);
    }

    in.close();
}

/**
 * @brief Add numbers from StatisticMultiset.
 *
 * @tparam _Tp type of class
 * @param statisticMultiset StatisticMultiset of numbers of type _Tp
 */
template < class _Tp >
void StatisticMultiset < _Tp >::addNum (const StatisticMultiset < _Tp > &statisticMultiset)
{
    this->addNum( statisticMultiset._numbers );
}

/**
 * @brief Add numbers from initializer list.
 *
 * @tparam _Tp type of class
 * @param statisticMultiset initializer list of numbers of type _Tp
 */
template < class _Tp >
void StatisticMultiset < _Tp >::addNum (const std::initializer_list < _Tp > &numbers)
{
    this->addNumFromContainer < std::initializer_list < _Tp > >(numbers);
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
 * @tparam _Tp type of class
 * @tparam _Tc type of container
 * @param numbers container with numbers:
 * <ul>
 * <li><code>   std::multiset          < _Tp >   </code></li>
 * <li><code>   std::vector            < _Tp >   </code></li>
 * <li><code>   std::list              < _Tp >   </code></li>
 * <li><code>   std::initializer_list  < _Tp >   </code></li>
 * </ul>
 */
template <class _Tp>
template <class _Tc>
void StatisticMultiset< _Tp >::addNumFromContainer (const _Tc &numbers)
{
    if (!std::is_same< _Tc, std::multiset        < _Tp > >::value &&
        !std::is_same< _Tc, std::vector          < _Tp > >::value &&
        !std::is_same< _Tc, std::list            < _Tp > >::value &&
        !std::is_same< _Tc, std::initializer_list< _Tp > >::value)
        return;

    std::string nameType = demangle(typeid(numbers).name());

    if ( numbers.size() == 0 ) {
        this->_loggerMultiset.log(logger::Level::FINE, nameType + " is empty!");

        throw InvalidTypeException("Invalid argument!");
    }

    this->_loggerMultiset.log(logger::Level::INFO, "Add " + nameType + " to StatisticMultiset!");

    for ( auto &num: numbers ) {
        addNum(num);
    }
}

/**
 * @brief Getting the maximum number.
 *
 * @tparam _Tp type of class
 * @return maximum of set
 */
template < class _Tp >
_Tp StatisticMultiset < _Tp >::getMax () const
{
    this->isEmpty();

    this->_loggerMultiset.log(logger::Level::INFO, "Get max: " + std::to_string(_currentMax));

    return this->_currentMax;
}

/**
 * @brief Getting the minimum number.
 *
 * @tparam _Tp type of class
 * @return minimum of set
 */
template < class _Tp >
_Tp StatisticMultiset < _Tp >::getMin () const
{
    this->isEmpty();

    this->_loggerMultiset.log(logger::Level::INFO, "Get min: " + std::to_string(_currentMin));

    return this->_currentMin;
}

/**
 * @brief Getting the average.
 *
 * @tparam _Tp type of class
 * @return average of set
 */
template < class _Tp >
float StatisticMultiset < _Tp >::getAvg () const
{
    this->isEmpty();

    float tmp = this->_currentAvg / static_cast<float>(this->_numbers.size());

    this->_loggerMultiset.log(logger::Level::INFO, "Get avg: " + std::to_string(tmp));

    return tmp;
}

/**
 * @brief Get count of numbers of part of set
 *
 * @tparam _Tp type of class
 * @param threshold the number to look for
 * @param countNum
 * @param flag if true than above, else under
 * @return count of numbers
 */
template < class _Tp >
int StatisticMultiset< _Tp >::getCountNumberOfPartOfSet (float threshold,
                                                         std::pair < float, _Tp > &countNum, bool flag) const
{
    this->isEmpty();

    if ( fCompare(threshold, countNum.first) == 0 )
        return countNum.second;

    countNum.first = threshold;
    if (flag)
        countNum.second = std::distance(this->_numbers.upper_bound(threshold), this->_numbers.end());
    else
        countNum.second = std::distance(this->_numbers.begin(), this->_numbers.lower_bound(threshold));

    return countNum.second;
}

/**
 * @brief Get the number of elements is greater than the threshold.
 *
 * @tparam _Tp type of class
 * @param threshold the number to look for
 * @return amount of numbers
 */
template < class _Tp >
int StatisticMultiset < _Tp >::getCountAbove (float threshold) const
{
    return this->getCountNumberOfPartOfSet(threshold, this->_countAbove, 1);
}

/**
 * @brief Get the number of elements is less than the threshold.
 *
 * @tparam _Tp type of class
 * @param threshold the number to look for
 * @return amount of numbers
 */
template < class _Tp >
int StatisticMultiset < _Tp >::getCountUnder (float threshold) const
{
    return this->getCountNumberOfPartOfSet(threshold, this->_countUnder, 0);
}

/**
 * @brief Compare two float numbers with accuracy to 5 digits after point.
 *
 * @tparam _Tp type of class
 * @param first float number
 * @param second float number
 * @return three values:
 * <ul>
 * <li>-1 - the first number is less than the second</li>
 * <li>0 - the numbers are equal.</li>
 * <li>1 - the first number is greater than the second</li>
 * </ul>
 */
template < class _Tp >
int StatisticMultiset < _Tp >::fCompare (float &first, float &second) const
{
    float tmp = std::fabs(second - first);

    this->_loggerMultiset.log(logger::Level::INFO, "Compare: " + std::to_string(first) + "  " + std::to_string(second));

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
 * @tparam _Tp type of class
 * @return false if set isn't empty
 */
template < class  _Tp >
bool StatisticMultiset< _Tp >::isEmpty () const
{
    if ( this->_numbers.size() == 0 ){
        this->_loggerMultiset.log(logger::Level::FINE, "StatisticMultiset is empty!");

        throw InvalidArgumentException("StatisticMultiset is empty!");
    }

    return false;
}

#endif //OOP_2_STATISTICMULTISET_H