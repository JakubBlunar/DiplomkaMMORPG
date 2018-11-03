#ifndef DELETE_POINTER_ELEMENT_H
#define DELETE_POINTER_ELEMENT_H

template<class T> class DeletePointerElement
{
    public:
    // Overloaded () operator.
    // This will be called by for_each() function.
    bool operator()(T x) const
    {
        // Delete pointer.
        delete x;
        return true;
    }
};

#endif