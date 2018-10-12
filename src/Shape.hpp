#ifndef Shape_h 
#define Shape_h 2


class LedShape 
{
    public:
        LedShape();
        virtual ~LedShape();
        virtual void scale(const unsigned short noOfNodes,
                           unsigned short intensity,
                           int * (nodes[4])) = 0;
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