#pragma once
#include <iostream>
#include <type_traits>
#include <deque>
#include <stack>
#include <set>
#include <vector>
#include <map>
#include <cmath>

/*
 For example, you are given integer X = 5 and array A such that:
 
 A[0] = 1
 A[1] = 3
 A[2] = 1
 A[3] = 4
 A[4] = 2
 A[5] = 3
 A[6] = 5
 A[7] = 4
 In second 6, a leaf falls into position 5. This is the earliest time when leaves appear in every position across the river.
 */


namespace mlp {

    
    class OrderBook{
        
    public:  // for testing leaving public
        
        enum BuySell
        {
            UNKNOWN =0,
            BUY     =1,
            SELL    =2,
        };
        
        
        struct Order
        {
            // should order have a ptr to Level..... might make things easier...
            // add the order book in which it is in as well
            
            Order(int orderId, BuySell bs, int sz, double p, bool alive=true  )
            : id( orderId), side(bs), qty(sz), price( p ), live( alive)
            {}
            
            Order(int orderId, char bs, int sz, double p, bool alive=true )
            : id( orderId), qty(sz), price( p ), live( alive)
            {
                if( bs == 'b' ) side = BuySell::BUY;
                else            side = BuySell::SELL;
            }
            
            int     id;
            BuySell side;
            int     qty;
            double  price;
            bool    live; // used for logical deletes
        };
        
        using order_map = std::map<int, Order *>;
        
        struct Level
        {
            // memory management requires more thought due to live indication/performance - right now
            // just implementing for correctness - not performance
            // would really need one location to request orders - and to request removal of them
            using order_queue = std::vector<Order *>; // ordered by some priority - ex time priority@give level
            
            int         qty;   // aggregated qty@ price point
            double      price;
            order_queue q;
            order_map   orders;
            
            
            void add_order( Order * o)
            {
                qty += o->qty;
                q.push_back( o );
                orders[ o->id ] = o ;
            }
            
            void rm_order( Order * o)
            {
                o->live = false;
                qty -= o->qty;
            }
            
            void update_order( Order * o, int quantity )
            {
                int qty_diff =  quantity - o->qty ;
                o->qty = quantity;
                qty += qty_diff;
            }
            
            bool operator<(const Level& rhs ) const 
            {
                // right now levels dont know bid/offer - so this is not quite right...
                return price < rhs.price ;
            }
            
        };
        
        using level_map    = std::map<double, Level *>;
        using level_vector = std::vector< Level *>;
        
        order_map orders_;  // orderids are unique
        level_map bids_;    // should be price_type - proxy to int double as a key not good
        level_map offers_;
        
        // level_vector bids_; shoudl maintain this really - doing it on the fly is wrong
        // level_vector bids_; level formation is less fequent then level query/update
        
    protected:
        
        static Level *allocate_level()
        {
            return new Level;
        }
        
        Level *create_level( Order *o )
        {
            Level *l = allocate_level();
            
            if(o->side == BuySell::BUY )   bids_[o->price]   = l;
            else                           offers_[o->price] = l;
            
            l->price = o->price;
            
            return l;
        }
        
        Level *find_level( Order *o )   // would leave this iterator based - but for now...
        {
            auto find_level_by_side = []( const level_map& levels, Order *o )
            {
                auto iter =  levels.find( o->price );
                if( iter == levels.end() ) return static_cast<Level *>( nullptr );
                else                       return iter -> second;
            };
            
            if( o->side == BuySell::BUY )   return find_level_by_side( bids_,   o );
            else                            return find_level_by_side( offers_, o );

        }
        
        static Order *allocate_order(int orderId, char side, int size, double price )
        {
            return new Order(orderId, side, size, price );
        }
        
    public:
        //adds order to order book
        void newOrder(int orderId, char side, int size, double price)
        {
            Order *o = allocate_order( orderId, side, size, price );
            Level *l = find_level( o );
            
            if( ! l ) l = create_level( o );
            
            orders_[ o->id ] = o ;
            
            l->add_order(o);
            
        }
        //changes quantity contained in order
        void changeOrder(int orderId, int newSize){
            
            auto iter = orders_.find( orderId );
            
            if( iter != orders_.end() )
            {
                Order *o = iter->second;
                
                Level *l = find_level( o );
                if( ! l ) throw; // this shoudl never happen - but would need to handle gracefully
                
                l->update_order(o, newSize );
            }
            
        }
        //replaces order with different order
        void replaceOrder(int oldOrderId, int orderId, char side, int size, double price){
            deleteOrder(oldOrderId);
            newOrder(orderId, side, size, price );
        }
        //deletes order from orderbook
        void deleteOrder(int orderId)
        {
            auto iter = orders_.find( orderId );
            
            if( iter != orders_.end() )
            {
                Order *o = iter->second;
                
                Level *l = find_level( o );
                if( ! l ) throw; // this should never happen - but would need to handle gracefully
                
                l->rm_order(o); // as of right now logical delete
                
                if( l->qty == 0 )
                {
                    // remove the level from the map
                    if( o->side == BuySell::BUY ) bids_.erase( o->price );
                    else                          offers_.erase( o->price );
                }
            }
            
        }
        //returns the number of levels on a side
        int getNumLevels(char side){
            // level management - from map needs to be better.... but for correctness, rather than speed

            if( side == 'b' )   return static_cast<int>( bids_.size() );
            else                return static_cast<int>( offers_.size() );
        }
        //returns the price of a level.  Level 0 is top of book.
        Level * getLevel(char side, int level) {
            // level management - from map needs to be better.... but for correctness, rather than speed
            
            auto levels_vector = []( const level_map& levels )
            {
                std::vector<Level *> ls;
                for( auto iter = levels.begin() ; iter != levels.end() ; ++iter )
                {
                    Level *l = iter -> second;
                    ls.push_back( l );
                }
                return ls;
            };
            
            if( side == 'b' )
            {
                auto compare = []( const Level * lhs, const Level * rhs )
                {
                    return lhs->price > rhs ->price ;  // woudl really need to debug this
                };
                auto levels = levels_vector( bids_ );
                std::sort(levels.begin(), levels.end(), compare ); // should sort high to low would need to debug
                
                if( ! levels.size() ) return nullptr;
                
                Level *l = levels[level];
                return l;
            }
            else
            {
                auto compare = []( const Level * lhs, const Level * rhs )
                {
                    return rhs->price > lhs ->price ;  // woudl really need to debug this and or chg so level has side
                    // and we dont need custom comparitors
                };
                auto levels = levels_vector( offers_ );
                std::sort(levels.begin(), levels.end(), compare ) ;
                if( ! levels.size() ) return nullptr;
                Level *l = levels[level];
                return l;
            }
            return 0;
        }
        
        
        //returns the price of a level.  Level 0 is top of book.
        double getLevelPrice(char side, int level) {
            // level management - from map needs to be better.... but for correctness, rather than speed
            Level *l = getLevel ( side, level );
            
            if ( l ) return l->price;
            
            return 0 ; // need some error processing
        };

        //returns the size of a level.
        int getLevelSize(char side, int level){
            Level *l = getLevel( side, level );
            if ( l ) return l->qty;
            
            return 0;
        }
        //returns the number of orders contained in price level.
        int getLevelOrderCount(char side, int level){
            
            Level *l = getLevel( side, level );
            return l->orders.size();
        }
    protected:
        
 
        
        
        
    };
   
        
        void solution()
        {
            OrderBook ob;
            
           
            ob.newOrder(1, 'b', 10, 100.25);
            ob.newOrder(2, 's', 10, 100.75);
            
     
            ob.newOrder(3, 's', 10, 100.5);
            ob.newOrder(4, 's', 50, 100.5);
            
            ob.newOrder(5, 'b', 30, 100.00);
            
            OrderBook::Level *b0 = ob.getLevel('b', 0);
            OrderBook::Level *s0 = ob.getLevel('s', 0);
            
            OrderBook::Level *b1 = ob.getLevel('b', 1);
            OrderBook::Level *s1 = ob.getLevel('s', 1);
            
            assert( b0->price == 100.25 );
            assert( s0->price == 100.5 );
            
            assert( b1->price == 100 );
            assert( s1->price == 100.75 );
            
            assert( s1->qty  == 40 );
            
            ob.changeOrder(5, 45 );
            {
                OrderBook::Level *s1 = ob.getLevel('s', 1);
                assert( s1->qty  == 75 );
            }
            
            ob.deleteOrder(5 );
            {
                OrderBook::Level *s1 = ob.getLevel('s', 1);
                assert( s1->qty  == 30 );
            }
            
            ob.deleteOrder(2); // delete a tob order
            {
                OrderBook::Level *s0 = ob.getLevel('s', 0);
                assert( s0->qty  == 30 ); // level 1 promoted to level 0
            }
            
            std::cout << "mlp::orderbook success!" << std::endl;
        };
    
    
};
    
    
