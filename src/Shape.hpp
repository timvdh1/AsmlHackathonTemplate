#ifndef Shape_h 
#define Shape_h 2


class LedShape 
{
    public:
        LedShape(){};
        virtual ~LedShape(){};
        virtual void scale(const unsigned short noOfNodes,
                           int * node1,
                           int * node2,
                           int * node3,
                           int * node4) = 0;
};

class Square: public LedShape
{
    public:
        Square(){}
        ~Square(){}
        void scale(const unsigned short noOfNodes,
                           int * node1,
                           int * node2,
                           int * node3,
                           int * node4);
    private:
        void fillMatrix(int, int, int *);
};

/*
class Rectangle: public LedShape
{
    public:
        Rectangle(bool isVertical = true, 
                  short shortAxisLength = 1): isVertical_(isVertical),
                                               shortAxisLength_(shortAxisLength)
        {}
        ~Rectangle() {}
        void scale(const unsigned short noOfNodes,
                           int & node1[32][8],
                           int & node2[32][8],
                           int & node3[32][8],
                           int & node4[32][8]);
    private:
        bool isVertical_;
        short shortAxisLength_;
        void fillMatrix(int, int, int & [32][8]);
};


class Circle: public LedShape
{
    public:
        Circle() {}
        ~Circle() {}
        void scale(const unsigned short noOfNodes,
                           int & node1[32][8],
                           int & node2[32][8],
                           int & node3[32][8],
                           int & node4[32][8]);
};

*/

#endif