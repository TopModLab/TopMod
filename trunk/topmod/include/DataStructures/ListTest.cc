#include "BaseObject.hh"
#include "List.hh"
#include <iostream.h>

class A : public BaseObject
{
  public :

     int a;

     A(int val=1)
       : BaseObject(), a(val)
       {}

     A(const A& newa)
       : BaseObject(newa), a(newa.a)
       {}

     virtual ~A()
       {}

     A& operator = (const A& newa)
       {
         BaseObject :: operator = (newa);
         a = newa.a;

         return *this;
       }

     virtual BaseObject * copy(void) const
       {
         A * newa = new A(*this);
         return newa;
       }
     
     friend ostream& operator << (ostream& o, const A& aobj)
       {
         o << "A :: a = " << aobj.a << endl;
         return o;
       }
};

typedef A * APtr;

void main(void)
{
  List list;
  A a1(1), a2(2), a3(3);
  
  cout << a1 << a2 << a3 << endl << endl;
  
  list.push_back(&a1);
  list.push_back(&a2);
  list.push_back(&a3);

  Iterator first = list.begin();
  Iterator last = list.end();

  while ( first != last )
     {
       cout << *((A *)(*first));
       ++first;
     }
  A * a4ptr;
  a4ptr = (A *)list.get(1);
  Iterator pos = list.getNode(a4ptr);
  list.insertAfter(pos,&a1);
  list.insertBefore(pos,&a3);

  cout << endl << endl;
  first = list.begin(); last = list.end();
  while ( first != last )
     {
       cout << *((A *)(*first));
       ++first;
     }
}
