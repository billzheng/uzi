#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cassert>
#include <tuple>
#include <stdint.h>

using namespace std;

struct QuoteLevel
{
    uint32_t Px {};
    uint32_t Qty {};
    uint32_t FilledQty {};
    uint32_t ActualFilledQty {};
    uint32_t orderQty {0};
};
std::ostream& operator<<(std::ostream& os, const QuoteLevel& o)
{
    os << o.Px << ' ' << o.Qty << ' ' << o.FilledQty << ' ' << o.ActualFilledQty << ' ' << o.orderQty;
    return os;
}

struct Order
{
    uint32_t Px {0};
    uint32_t Qty {0};
    uint32_t FilledQty {0};
    uint32_t ActualFilledQty {0};
};

std::ostream& operator<<(std::ostream& os, const Order& o)
{
    os << o.Px << ' ' << o.Qty << ' ' << o.FilledQty << ' ' << o.ActualFilledQty;
    return os;
}

struct IsGreater
{
    IsGreater() = default;
    bool operator()(const Order& order, const QuoteLevel& level) const
    {
        return order.Px > level.Px;
    }
};

struct IsEqual
{
    IsEqual() = default;
    bool operator()(const Order& order, const QuoteLevel& level) const
    {
        return order.Px == level.Px;
    }
};
enum class QuoteAction : uint8_t
{
    New = 0,
    Amend,
    Cancel
};
struct QuoteDecision
{
    QuoteDecision() = default;
    uint32_t px {0};
    uint32_t qty {0};

    QuoteAction action {};
    std::vector<Order*> orders{};
};

struct MultiQuoteSide
{
   MultiQuoteSide()
   {
       levels.push_back(QuoteLevel{100, 1000, 0, 0});
       levels.push_back(QuoteLevel{99, 1000, 0, 0});
       levels.push_back(QuoteLevel{98, 1000, 0, 0});
   
       orders.push_back(new Order{99, 1000, 0, 0});
       orders.push_back(new Order{98, 1000, 0, 0});
       orders.push_back(new Order{97, 1000, 0, 0});
   }

   template <typename InputIt1, typename InputIt2>
   void Merge(InputIt1 first1, InputIt1 last1,
                  InputIt2 first2, InputIt2 last2)
   {
        for(; first1 != last1;)
        {
            if (first2 == last2) // finished check orders
            {
                // rest of levels
                // if bid price moves down we will have rest of levels
                std::cout << "print reset of levels " << std::endl;
                for(; first1 != last1; ++first1++)
                {
                    std::cout << "level : " << *first1 << std::endl;
                }
            }
            if (IsGreater()(**first2, *first1))
            {
                std::cout << "advance order: " << **first2 << std::endl;
                ++first2;
            }
            else if (IsEqual()(**first2, *first1))
            {
               std::cout << "equal " << (*first2)->Px << ' ' << first1->Px << std::endl; 
               // maybe need to iterate orders2 to drain all orders at this px level
               for (; first2 != last2; ++first2)
               {
                    if (IsEqual()(**first2, *first1))
                    {
                        first1->orderQty += (*first2)->Qty;
                    }
                    else
                    {
                        break;
                    }
               }
               ++first1;
            }
            else
            {
                std::cout << "advance level: " << *first1 << std::endl;
                ++first1;
            }
        }
        
        if (first2 != last2)
        {
            std::cout << "print rest of orders: " << std::endl;
        }
        for (; first2 != last2; ++first2)
        {
                std::cout << "rest order: " << **first2 << std::endl;
        }
   }


   std::vector<QuoteLevel> levels;
   std::vector<Order*> orders;
};

int main(int argc, char* argv[])
{
    MultiQuoteSide bidSide;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Levels "  << std::endl;
    for (auto const& e : bidSide.levels)
    {
        std::cout << e << std::endl;
    }

    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "orders"<< std::endl;
    for (auto const& o : bidSide.orders)
    {
        std::cout << *o << std::endl;
    }
    std::cout << "---------------------------------------------------" << std::endl;
    auto const& orders = bidSide.orders;
    auto & levels = bidSide.levels;
    bidSide.Merge(levels.begin(), levels.end(), orders.cbegin(), orders.cend());

    std::cout << "New Levels "  << std::endl;
    for (auto const& e : bidSide.levels)
    {
        std::cout << e << std::endl;
    }
    return 0;
}












