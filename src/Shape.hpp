#ifndef Shape_h 
#define Shape_h 2

#include <initializer_list>

class Coordinate
{
    public:
        Coordinate(float x, float y) {
            c_[0] = x;
            c_[1] = y;
        }
        Coordinate(std::initializer_list<float> ilist)
        {
            short k = 0;
            for( auto i: ilist)
            {
                c_[k] = i;
                k++;
            }
        }
        float getX() { return c_[0];}
        float getY() { return c_[1];}
    private:
        float c_[2];

};


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
        bool isInsideCircle (float x, float y);
        float center_;
};


class Triangle: public LedShape
{
    public:
        Triangle(){}
        ~Triangle(){}
        void scale(const unsigned short noOfNodes,
                           unsigned short intensity,
                           int * (nodes[4]));
    private:
        //void drawTriangle(int, int, unsigned short, int *);
        //void drawSlope(int, int, int, unsigned short, int *);
        //void drawSquare(int, int, unsigned short, int *);
        bool isInside(Coordinate, Coordinate, Coordinate , Coordinate);
        float sign (Coordinate p1, Coordinate p2, Coordinate p3)
        {
            return (p1.getX() - p3.getX()) * 
                   (p2.getY() - p3.getY()) - 
                   (p2.getX() - p3.getX()) * 
                   (p1.getY() - p3.getY());
        }
        void traverse(unsigned short, int * (p[4]), Coordinate, Coordinate, Coordinate);
};


#endif