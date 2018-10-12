#ifndef Shape_h 
#define Shape_h 2


class LedShape 
{
    public:
        LedShape(){};
        virtual ~LedShape(){};
        virtual void scale(const unsigned short noOfNodes,
                           unsigned short intensity,
                           int * (nodes[4])) = 0;
    protected:
        unsigned short unwrapX ( unsigned short x)
        {
            short unsigned division = x/8;
            if( division == 0) 
                return x + 24;
            else if (division == 1)
                return x + 8;
            else if( division == 2)
                return x - 8;
            else
                return x - 24;
        }
        unsigned short unwrapY ( unsigned short y)
        {
            return (7-y) < 0 ? (y-7) : (7-y);
        }
};

class Square: public LedShape
{
    public:
        Square(){}
        ~Square(){}
        void scale(const unsigned short noOfNodes,
                           unsigned short intensity,
                           int * (nodes[4]));
    private:
        void fillMatrix(int, int, unsigned short, int *);
};


class Circle: public LedShape
{
    public:
        Circle() {}
        ~Circle() {}
        void scale(const unsigned short noOfNodes,
                           unsigned short intensity,
                           int * (nodes[4]));
    private:
        bool isInsideCircle (short x, short y);
        float center_;
};



#endif