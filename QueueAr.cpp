        #include "QueueAr.h"

        /**
         * Construct the queue.
         */
        template <class Object>
        Queue<Object>::Queue( int capacity ) //: theArray( capacity )
        {
            theArray = new Object[capacity];
            this->capacity = capacity;
            makeEmpty( );
        }

        /**
         * Test if the queue is logically empty.
         * Return true if empty, false otherwise.
         */
        template <class Object>
        bool Queue<Object>::isEmpty( ) const
        {
            return currentSize == 0;
        }

        /**
         * Test if the queue is logically full.
         * Return true if full, false otherwise.
         */
        /*
        template <class Object>
        bool Queue<Object>::isFull( ) const
        {
            return currentSize == theArray.size( );
        }
        */

        /**
         * Make the queue logically empty.
         */
        template <class Object>
        void Queue<Object>::makeEmpty( )
        {
            currentSize = 0;
            front = 0;
            back = -1;
        }

        /**
         * Get the least recently inserted item in the queue.
         * Return the least recently inserted item in the queue
         * or throw Underflow if empty.
         */
        template <class Object>
        const Object & Queue<Object>::getFront( ) const
        {
            //if( isEmpty( ) )
              //  throw Underflow( );
            return theArray[ front ];
        }

        /**
         * Return and remove the least recently inserted item from the queue.
         * Throw Underflow if empty.
         */
        template <class Object>
        Object Queue<Object>::dequeue( )
        {
            //if( isEmpty( ) )
              //  throw Underflow( );

            currentSize--;
            //int temp = front;
            Object frontItem = theArray[ front ];
            //increment( front );
            if(++front == capacity - 1)
              front = 0;
            return frontItem;
            //return theArray[temp];
        }

        /**
         * Insert x into the queue.
         * Throw Overflow if queue is full
         */
        template <class Object>
        void Queue<Object>::enqueue( const Object & x )
        {
            //if( isFull( ) )
              //  throw Overflow( );
            //increment( back );
            if(++back == capacity - 1)
              back = 0;
            theArray[ back ] = x;
            currentSize++;
        }

        /**
         * Internal method to increment x with wraparound.
         */
        template <class Object>
        void Queue<Object>::increment( int & x )
        {
          //++x == theArray.size( )
            if( ++x == capacity - 1)
                x = 0;
        }
